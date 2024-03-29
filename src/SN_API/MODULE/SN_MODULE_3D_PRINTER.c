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
 *
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
#define STOP_DEVICE_DELAY_TIME     5000 /**< stop device delay - @return msec */

///@}

/** @name Other Define */ ///@{
#define GCODE_BUFFER_SIZE           30  /**< gcode max buffer size */
#define INPUT_CHAIN_CONDITION        3  /**< multiple touch condition */
///@}

/** @def estimatedBuildTimeSecCal(z_delay, uv_delay, uv_bottom_deleay, num_of_layer, num_of_bottom_layer)
 *  @brief estimated build time calculator
 *  estimatedBuildTimeSecCal - inline. funtion
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
static inline uint32_t estimatedBuildTimeSecCal(uint32_t z_delay, uint32_t uv_delay, uint32_t uv_bottom_deleay, uint32_t num_of_layer, uint32_t num_of_bottom_layer)
{
    uint32_t retValue = 0;

    retValue = ((((z_delay) + (uv_delay)) / 1000) * ((num_of_layer) - (num_of_bottom_layer)) + (((uv_bottom_deleay + z_delay) / 1000) * (num_of_bottom_layer)));

    return retValue;
}

/** @def currentTimeSyncByLayer(z_delay, uv_delay, uv_bottom_deleay, num_of_layer, num_of_bottom_layer)
 *  @brief current time sync
 *  currentTimeSyncByLayer - inline. funtion
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
static inline uint32_t currentTimeSyncByLayer(uint32_t z_delay, uint32_t uv_delay, uint32_t uv_bottom_deleay, uint32_t current_num_of_layer, uint32_t num_of_bottom_layer)
{
    uint32_t retValue = 0;

    retValue = (((((z_delay) + (uv_delay)) / 1000) * ((current_num_of_layer > num_of_bottom_layer) ? ((current_num_of_layer) - (num_of_bottom_layer)) : (0))) + (((uv_bottom_deleay + z_delay) / 1000) * ((current_num_of_layer > num_of_bottom_layer) ? (num_of_bottom_layer) : (current_num_of_layer))));

    return retValue;
}

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

typedef enum device_axis {
    DEVICE_AXIS_X = 0,
    DEVICE_AXIS_Y,
    DEVICE_AXIS_Z,
    DEVICE_AXIS_CNT,
    DEVICE_AXIS_MARGIN = 1
} deviceAxis_t;

typedef struct moduel_3d_printer {
    deviceState_t   prevState;
    deviceState_t       state;              /**< device state */

    uint32_t       sliceIndex;              /**< count slice when printing */

    float       current_position[DEVICE_AXIS_CNT];     /**< current axis position */
    float          prev_position[DEVICE_AXIS_CNT];     /**< current axis position */
    char           gcodeLiftUp[GCODE_BUFFER_SIZE];    /**< gcode lift   up buffer */
    char           gcodeLiftDown[GCODE_BUFFER_SIZE];  /**< gcode lift down buffer */
    char           gcodeUVLampOn[GCODE_BUFFER_SIZE];  /**< gcode UV Lamp On buffer */
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

/* ******* STATIC FUNCTIONS ******* */
/* *** TIMER *** */
static void sTMR_Z_Busy_Callback(void);
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
static SN_STATUS sGcodeZMove(float liftDistance, float liftFeedRate);
static SN_STATUS sGetCoordinates(float x, float y, float z);
static SN_STATUS sResetPrevCoordinates(void);
static SN_STATUS sSetPrevCoordinates(void);

