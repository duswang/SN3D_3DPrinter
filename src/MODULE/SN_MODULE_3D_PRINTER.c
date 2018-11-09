/**
 * @file SN_MODULE_3D_PRINTER.c
 * @author Bato
 * @date 10 Oct 2018
 *
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo Finish Notify Screen = with MODULE::DISPLAY
 * @todo Z move soft limit
 * @todo Need More Printing test
 * @todo real-time get current z position info with nextion display
 */

#include "SN_API.h"
#include "SN_MODULE_3D_PRINTER.h"

#include "SN_MODULE_IMAGE_VIEWER.h"

/* ******* STATIC DEFINE ******* */
/** @name Serial Config Define */ ///@{
#define BYTE_SIZE   SN_SYS_SERIAL_COMM_RX_REALTIME
#define BAUD_RATE   SN_SYS_SERIAL_COMM_BAUD_RATE_115200
#define RETURN_MODE SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN

#ifdef __APPLE__
#define UART_DEVICE "/dev/cu.usbmodem1421"
#endif
#ifdef linux
#define UART_DEVICE "/dev/ttyACM0"
#endif

#define UART_OFLAGS  (O_RDWR | O_NOCTTY | O_NONBLOCK)
///@}

/** @name Printing Config Define */ ///@{
#define FIRST_SLICE_DELAY_TIME     5000 /**< first slice getin delay - @return msec */
#define FINISH_DEVICE_DELAY_TIME  90000 /**< finish device delay - @return msec */
#define STOP_DEVICE_DELAY_TIME    15000 /**< stop device delay - @return msec */
///@}

/** @name Other Define */ ///@{
#define GCODE_BUFFER_SIZE           30  /**< gcode max buffer size */
#define INPUT_CHAIN_CONDITION        3  /**< multiple touch condition */
///@}

/** @def ESTIMATED_BUILD_TIME_SEC_CAL(z_delay, uv_delay, uv_bottom_deleay, num_of_layer, num_of_bottom_layer)
 *  @brief estimated build time calculator
 *  ESTIMATED_BUILD_TIME_SEC_CAL - Def. funtion
 *
 *  @param z_delay
 *  @param uv_delay
 *  @param uv_bottom_delay
 *  @param num_of_layer
 *  @param num_of_bottom_layer
 *
 *  @return estimated build time (sec)
 *  @note
 */
#define ESTIMATED_BUILD_TIME_SEC_CAL(z_delay, uv_delay, uv_bottom_deleay, num_of_layer, num_of_bottom_layer) \
((((z_delay) + (uv_delay)) / 1000) * ((num_of_layer) - (num_of_bottom_layer)) + (((uv_bottom_deleay + z_delay) / 1000) * (num_of_bottom_layer)))


/* *** MODULE *** */
typedef enum device_state {
    DEVICE_STANDBY,     /**< Device Ready to Working */                    //!< DEVICE_STANDBY
    DEVICE_INIT,        /**< Device Initializing */                        //!< DEVICE_INIT
    DEVICE_PRINTING,    /**< Device Now Printing < Motor Lift or UV on > *///!< DEVICE_PRINTING
    DEVICE_PAUSE,       /**< Trying Device Pause */                        //!< DEVICE_PAUSE
    DEVICE_RESUME,      /**< Trying Device Resume */                       //!< DEVICE_RESUME
    DEVICE_STOP,        /**< Trying Device Stop */                         //!< DEVICE_STOP
    DEVICE_FINISH,      /**< Trying Device Finising */                     //!< DEVICE_FINISH
    DEVICE_BUSY,        /**< Device Working Now */                         //!< DEVICE_BUSY
    DEVICE_HOMING,      /**< Device Homing Now */                          //!< DEVICE_HOMING
    DEVICE_NONE         /**< BAD ACCESS */                                 //!< DEVICE_NONE
} deviceState_t;

typedef struct moduel_3d_printer {
    deviceState_t       state;              /**< device state */

    machineInfo_t machineInfo;              /**< machine info for print */
    printInfo_t     printInfo;              /**< print target info for print */
    uint32_t       sliceIndex;              /**< count slice when printing */

    char gcodeLiftUp[GCODE_BUFFER_SIZE];    /** gcode lift   up buffer */
    char gcodeLiftDown[GCODE_BUFFER_SIZE];  /** gcode lift down buffer */
} module3DPrinter_t;

/* ******* SYSTEM DEFINE ******* */
/* *** MODULE THREAD *** */
static pthread_mutex_t ptm3DPrinter = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       pt3DPrinter;