/* *** CONSOLE *** */
static void sClrscr(void);
static void sPrintProgressPrinting(uint32_t progressed, uint32_t goal, const printOption_t printOption, const printTarget_t printTarget);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_MODULE_3D_PRINTER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("MODULE INIT => 3D PRINTER");

    SN_MODULE_DISPLAY_BootProgressUpdate(60, "Ptr. Module Loading...");
    SN_SYS_TIMER_Delay(500);

    /* MESSAGE INIT */
    msgQId3DPrinter = SN_SYS_MESSAGE_Q_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Module Message Q Init Failed.");

    /* SERIAL INIT */
    serialId3DPrinter = SN_SYS_SERIAL_Create(sysSerial(3DPrinterSerial), sSerialRx_Callback);
    if(serialId3DPrinter == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "3D Printer Serial Init Failed.");
    }

    /* MUTEX INIT */
    if(pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "3D Printer Mutex Init Failed.");
    }

    /* THREAD INIT */
    if(pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL))
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "3D Printer Thread Init Failed.");
    }

    SN_MODULE_DISPLAY_BootProgressUpdate(60, "Serial Checking...");
    SN_SYS_TIMER_Delay(3000);

    /* MODULE INIT */
    s3DPrinterEnterState(DEVICE_STANDBY);

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_STANDBY, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Send Failed");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE);
    SN_SYS_ERROR_StatusCheck(retStatus, "APP Send Message Failed.");

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
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Send Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_MotorUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //Not Used

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_Homing(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        retStatus = SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);

    	/* HOMING */
        retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);
        SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Send Failed.");
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO HOMING.");
    }
    else
    {
        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => MOTOR ALREADY WORKING.");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_Move(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        /* Z UP */
        retStatus = sGcodeZMove((float)mm, (float)DEFAULT_FEEDRATE);
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO Z CONTROL.");
    }
    else
    {
        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => MOTOR ALREADY WORKING.");
    }

    return retStatus;
}

float SN_MODULE_3D_PRINTER_Z_Position(void)
{
    return module3DPrinter.current_position[DEVICE_AXIS_Z];
}


bool SN_MODULE_3D_PRINTER_IsPrinting(void)
{
    return (module3DPrinter.state == DEVICE_PRINTING);
}

bool SN_MODULE_3D_PRINTER_IsPause(void)
{
    return (module3DPrinter.state == DEVICE_PAUSE);
}

bool SN_MODULE_3D_PRINTER_IsMotorBusy(void)
{
    return (module3DPrinter.state == DEVICE_BUSY);
}

SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex, uint32_t optionIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    deviceInfo_t deviceInfo = *SN_MODULE_FILE_SYSTEM_DeviceInfoGet();

    SN_MODULE_FILE_SYSTEM_TargetLoad(pageIndex, itemIndex);

    SN_MODULE_FILE_SYSTEM_OptionLoad(optionIndex);

    deviceInfo.optionIndex = optionIndex;

    SN_MODULE_FILE_SYSTEM_DeviceInfoUpdate(deviceInfo);

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_INIT, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Pause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_PAUSE, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Resume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_RESUME, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Stop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_STOP, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Send Message Failed.");

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
    SN_SYS_ERROR_StatusCheck(retStatus,"IMAGE VIEWER INIT Failed.");

    while(true)
    {
        evt = SN_SYS_MESSAGE_Q_Get(msgQId3DPrinter);

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
            SN_SYS_ERROR_StatusCheck(SN_STATUS_UNKNOWN_MESSAGE, "3D Printer Get Unknown Message.");
            break;
        }
        SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Module Get Error.");
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
            SN_SYS_ERROR_SystemLog("Module => 3D Printer  => HOMING DONE.");
            retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_HOMING_DONE);
            SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");

            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case DEVICE_BUSY:
            /* Check 'sTMR_Z_Busy_Callback(void)' function */
            break;
        case DEVICE_PRINTING:
        case DEVICE_PAUSE:
        case DEVICE_RESUME:
            break;
        default:
              SN_SYS_ERROR_SystemLog("Module => 3D Printer  => RESPONSE.");
              break;
        }
    }
    else
    {
#if(PRINTER_RX_DEBUG)
        SN_SYS_ERROR_SystemLog(rxBuffer);
#endif
    }

    if(strstr(rxBuffer, GCODE_GET_CURRENT_X_POSITION_RESPONSE) != NULL && \
       strstr(rxBuffer, GCODE_GET_CURRENT_Y_POSITION_RESPONSE) != NULL && \
       strstr(rxBuffer, GCODE_GET_CURRENT_Z_POSITION_RESPONSE) != NULL)
    {
        char coorX[10];
        char coorY[10];
        char coorZ[10];
        char* ptr = NULL;
        int strIndex = 0;

        /* GET X POSITION */
        ptr = strstr(rxBuffer, "X:") + 2;
        if(ptr != NULL)
        {
            while(ptr[strIndex] <= '9')
            {
               coorX[strIndex] = ptr[strIndex];

               strIndex++;
            }
            strIndex = 0;
        }

        /* GET Y POSITION */
        ptr = strstr(rxBuffer, "Y:") + 2;
        if(ptr != NULL)
        {
            while(ptr[strIndex] <= '9')
            {
               coorY[strIndex] = ptr[strIndex];

               strIndex++;
            }
            strIndex = 0;
        }

        /* GET Z POSITION */
        ptr = strstr(rxBuffer, "Z:") + 2;
        if(ptr != NULL)
        {
            while(ptr[strIndex] <= '9')
            {
               coorZ[strIndex] = ptr[strIndex];

               strIndex++;
            }
            strIndex = 0;
        }
        sGetCoordinates(strtof(coorX, NULL), strtof(coorY, NULL), strtof(coorZ, NULL));
    }
    else
    {
        /* GET CURRENT POSITION */
        retStatus = sSendGCode(GCODE_GET_CURRENT_POSITION, sizeof(GCODE_GET_CURRENT_POSITION));
        SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");
    }

    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Timer Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sTMR_Z_Busy_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Module => 3D Printer  => Z MOVE DONE.");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_Z_MOVE_DONE);
    SN_SYS_ERROR_StatusCheck(retStatus, "APP Send Message Failed.");

    s3DPrinterEnterState(module3DPrinter.prevState);
}


static void sTMR_NextCycle_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Send Failed.");

}

static void sTMR_Lift_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_Z_LIFT, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Send Failed.");
}

static void sTMR_FinishDevice_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Module => 3D Printer  => PRINT FINISH.");

    /* Printing Finish */
    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FINISH, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Message Failed.");
}

static void sTMR_StopDevice_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Module => 3D Printer  => PRINT STOP.");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_STOP);
    SN_SYS_ERROR_StatusCheck(retStatus,"App Message Send Failed.");
}

/** @name Static Print Control Funtions
 *  @brief Description of Display Module Init and Uninit funtions.
 *////@{
static SN_STATUS s3DPrinter_PrintInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint32_t estimatedBuildTime = 0;

    const printTarget_t* printTarget = NULL;
    const machineInfo_t* machineInfo = NULL;
    printOption_t printOption;

    if(module3DPrinter.state != DEVICE_BUSY || module3DPrinter.state != DEVICE_INIT)
    {
        s3DPrinterEnterState(DEVICE_INIT);

        /* Get Mahcine Info */
        machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
        if(machineInfo == NULL)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "machineInfo not initialized.");
        }

        /* Get Option */
        printOption = SN_MODULE_FILE_SYSTEM_OptionGet();

        /* Get Target */
        printTarget = SN_MODULE_FILE_SYSTEM_TargetGet();
        if(printTarget == NULL)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Target not initialized.");
        }

		if(printTarget->layerThickness > 0)
		{
	        printOption.layerThickness = printTarget->layerThickness;
		}

        /* Get Lift GCode */
        sprintf(module3DPrinter.gcodeLiftUp,"G1 Z%.3f F%.3f", (float)printOption.liftDistance, printOption.liftFeedRate);
        sprintf(module3DPrinter.gcodeLiftDown,"G1 Z-%.3f F%.3f", ((float)printOption.liftDistance - printOption.layerThickness), printOption.liftFeedRate);

        /* Get UV Lamp GCode */
        sprintf(module3DPrinter.gcodeUVLampOn, "M106 S%ld", printOption.bright);

        estimatedBuildTime = estimatedBuildTimeSecCal( \
                printOption.liftTime, \
                printOption.layerExposureTime, \
                printOption.bottomLayerExposureTime, \
                printTarget->slice, \
                printOption.bottomLayerNumber);

        /* NEXTION TIMER INFO INIT */
        retStatus = SN_MODULE_DISPLAY_PrintingTimerInit(estimatedBuildTime);
        SN_SYS_ERROR_StatusCheck(retStatus, "Display Timer Info Update Failed.");

        /* NEXTION PAGE INFO INIT */
        retStatus = SN_MODULE_DISPLAY_PrintingInfoInit(printTarget->targetName, printOption.name);
        SN_SYS_ERROR_StatusCheck(retStatus, "Display Base Info Update Failed.");

        retStatus = SN_MODULE_DISPLAY_PrintingInfoUpdate((module3DPrinter.sliceIndex + 1), printTarget->slice);
        SN_SYS_ERROR_StatusCheck(retStatus, "Display Slice Info Update Failed.");

        /* Motor Init */
        s3DPrinter_MotorInit();

        /* IMAGE VIEWER CLEAER */
        retStatus = SN_MODULE_IMAGE_VIEWER_WindowClean();
        SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Window Clear Failed.");

        retStatus = SN_MODULE_IMAGE_VIEWER_ThumbnailClean();
        SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Update Failed.");


        printf("Module => 3D Printer  => TARGET NAME  [ %s ]\n", printTarget->targetName);
        printf("Module => 3D Printer  => TARGET SLICE [ %04d ]\n", printTarget->slice);
        printf("Module => 3D Printer  => OPTION NAME  [ %s ]\n", printOption.name);

        retStatus = SN_SYS_TIMER_Create(&timerPrint, FIRST_SLICE_DELAY_TIME, sTMR_NextCycle_Callback);
        SN_SYS_ERROR_StatusCheck(retStatus, "Timer Cretae Failed.");

        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_START);
        SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT INIT.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintCycle(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    uint32_t exposureTime = 0;

    const printTarget_t* printTarget = NULL;
    printOption_t printOption;

    /* One Cylce Start */
    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        s3DPrinterEnterState(DEVICE_PRINTING);

        /* Get Option */
        printOption = SN_MODULE_FILE_SYSTEM_OptionGet();

        /* Get Target */
        printTarget = SN_MODULE_FILE_SYSTEM_TargetGet();
        if(printTarget == NULL)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Target not initialized.");
        }

		if(printTarget->layerThickness > 0)
		{
	        printOption.layerThickness = printTarget->layerThickness;
		}

        /* Console Clear */
        sClrscr();

        /* Console Display */
        sPrintProgressPrinting(module3DPrinter.sliceIndex + 1 ,printTarget->slice, printOption, *printTarget);

        /* Slice Sequence */
        /* IMAGE VIEWER UPDATE */
        retStatus = SN_MODULE_IMAGE_VIEWER_WindowUpdate(module3DPrinter.sliceIndex);
        SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Update Failed.");

        /* UV Turn On */
        retStatus = SN_SYS_SERIAL_Tx(serialId3DPrinter, module3DPrinter.gcodeUVLampOn, sizeof(module3DPrinter.gcodeUVLampOn));
        SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

        SN_MODULE_DISPLAY_PrintingInfoUpdate((module3DPrinter.sliceIndex + 1), printTarget->slice);

        /* Exposure Timer Call */
        if(module3DPrinter.sliceIndex < printOption.bottomLayerNumber)
        {
            exposureTime = printOption.bottomLayerExposureTime;
        }
        else
        {
            exposureTime = printOption.layerExposureTime;
        }

        retStatus = SN_SYS_TIMER_Create(&timerPrint , exposureTime, sTMR_Lift_Callback);
        SN_SYS_ERROR_StatusCheck(retStatus, "Timer Create Failed.");
    }
    else if(module3DPrinter.state == DEVICE_PAUSE)
    {
        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => PRINT PAUSE.");

        retStatus = SN_MODULE_DISPLAY_PrintingTimerPause();
        SN_SYS_ERROR_StatusCheck(retStatus, "Display Printing Time Info Timer Pause Failed.");

        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PAUSE);
        SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT CYCLE.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintLift(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    const printTarget_t* printTarget = NULL;
    printOption_t printOption;

    uint32_t currentTime = 0;

    if((module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE))
    {
        /* Get Option */
        printOption = SN_MODULE_FILE_SYSTEM_OptionGet();

        /* Get Target */
        printTarget = SN_MODULE_FILE_SYSTEM_TargetGet();
        if(printTarget == NULL)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Target not initialized.");
        }

		if(printTarget->layerThickness > 0)
		{
	        printOption.layerThickness = printTarget->layerThickness;
		}

        /* Lift Sequence */
        /* UV OFF */
        retStatus = SN_SYS_SERIAL_Tx(serialId3DPrinter,GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
        SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

#if(MOTOR_DISALBE)

#else
        /* IMAGE VIEWER CLEAER */
        retStatus = SN_MODULE_IMAGE_VIEWER_WindowClean();
        SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Window Clear Failed.");

        retStatus = SN_MODULE_IMAGE_VIEWER_ThumbnailClean();
        SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Update Failed.");

        /* Z LIFT */
        retStatus = sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
        SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

        retStatus = sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
        SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");


#endif
        /* One Cycle Done */
        if((module3DPrinter.sliceIndex + 1) >= printTarget->slice)
        {
            retStatus = SN_SYS_TIMER_Create(&timerPrint, printOption.liftTime, sTMR_FinishDevice_Callback);
            SN_SYS_ERROR_StatusCheck(retStatus, "Timer Create Failed.");
        }
        else
        {
            /* Time Sync */
            currentTime = currentTimeSyncByLayer( \
                    printOption.liftTime, \
                    printOption.layerExposureTime, \
                    printOption.bottomLayerExposureTime, \
                    module3DPrinter.sliceIndex + 1, \
                    printOption.bottomLayerNumber);

            SN_MODULE_DISPLAY_PrintingTimerSync(currentTime);

            /* Slice Index Update */
            module3DPrinter.sliceIndex++;

            /* Next Cycle */
#if(MOTOR_DISALBE)
            retStatus = SN_SYS_TIMER_Create(&timerPrint, 50, sTMR_NextCycle_Callback);
            SN_SYS_ERROR_StatusCheck(retStatus, "Timer Create Failed.");
#else
            retStatus = SN_SYS_TIMER_Create(&timerPrint, printOption.liftTime, sTMR_NextCycle_Callback);
            SN_SYS_ERROR_StatusCheck(retStatus, "Timer Create Failed.");
#endif
        }
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT LIFT.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintPause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(module3DPrinter.state == DEVICE_PRINTING || \
       module3DPrinter.state == DEVICE_INIT)
    {
        /* Pause Sequence */
        s3DPrinterEnterState(DEVICE_PAUSE);

        /* Set Prev Motor Position */
        sSetPrevCoordinates();
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT PAUSE.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintResume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    float mm = module3DPrinter.prev_position[DEVICE_AXIS_Z] - module3DPrinter.current_position[DEVICE_AXIS_Z];

    if(module3DPrinter.state == DEVICE_PAUSE)
    {
        /* Resume Sequence */
        s3DPrinterEnterState(DEVICE_RESUME);

        /* Nextion Display to Loading */
        SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);

        sGcodeZMove((float)(mm), (float)DEFAULT_FEEDRATE);

        while(module3DPrinter.state == DEVICE_BUSY); /* NEED TO TIME OUT */

        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => PRINT RESUME");

        /* Reset Prev Motor Position */
        sResetPrevCoordinates();

        retStatus = SN_MODULE_DISPLAY_PrintingTimerResume();
        SN_SYS_ERROR_StatusCheck(retStatus, "Display Printing Time Info Timer Resume Failed.");


        /* Send App Message */
        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_RESUME);
        SN_SYS_ERROR_StatusCheck(retStatus,"App Message Send Failed.");

        retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE, 0);
        SN_SYS_ERROR_StatusCheck(retStatus, "3D Printer Messgae Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT RESUME.");
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

        /* Reset Prev Motor Position */
        sResetPrevCoordinates();

        /* Print Uninit */
        s3DPrinter_PrintUninit();

        /* Stop Device */
        retStatus = SN_SYS_TIMER_Create(&timerPrint, STOP_DEVICE_DELAY_TIME, sTMR_StopDevice_Callback);
        SN_SYS_ERROR_StatusCheck(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT STOP.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintFinish(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    const machineInfo_t* machineInfo = NULL;

    if((module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE)    || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        /* Get Mahcine Info */
        machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
        if(machineInfo == NULL)
        {
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "machineInfo not initialized.");
        }

        /* Print Uninit */
        s3DPrinter_PrintUninit();

        s3DPrinterEnterState(DEVICE_FINISH);

        /* Nextion Display to Loading */
        SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);

        sGcodeZMove((float)(machineInfo->machineHeight), (float)DEFAULT_FEEDRATE);

        while(module3DPrinter.state == DEVICE_BUSY); /* NEED TO TIME OUT */

        /* Send App Message */
        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_FINISH);
        SN_SYS_ERROR_StatusCheck(retStatus,"App Message Send Failed.");
    }
    else
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT FINISH.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* Cancle Print Timer */
    retStatus = SN_SYS_TIMER_Cancel(&timerPrint);
    SN_SYS_ERROR_StatusCheck(retStatus, "Timer Cancle Failed.");

    /* UV Off */
    retStatus = sSendGCode(GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
    SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

    /* Image Viewer Clear */
    retStatus = SN_MODULE_IMAGE_VIEWER_WindowClean();
    SN_SYS_ERROR_StatusCheck(retStatus, "Image Viewer Clear Failed.");

    /* Remove Target & Option */
    retStatus = SN_MODULE_FILE_SYSTEM_TargetDestroy();
    SN_SYS_ERROR_StatusCheck(retStatus, "Target Data Remove Failed.");

    /* STOP DISPLAY TIME INFO TIMER */
    retStatus = SN_MODULE_DISPLAY_PrintingTimerStop();
    SN_SYS_ERROR_StatusCheck(retStatus, "Display Printing Time Info Timer Stop Failed.");

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

        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => HOMING START.");
        retStatus = sSendGCode(GCODE_HOMING, sizeof(GCODE_HOMING));

        while(module3DPrinter.state == DEVICE_HOMING); /* NEED TO TIME OUT */
    }
    else
    {
        SN_SYS_ERROR_SystemLog("Module => 3D Printer  => ALREADY HOMING or BUSY.");
    }

    return retStatus;
}