/* *** MODULE SERIAL *** */
sysSerialDef(3DPrinterSerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialId3DPrinter;

/* *** MODULE TIMER *** */
static sysTimerId_t timerPrint;

/* *** MODULE MESSAGE Q *** */
static sysMessageQId msgQId3DPrinter;
    /* *** MODULE MESSAGES *** */
typedef enum event_3d_printer_message {
    MSG_3D_PRINTER_PRINTING_INIT = 0,       /**< Printing Init */
    MSG_3D_PRINTER_PRINTING_CYCLE,          /**< Turn On UV and Update Image */
    MSG_3D_PRINTER_PRINTING_Z_LIFT,         /**< Motor Lift. */
    MSG_3D_PRINTER_PRINTING_PAUSE,          /**< Printing Pause */
    MSG_3D_PRINTER_PRINTING_RESUME,         /**< Printing Resume ( When Pause ) */
    MSG_3D_PRINTER_PRINTING_STOP,           /**< Printing Stop */
    MSG_3D_PRINTER_PRINTING_FINISH,         /**< Printing Finish */
    MSG_3D_PRINTER_HOMMING,                 /**< Z Homing */
    MSG_3D_PRINTER_STANDBY,                 /**< Device Standby */
    MSG_3D_PRINTER_NONE,                    /**< BAD ACCESS */
} evt3DPrinter_t;

/* *** MODULE HANDLER  *** */
static module3DPrinter_t module3DPrinter;

/* ******* GLOBAL VARIABLE ******* */
static int inputChain;

/* ******* STATIC FUNCTIONS ******* */
/* *** TIMER *** */
static void sTMR_NextCycle_Callback(void);
static void sTMR_Lift_Callback(void);
static void sTMR_StopDevice_Callback(void);
static void sTMR_FinishDevice_Callback(void);

/* *** SERIAL *** */
static void* sSerialRx_Callback(char* rxBuffer);
static SN_STATUS sSendGCode(char* command, size_t bufferSize);

/* *** SYSTEM *** */
static void*     s3DPrinterThread();
static SN_STATUS s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage);
static void      s3DPrinterEnterState(deviceState_t state);

/* *** BASIC BOARD CONTROL *** */
static SN_STATUS s3DPrinter_Homming(void);
static SN_STATUS s3DPrinter_MotorInit(void);
static SN_STATUS s3DPrinter_MotorUninit(void);

/* *** PRINTING *** */
static SN_STATUS s3DPrinter_PrintInit(void);
static SN_STATUS s3DPrinter_PrintCycle(void);
static SN_STATUS s3DPrinter_PrintLift(void);
static SN_STATUS s3DPrinter_PrintPause(void);
static SN_STATUS s3DPrinter_PrintResume(void);
static SN_STATUS s3DPrinter_PrintStop(void);
static SN_STATUS s3DPrinter_PrintFinish(void);
static SN_STATUS s3DPrinter_PrintUninit(void);

/* *** UTIL *** */
static SN_STATUS sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp);