static SN_STATUS s3DPrinter_MotorInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Module => 3D Printer  => MOTOR INIT.");

    retStatus = sSendGCode(GCODE_INIT_SET_MM, sizeof(GCODE_INIT_SET_MM));
    SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

    retStatus = sSendGCode(GCODE_INTT_POSITION_RELATIVE, sizeof(GCODE_INTT_POSITION_RELATIVE));
    SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

    retStatus = sSendGCode(GCODE_INIT_MOTOR, sizeof(GCODE_INIT_MOTOR));
    SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

    return retStatus;
}

static SN_STATUS s3DPrinter_MotorUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Module => 3D Printer  => MOTOR UNINIT.");

    retStatus = sSendGCode(GCODE_UNINIT_MOTOR, sizeof(GCODE_UNINIT_MOTOR));
    SN_SYS_ERROR_StatusCheck(retStatus, "Send GCode Failed.");

    return retStatus;
}
///@}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Util
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sGetCoordinates(float x, float y, float z)
{
    module3DPrinter.current_position[DEVICE_AXIS_X] = x;
    module3DPrinter.current_position[DEVICE_AXIS_Y] = y;
    module3DPrinter.current_position[DEVICE_AXIS_Z] = z;

    SN_MODULE_DISPLAY_ControlZPosition(module3DPrinter.current_position[DEVICE_AXIS_Z]);

    return SN_STATUS_OK;
}