SN_STATUS SN_MODULE_3D_PRINTER_Test(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_ON, sizeof(GCODE_LCD_ON));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    while(true);

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_MODULE_3D_PRINTER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("MODULE INIT => 3D PRINTER");

    /* MESSAGE INIT */
    retStatus = SN_SYS_MessageQInit(&msgQId3DPrinter);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Module Message Q Init Failed.");

    /* SERIAL INIT */
    serialId3DPrinter = SN_SYS_SerialCreate(sysSerial(3DPrinterSerial), sSerialRx_Callback);
    if(serialId3DPrinter == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Serial Init Failed.");
    }

    /* MUTEX INIT */
    if(pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Mutex Init Failed.");
    }

    /* THREAD INIT */
    if(pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Thread Init Failed.");
    }

    /* MODULE INIT */
    s3DPrinterEnterState(DEVICE_STANDBY);

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_STANDBY, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE);
    SN_SYS_ERROR_CHECK(retStatus, "APP Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;


    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_MotorInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinter_MotorInit();
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_MotorUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_Homing(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_Up(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char gcodeBuffer[GCODE_BUFFER_SIZE];

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        retStatus = sGcodeParser_ZMove( \
                gcodeBuffer, \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                true);

        /* Z INFO SET */
        retStatus = sSendGCode(GCODE_CLEAR_BUFFER, sizeof(GCODE_CLEAR_BUFFER));

        /* Z UP */
        retStatus = sSendGCode(gcodeBuffer, sizeof(gcodeBuffer));
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO Z CONTROL.");
    }
    else
    {
        inputChain++;

        if(inputChain >= INPUT_CHAIN_CONDITION)
        {
            inputChain = 0;
            s3DPrinterEnterState(DEVICE_STANDBY);
        }

        SN_SYS_Log("Module => 3D Printer  => MOTOR ALREADY WORKING.");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_Down(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char gcodeBuffer[GCODE_BUFFER_SIZE];

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        /* Z DOWN */
        retStatus = sGcodeParser_ZMove( \
                gcodeBuffer, \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                false);

        /* Z INFO SET */
        retStatus = sSendGCode(GCODE_CLEAR_BUFFER, sizeof(GCODE_CLEAR_BUFFER));


        retStatus = sSendGCode(gcodeBuffer, sizeof(gcodeBuffer));
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO Z CONTROL.");
    }
    else
    {
        inputChain++;

        if(inputChain >= INPUT_CHAIN_CONDITION)
        {
            inputChain = 0;
            s3DPrinterEnterState(DEVICE_STANDBY);
        }

        SN_SYS_Log("Module => 3D Printer  => MOTOR ALREADY WORKING.");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* PrintInfo Init */
    retStatus = SN_MODULE_FILE_SYSTEM_PrintInfoInit(pageIndex, itemIndex);
    SN_SYS_ERROR_CHECK(retStatus, "Get Print Info Failed.");

    /* Start Hardware Sequence */
    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Send Message Failed.");

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_INIT, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Pause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_PAUSE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Resume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_RESUME, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Stop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_STOP, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Module Thread
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static void* s3DPrinterThread()
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    /* IMAGE VIEWER INIT - IT NEED MACHINE INFO FROM "FILE SYSTEM" FOR INIT */
    retStatus = SN_MODULE_IMAGE_VIEWER_Init();
    SN_SYS_ERROR_CHECK(retStatus,"IMAGE VIEWER INIT Failed.");

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQId3DPrinter);

        switch(evt.evt_id)
        {
        case MSG_3D_PRINTER_PRINTING_INIT:
            retStatus = s3DPrinter_PrintInit();
            break;
        case MSG_3D_PRINTER_PRINTING_CYCLE:
            retStatus = s3DPrinter_PrintCycle();
            break;
        case MSG_3D_PRINTER_PRINTING_Z_LIFT:
            retStatus = s3DPrinter_PrintLift();
        break;
        case MSG_3D_PRINTER_PRINTING_PAUSE:
            retStatus = s3DPrinter_PrintPause();
            break;
        case MSG_3D_PRINTER_PRINTING_RESUME:
            retStatus = s3DPrinter_PrintResume();
            break;
        case MSG_3D_PRINTER_PRINTING_STOP:
            retStatus = s3DPrinter_PrintStop();
            break;
        case MSG_3D_PRINTER_PRINTING_FINISH:
            /* Finish Sequence */
            retStatus = s3DPrinter_PrintFinish();
            break;
        case MSG_3D_PRINTER_HOMMING:
            retStatus = s3DPrinter_Homming();
            break;
        case MSG_3D_PRINTER_STANDBY:
            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        default:
            SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "3D Printer Get Unknown Message.");
            break;
        }
        SN_SYS_ERROR_CHECK(retStatus, "3D Printer Module Get Error.");
    }

  return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Serial Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static void* sSerialRx_Callback(char* rxBuffer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(strstr(rxBuffer, GCODE_MOTOR_RESPONSE) != NULL)
    {
        switch(module3DPrinter.state)
        {
        case DEVICE_HOMING:
            SN_SYS_Log("Module => 3D Printer  => HOMING DONE.");

            retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_HOMING_DONE);
            SN_SYS_ERROR_CHECK(retStatus, "APP Send Message Failed.");

            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case DEVICE_BUSY:
            SN_SYS_Log("Module => 3D Printer  => Z MOVE DONE.");

            retStatus = sSendGCode(GCODE_GET_CURRENT_POSITION, sizeof(GCODE_GET_CURRENT_POSITION));
            SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

            retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_Z_MOVE_DONE);
            SN_SYS_ERROR_CHECK(retStatus, "APP Send Message Failed.");

            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case DEVICE_PRINTING:
        case DEVICE_PAUSE:
        case DEVICE_RESUME:
            break;
        default:
              SN_SYS_Log("Module => 3D Printer  => RESPONSE.");
              break;
        }
    }
    else if(strstr(rxBuffer, GCODE_GET_CURRENT_POSITION_RESPONSE) != NULL)
    {
        SN_SYS_Log(rxBuffer);
    }
    else
    {
#if(PRINTER_RX_DEBUG)
        SN_SYS_Log(rxBuffer);
#endif
    }

    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Timer Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sTMR_NextCycle_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");

}

static void sTMR_Lift_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_Z_LIFT, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");
}

static void sTMR_FinishDevice_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* GET CURRENT POSITION */
    retStatus = sSendGCode(GCODE_GET_CURRENT_POSITION, sizeof(GCODE_GET_CURRENT_POSITION));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    /* MOTOR DISABLE */
    retStatus = s3DPrinter_MotorUninit();
    SN_SYS_ERROR_CHECK(retStatus, "Motor Uninit Failed.");

    SN_SYS_Log("Module => 3D Printer  => PRINT FINISH.");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_FINISH);
    SN_SYS_ERROR_CHECK(retStatus,"App Message Send Failed.");
}

static void sTMR_StopDevice_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* MOTOR DISABLE */
    retStatus = s3DPrinter_MotorUninit();
    SN_SYS_ERROR_CHECK(retStatus, "Motor Uninit Failed.");

    SN_SYS_Log("Module => 3D Printer  => PRINT STOP.");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_STOP);
    SN_SYS_ERROR_CHECK(retStatus,"App Message Send Failed.");
}

/** @name Static Print Control Funtions
 *  @brief Description of Display Module Init and Uninit funtions.
 *////@{
static SN_STATUS s3DPrinter_PrintInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint32_t estimatedBuildTime = 0;

    if(module3DPrinter.state != DEVICE_BUSY || module3DPrinter.state != DEVICE_INIT)
    {
        s3DPrinterEnterState(DEVICE_INIT);

        /* Get Print Info */
        module3DPrinter.printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
        if(!module3DPrinter.printInfo.isInit)
        {
            return SN_STATUS_NOT_INITIALIZED;
        }

        /* Get Mahcine Info */
        module3DPrinter.machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
        if(!module3DPrinter.machineInfo.isInit)
        {
            return SN_STATUS_NOT_INITIALIZED;
        }

        /* Get Lift GCode */
        sGcodeParser_ZMove(module3DPrinter.gcodeLiftUp,     \
                (float)module3DPrinter.printInfo.printParameter.liftDistance, \
                (float)module3DPrinter.printInfo.printParameter.layerThickness, \
                (float)module3DPrinter.printInfo.printParameter.liftFeedRate,
                true);

        sGcodeParser_ZMove(module3DPrinter.gcodeLiftDown,  \
                (float)module3DPrinter.printInfo.printParameter.liftDistance, \
                (float)module3DPrinter.printInfo.printParameter.layerThickness, \
                (float)module3DPrinter.printInfo.printParameter.liftFeedRate,
                false);

        estimatedBuildTime = ESTIMATED_BUILD_TIME_SEC_CAL( \
                module3DPrinter.printInfo.printParameter.liftTime, \
                module3DPrinter.printInfo.printParameter.layerExposureTime, \
                module3DPrinter.printInfo.printParameter.bottomLayerExposureTime, \
                module3DPrinter.printInfo.printTarget.slice, \
                module3DPrinter.printInfo.printParameter.bottomLayerNumber);

        /* NEXTION TIMER INFO INIT */
        retStatus = SN_MODULE_DISPLAY_PrintingTimerInit(estimatedBuildTime);
        SN_SYS_ERROR_CHECK(retStatus, "Display Timer Info Update Failed.");

        /* NEXTION PAGE INFO INIT */
        retStatus = SN_MODULE_DISPLAY_PrintingInfoInit(module3DPrinter.printInfo.printTarget.targetName , "Demo");
        SN_SYS_ERROR_CHECK(retStatus, "Display Base Info Update Failed.");
        retStatus = SN_MODULE_DISPLAY_PrintingInfoUpdate((module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice);
        SN_SYS_ERROR_CHECK(retStatus, "Display Slice Info Update Failed.");

        /* Motor Init */
        s3DPrinter_MotorInit();

        /* IMAGE VIEWER CLEAER */
        retStatus = SN_MODULE_IMAGE_VIEWER_CLEAR();
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Clear Failed.");

        printf("Module => 3D Printer  => TARGET NAME [ %s ]\n", module3DPrinter.printInfo.printTarget.targetName);

        retStatus = SN_SYS_TimerCreate(&timerPrint, FIRST_SLICE_DELAY_TIME, sTMR_NextCycle_Callback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT INIT.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintCycle(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint32_t exposureTime = 0;

    /* One Cylce Start */
    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        s3DPrinterEnterState(DEVICE_PRINTING);

        SN_MODULE_DISPLAY_PrintingInfoUpdate((module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice);
        printf("Module => 3D Printer  => SLICE %04d  =========", (module3DPrinter.sliceIndex + 1)); fflush(stdout);

        /* Slice Sequence */
        /* Update Image Viewer */
        retStatus = SN_MODULE_IMAGE_VIEWER_UPDATE(module3DPrinter.sliceIndex);
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Update Failed.");

        /* UV Turn On */
        retStatus = SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_ON, sizeof(GCODE_LCD_ON));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /* Exposure */
        if(module3DPrinter.sliceIndex < module3DPrinter.printInfo.printParameter.bottomLayerNumber)
        {
            exposureTime = module3DPrinter.printInfo.printParameter.bottomLayerExposureTime;
        }
        else
        {
            exposureTime = module3DPrinter.printInfo.printParameter.layerExposureTime;
        }

        retStatus = SN_SYS_TimerCreate(&timerPrint , exposureTime, sTMR_Lift_Callback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Create Failed.");
    }
    else if(module3DPrinter.state == DEVICE_PAUSE)
    {
        SN_SYS_Log("Module => 3D Printer  => PRINT PAUSE.");

        retStatus = SN_MODULE_DISPLAY_PrintingTimerPause();
        SN_SYS_ERROR_CHECK(retStatus, "Display Printing Time Info Timer Pause Failed.");

        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PAUSE);
        SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT CYCLE.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintLift(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE))
    {
        /* Lift Sequence */
        /* UV OFF */
        retStatus = SN_SYS_SerialTx(serialId3DPrinter,GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

#if(MOTOR_DISALBE)

#else
        /* IMAGE VIEWER CLEAER */
        retStatus = SN_MODULE_IMAGE_VIEWER_CLEAR();
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Clear Failed.");

        /* Z LIFT */

        retStatus = sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");
        retStatus = sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");
#endif
        /* One Cycle Done */
        if((module3DPrinter.sliceIndex + 1) >= module3DPrinter.printInfo.printTarget.slice)
        {
            printf("=========> FINISH. [ %4d/ %4d ]\n", (module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice); fflush(stdout);

            /* Printing Finish */
            retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FINISH, 0);
            SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Failed.");
        }
        else
        {
            printf("=========> DONE. [ %4d/ %4d ]\n", (module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice); fflush(stdout);

            /* Slice Index Update */
            module3DPrinter.sliceIndex++;

            /* Next Cycle */
#if(MOTOR_DISALBE)
            retStatus = SN_SYS_TimerCreate(&timerPrint, 50, sTMR_NextCycle_Callback);
            SN_SYS_ERROR_CHECK(retStatus, "Timer Create Failed.");
#else
            retStatus = SN_SYS_TimerCreate(&timerPrint, module3DPrinter.printInfo.printParameter.liftTime, sTMR_NextCycle_Callback);
            SN_SYS_ERROR_CHECK(retStatus, "Timer Create Failed.");
#endif
        }
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT LIFT.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintPause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(module3DPrinter.state == DEVICE_PRINTING || \
       module3DPrinter.state == DEVICE_INIT)
    {
        /*s Pause Sequence */
        s3DPrinterEnterState(DEVICE_PAUSE);
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT PAUSE.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintResume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(module3DPrinter.state == DEVICE_PAUSE)
    {
        /* Resume Sequence */
        s3DPrinterEnterState(DEVICE_RESUME);

        SN_SYS_Log("Module => 3D Printer  => PRINT RESUME");

        retStatus = SN_MODULE_DISPLAY_PrintingTimerResume();
        SN_SYS_ERROR_CHECK(retStatus, "Display Printing Time Info Timer Resume Failed.");

        retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE, 0);
        SN_SYS_ERROR_CHECK(retStatus, "3D Printer Messgae Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT RESUME.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintStop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE)    || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        /* Stop Sequence */
        s3DPrinterEnterState(DEVICE_STOP);

        /* Print Uninit */
        s3DPrinter_PrintUninit();

        /* Stop Device */
        retStatus = SN_SYS_TimerCreate(&timerPrint, STOP_DEVICE_DELAY_TIME, sTMR_StopDevice_Callback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT STOP.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintFinish(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char finishGCode[30];

    if((module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE)    || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        /* Stop Sequence */
        s3DPrinterEnterState(DEVICE_FINISH);

        /* Print Uninit */
        s3DPrinter_PrintUninit();

        /* Nextion Display to Loading */
        SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);

        /* Z Position Init */
        retStatus = sSendGCode(GCODE_INIT_POSITION_ABSOLUTE, sizeof(GCODE_INIT_POSITION_ABSOLUTE));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        sGcodeParser_ZMove(finishGCode,     \
                (float)(module3DPrinter.machineInfo.height - 5), \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                true);

        retStatus = sSendGCode(finishGCode, sizeof(finishGCode));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /* Wait Z Position */
        retStatus = SN_SYS_TimerCreate(&timerPrint, FINISH_DEVICE_DELAY_TIME, sTMR_FinishDevice_Callback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT FINISH.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* Cancle Print Timer */
    retStatus = SN_SYS_TimerCancle(&timerPrint);
    SN_SYS_ERROR_CHECK(retStatus, "Timer Cancle Failed.");

    /* UV Off */
    retStatus = sSendGCode(GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    /* Image Viewer Clear */
    retStatus = SN_MODULE_IMAGE_VIEWER_CLEAR();
    SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Clear Failed.");

    /* Remove TempFile & Print Info Uninit */
    retStatus = SN_MODULE_FILE_SYSTEM_PrintInfoUninit();
    SN_SYS_ERROR_CHECK(retStatus, "Target Data Unint Failed.");

    /* STOP DISPLAY TIME INFO TIMER */
    retStatus = SN_MODULE_DISPLAY_PrintingTimerStop();
    SN_SYS_ERROR_CHECK(retStatus, "Display Printing Time Info Timer Stop Failed.");

    /* Target Value Uninit */
    pthread_mutex_lock(&ptm3DPrinter);
    module3DPrinter.sliceIndex = 0;
    strcpy(module3DPrinter.gcodeLiftUp,   GCODE_LIFT_UNINIT);
    strcpy(module3DPrinter.gcodeLiftDown, GCODE_LIFT_UNINIT);
    pthread_mutex_unlock(&ptm3DPrinter);

    return retStatus;
}

static SN_STATUS s3DPrinter_Homming(void /* uint32_t timeOut */)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_HOMING);

        SN_SYS_Log("Module => 3D Printer  => HOMING START.");
        retStatus = sSendGCode(GCODE_HOMING, sizeof(GCODE_HOMING));

        while(module3DPrinter.state == DEVICE_HOMING); /* NEED TO TIME OUT */
    }
    else
    {
        SN_SYS_Log("Module => 3D Printer  => ALREADY HOMING or BUSY.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_MotorInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("Module => 3D Printer  => MOTOR INIT.");

    retStatus = sSendGCode(GCODE_INIT_SET_MM, sizeof(GCODE_INIT_SET_MM));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    retStatus = sSendGCode(GCODE_INTT_POSITION_RELATIVE, sizeof(GCODE_INTT_POSITION_RELATIVE));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    retStatus = sSendGCode(GCODE_INIT_MOTOR, sizeof(GCODE_INIT_MOTOR));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    return retStatus;
}

static SN_STATUS s3DPrinter_MotorUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("Module => 3D Printer  => MOTOR UNINIT.");

    retStatus = sSendGCode(GCODE_UNINIT_MOTOR, sizeof(GCODE_UNINIT_MOTOR));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    return retStatus;
}
///@}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Util
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(pGcode == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    if(isUp)
    {
        sprintf(pGcode,"G1 Z%.3f F%.3f", liftDistance, liftFeedRate);
    }
    else
    {
        sprintf(pGcode,"G1 Z-%.3f F%.3f", (liftDistance - layerThickness), liftFeedRate);
    }

    return retStatus;
}

static SN_STATUS sSendGCode(char* command, size_t bufferSize)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((serialId3DPrinter == NULL))
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    if(command == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    pthread_mutex_lock(&ptm3DPrinter);
    SN_SYS_SerialTx(serialId3DPrinter, command, bufferSize);
    pthread_mutex_unlock(&ptm3DPrinter);

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  System
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage)
{
    return SN_SYS_MessagePut(&msgQId3DPrinter, evtId, evtMessage);
}

static void s3DPrinterEnterState(deviceState_t state)
{
    pthread_mutex_lock(&ptm3DPrinter);
    module3DPrinter.state = state;
    pthread_mutex_unlock(&ptm3DPrinter);
}