static SN_STATUS sResetPrevCoordinates(void)
{
    module3DPrinter.prev_position[DEVICE_AXIS_X] = 0;
    module3DPrinter.prev_position[DEVICE_AXIS_Y] = 0;
    module3DPrinter.prev_position[DEVICE_AXIS_Z] = 0;

    return SN_STATUS_OK;
}
static SN_STATUS sSetPrevCoordinates(void)
{
    module3DPrinter.prev_position[DEVICE_AXIS_X] = module3DPrinter.current_position[DEVICE_AXIS_X];
    module3DPrinter.prev_position[DEVICE_AXIS_Y] = module3DPrinter.current_position[DEVICE_AXIS_Y];
    module3DPrinter.prev_position[DEVICE_AXIS_Z] = module3DPrinter.current_position[DEVICE_AXIS_Z];

    return SN_STATUS_OK;
}

static SN_STATUS sGcodeZMove(float liftDistance, float liftFeedRate)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char gcodeBuffer[GCODE_BUFFER_SIZE];
    const machineInfo_t* machineInfo = NULL;

    module3DPrinter.prevState = module3DPrinter.state;

    s3DPrinterEnterState(DEVICE_BUSY);

    /* Get Mahcine Info */
    machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(machineInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "machineInfo not initialized.");
    }

    if((machineInfo->machineHeight - DEVICE_AXIS_MARGIN) <= (module3DPrinter.current_position[DEVICE_AXIS_Z] + liftDistance))
    {
        liftDistance = (machineInfo->machineHeight - DEVICE_AXIS_MARGIN) - module3DPrinter.current_position[DEVICE_AXIS_Z];
    }
    else if(0 > module3DPrinter.current_position[DEVICE_AXIS_Z] + liftDistance)
    {
        if((module3DPrinter.prevState == DEVICE_PRINTING) || (module3DPrinter.prevState == DEVICE_PAUSE) || (module3DPrinter.prevState == DEVICE_RESUME))
        {
            liftDistance = -module3DPrinter.current_position[DEVICE_AXIS_Z];
        }
        else
        {
            /* Z Down Unlimit */
        }
    }

    sprintf(gcodeBuffer,"G1 Z%.3f F%.3f", liftDistance, liftFeedRate);

    retStatus = sSendGCode(gcodeBuffer, sizeof(gcodeBuffer));

    /* Wait Next Key with Z Delay */
    if(liftDistance < 0)
    {
        liftDistance *= (-1);
    }

    retStatus = SN_SYS_TIMER_Create(&timerPrint, liftDistance / ((DEFAULT_FEEDRATE) / (60 * 1000)), sTMR_Z_Busy_Callback);
    SN_SYS_ERROR_StatusCheck(retStatus, "Timer Cretae Failed.");

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

#if(PRINTER_TX_DEBUG)
        printf("3D Printer => Module => GCODE TX : %s\n", command); fflush(stdout);
#endif


    pthread_mutex_lock(&ptm3DPrinter);
    SN_SYS_SERIAL_Tx(serialId3DPrinter, command, bufferSize);
    pthread_mutex_unlock(&ptm3DPrinter);

    return retStatus;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Console
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static void sClrscr(void)
{
    system("clear");
}

static void sPrintProgressPrinting(uint32_t progressed, uint32_t goal, const printOption_t printOption, const printTarget_t printTarget)
{
    const char prefix[] = "Progress: [";
    const char suffix[] = "]";
    const size_t prefix_length = sizeof(prefix) - 1;
    const size_t suffix_length = sizeof(suffix) - 1;

    uint32_t max = 100;
    float count = 0;


    if(progressed > goal)
    {
    	progressed = goal;
    }
    count = ((float)progressed / (float)goal) * max;

    char *buffer = calloc(max + prefix_length + suffix_length + 1, 1); // +1 for \0
    size_t i = 0;

    strcpy(buffer, prefix);
    for (i = 0; i < max; ++i)
    {
        buffer[prefix_length + i] = i < count ? '@' : ' ';
    }

    strcpy(&buffer[prefix_length + i], suffix);
    printf("\n\b%c[2K\r%s[%.2f%](%d/%d slice)\n", 27, buffer, count, progressed, goal);
    fflush(stdout);
    free(buffer);

    printf("\n\n\n[ Option : %s ]\n\n",printOption.name);
    printf("Bottom Layer Exposure Time : %ld\n",printOption.bottomLayerExposureTime);
    printf("Bottom Layer Number : %ld\n",printOption.bottomLayerNumber);
    printf("Bottom Layer FeedRate : %f\n",printOption.bottomLiftFeedRate);
    printf("Layer Exposure Time : %ld\n",printOption.layerExposureTime);
    printf("Layer Thickness: %f \n",printOption.layerThickness);
    printf("Lift Distance : %ld \n",printOption.liftDistance);
    printf("Lift TIme : %ld \n",printOption.liftTime);
    printf("Lift Feed Rate : %f \n",printOption.liftFeedRate);
    printf("Bright : %ld \n",printOption.bright);

    printf("\n\n[ Target : %s ]\n\n", printTarget.projectName);
    printf("Layer Thickness: %f \n",printTarget.layerThickness);
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  System
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage)
{
    return SN_SYS_MESSAGE_Q_Put(msgQId3DPrinter, evtId, evtMessage);
}

static void s3DPrinterEnterState(deviceState_t state)
{
    pthread_mutex_lock(&ptm3DPrinter);
    module3DPrinter.state = state;
    pthread_mutex_unlock(&ptm3DPrinter);
}
