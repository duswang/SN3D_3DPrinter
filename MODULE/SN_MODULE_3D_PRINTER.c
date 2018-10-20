/*
 * SN_MODULE_3D_PRINTER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#include "SN_API.h"
#include "SN_MODULE_3D_PRINTER.h"

#include "SN_MODULE_IMAGE_VIEWER.h" /* THIS MODULE ONLY CALL ONE THREAD(MODULE) */

/******** STATIC DEFINE ********/
/**** SERIAL CONFIG ****/
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

/**** PRINT CONFIG ****/
#define FIRST_SLICE_DELAY_TIME   5000 //ms  (  5 sec )
#define STOP_DEVICE_DELAY_TIME  50000 //ms  ( 20 sec )

/**** MODULE ****/
typedef enum {
    DEVICE_STANDBY,
    DEVICE_INIT,
    DEVICE_PRINTING,
    DEVICE_PAUSE,
    DEVICE_RESUME,
    DEVICE_STOP,
    DEVICE_FINISH,
    DEVICE_BUSY,
    DEVICE_HOMING,
    DEVICE_NONE
} deviceState_t;

typedef struct moduel_3d_printer {
    deviceState_t       state;

    /** Print Info - Need Init & Uninit **/
    machineInfo_t machineInfo;
    printInfo_t     printInfo;
    uint32_t       sliceIndex;

    /** Gcode - Need Init & Uninit **/
    char gcodeLiftUp[30];
    char gcodeLiftDown[30];
} module3DPrinter_t;


#define INPUT_CHAIN_CONDITION       3   //DEMO
/******** SYSTEM DEFINE ********/
/**** MODULE THREAD ****/
static pthread_mutex_t ptm3DPrinter = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       pt3DPrinter;

/**** MODULE SERIAL ****/
sysSerialDef(3DPrinterSerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialId3DPrinter;

/**** MODULE TIMER ****/
static sysTimerId_t timerPrint;

/**** MODULE MESSAGE Q ****/
static sysMessageQId msgQId3DPrinter;
    /**** MODULE MESSAGES ****/
typedef enum {
    MSG_3D_PRINTER_PRINTING_INIT = 0,
    MSG_3D_PRINTER_PRINTING_CYCLE,
    MSG_3D_PRINTER_PRINTING_Z_LIFT,
    MSG_3D_PRINTER_PRINTING_PAUSE,
    MSG_3D_PRINTER_PRINTING_RESUME,
    MSG_3D_PRINTER_PRINTING_STOP,
    MSG_3D_PRINTER_PRINTING_FINISH,
    MSG_3D_PRINTER_HOMMING,
    MSG_3D_PRINTER_STANDBY,
    MSG_3D_PRINTER_NONE,
    MSG_3D_PRINTER_IGNORE         = 0xFF01
} evt3DPrinter_t;

/**** MODULE HANDLER  ****/
static module3DPrinter_t module3DPrinter;

/******** GLOBAL VARIABLE ********/
static int inputChain;

/******** STATIC FUNCTIONS ********/
/**** TIMER ****/
static void sTMR_NextCycle_Callback(void);
static void sTMR_Lift_Callback(void);
static void sTMR_StopDevice_Callback(void);

/**** SERIAL ****/
static void* sSerialRx_Callback(char* rxBuffer);
static SN_STATUS sSendGCode(char* command, size_t bufferSize);

/**** SYSTEM ****/
static void*     s3DPrinterThread();
static SN_STATUS s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage);
static void      s3DPrinterEnterState(deviceState_t state);

/**** BASIC BOARD CONTROL ****/
static SN_STATUS s3DPrinter_Homming(void);
static SN_STATUS s3DPrinter_MotorInit(void);
static SN_STATUS s3DPrinter_MotorUninit(void);
static SN_STATUS s3DPrinter_StopDevice(void);

/**** PRINTING ****/
static SN_STATUS s3DPrinter_PrintInit(void);
static SN_STATUS s3DPrinter_PrintPause(void);
static SN_STATUS s3DPrinter_PrintResume(void);
static SN_STATUS s3DPrinter_PrintStop(void);
static SN_STATUS s3DPrinter_PrintCycle(void);
static SN_STATUS s3DPrinter_PrintLift(void);

/**** UTIL ****/
static SN_STATUS sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp);


SN_STATUS SN_MODULE_3D_PRINTER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("MODULE INIT => 3D PRINTER");

    /** MESSAGE INIT **/
    retStatus = SN_SYS_MessageQInit(&msgQId3DPrinter);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Module Message Q Init Failed.");

    /** SERIAL INIT **/
    serialId3DPrinter = SN_SYS_SerialCreate(sysSerial(3DPrinterSerial), sSerialRx_Callback);
    if(serialId3DPrinter == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Serial Init Failed.");
    }

    /** MUTEX INIT **/
    if(pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Mutex Init Failed.");
    }

    /** THREAD INIT **/
    if(pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "3D Printer Thread Init Failed.");
    }

    /** MODULE INIT **/
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

SN_STATUS SN_MODULE_3D_PRINTER_MOTOR_INIT(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinter_MotorInit();
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_HOMING(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);
    SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Send Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_UP(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        retStatus = sGcodeParser_ZMove(module3DPrinter.gcodeLiftUp,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                true);

        /** Z INFO SET **/
        retStatus = sSendGCode(GCODE_CLEAR_BUFFER, sizeof(GCODE_CLEAR_BUFFER));

        /** Z UP **/
        retStatus = sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO Z CONTROL.");
    }
    else
    {
        //@DEMO
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

SN_STATUS SN_MODULE_3D_PRINTER_Z_DOWN(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        /** Z DOWN **/
        retStatus = sGcodeParser_ZMove(module3DPrinter.gcodeLiftDown,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                false);

        /** Z INFO SET **/
        retStatus = sSendGCode(GCODE_CLEAR_BUFFER, sizeof(GCODE_CLEAR_BUFFER));


        retStatus = sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
    }
    else if(module3DPrinter.state == DEVICE_HOMING)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO Z CONTROL.");
    }
    else
    {
        //@DEMO
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

    /** PrintInfo Init **/
    retStatus = SN_MODULE_FILE_SYSTEM_PrintInfoInit(pageIndex, itemIndex);
    SN_SYS_ERROR_CHECK(retStatus, "Get Print Info Failed.");

    /** Start Hardware Sequence **/
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

static void* s3DPrinterThread()
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    /** IMAGE VIEWER INIT - IT NEED MACHINE INFO FROM "FILE SYSTEM" FOR INIT **/
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
            retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PRINTING_FINISH);
            SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
            break;
        case MSG_3D_PRINTER_HOMMING:
            retStatus = s3DPrinter_Homming();
            break;
        case MSG_3D_PRINTER_STANDBY:
            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case MSG_3D_PRINTER_IGNORE:
            break;
        default:
            SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "3D Printer Get Unknown Message.");
            break;
        }
        SN_SYS_ERROR_CHECK(retStatus, "3D Printer Module Get Error.");
    }

  return NULL;
}

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

static void sTMR_StopDevice_Callback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* GET CURRENT POSITION */
    retStatus = sSendGCode(GCODE_GET_CURRENT_POSITION, sizeof(GCODE_GET_CURRENT_POSITION));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    SN_SYS_Log("Module => 3D Printer  => MOTOR UNINIT.");

    /* MOTOR DISABLE */
    retStatus = s3DPrinter_MotorUninit();
    SN_SYS_ERROR_CHECK(retStatus, "Motor Uninit Failed.");

    SN_SYS_Log("Module => 3D Printer  => DEVICE STOP DONE.");

    retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_DEVICE_STOP_DONE);
    SN_SYS_ERROR_CHECK(retStatus,"App Message Send Failed.");
}

static SN_STATUS s3DPrinter_StopDevice(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char stopGCode[30];

    sGcodeParser_ZMove(stopGCode,     \
            (float) module3DPrinter.machineInfo.height, \
            (float)0, \
            (float)DEFAULT_FEEDRATE,
            true);

    /* Z Position Init */
    retStatus = sSendGCode(GCODE_INIT_POSITION_ABSOLUTE, sizeof(GCODE_INIT_POSITION_ABSOLUTE));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    retStatus = sSendGCode(stopGCode, sizeof(stopGCode));
    SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

    retStatus = SN_SYS_TimerCreate(&timerPrint, STOP_DEVICE_DELAY_TIME, sTMR_StopDevice_Callback);
    SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");

    return retStatus;
}

static SN_STATUS s3DPrinter_PrintInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char* liftGcode = NULL;

    if(module3DPrinter.state != DEVICE_BUSY || module3DPrinter.state != DEVICE_INIT)
    {
        s3DPrinterEnterState(DEVICE_INIT);

        /* Get Print Info */
        module3DPrinter.printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
        if(!module3DPrinter.printInfo.isInit)
        {
            return SN_STATUS_NOT_INITIALIZED;
        }

        /** GET Mahcine Info **/
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

        /** PRINTING DISPLAY UPDATE **/
        SN_MODULE_DISPLAY_PrintingInfoInit(module3DPrinter.printInfo.printTarget.tempFileName, "Option 1");

        /** Motor Init **/
        s3DPrinter_MotorInit();
        SN_MODULE_IMAGE_VIEWER_CLEAR();

        printf("Module => 3D Printer  => TARGET NAME [ %s ]\n", module3DPrinter.printInfo.printTarget.tempFileName);

        retStatus = SN_SYS_TimerCreate(&timerPrint, FIRST_SLICE_DELAY_TIME, sTMR_NextCycle_Callback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT INIT.");
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

static SN_STATUS s3DPrinter_PrintCycle(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint32_t exposureTime = 0;

    /** One Cylce Start **/
    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        s3DPrinterEnterState(DEVICE_PRINTING);

        SN_MODULE_DISPLAY_PrintingInfoUpdate((module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice);
        printf("Module => 3D Printer  => SLICE %04d  =========", (module3DPrinter.sliceIndex + 1)); fflush(stdout);

        /** Slice Sequence **/
        /* Update Image Viewer */
        retStatus = SN_MODULE_IMAGE_VIEWER_UPDATE(module3DPrinter.sliceIndex);
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Update Failed.");

        /* UV Turn On */
        retStatus = SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_ON, sizeof(GCODE_LCD_ON));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /* Exposure */
        //if(module3DPrinter.sliceIndex < module3DPrinter.printInfo.printParameter.bottomLayerNumber)
        if(module3DPrinter.sliceIndex < 5)
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
        SN_SYS_Log("Module => 3D Printer  => TO PAUSE.");

        retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PAUSE_DONE);
        SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
    }
    else if(module3DPrinter.state == DEVICE_STOP)
    {
        SN_SYS_Log("Module => 3D Printer  => TO STOP.");
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
        /** Lift Sequence **/
        /** UV OFF **/
        retStatus = SN_SYS_SerialTx(serialId3DPrinter,GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /** IMAGE VIEWER CLEAER **/
        retStatus = SN_MODULE_IMAGE_VIEWER_CLEAR();
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Clear Failed.");

        /** Z LIFT **/
        retStatus = sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");
        retStatus = sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /** One Cycle Done **/
        if((module3DPrinter.sliceIndex + 1) >= module3DPrinter.printInfo.printTarget.slice)
        {
            printf("=========> FINISH. [ %4d/ %4d ]\n", (module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice); fflush(stdout);

            /** Finish Sequence **/
            retStatus = s3DPrinter_PrintStop();
            SN_SYS_ERROR_CHECK(retStatus, "Finish Sequence Failed.");

            /* Printing Finish */
            s3DPrinterEnterState(DEVICE_FINISH);
            retStatus = s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FINISH, 0);
            SN_SYS_ERROR_CHECK(retStatus, "3D Printer Message Failed.");
        }
        else
        {
            printf("=========> DONE. [ %4d/ %4d ]\n", (module3DPrinter.sliceIndex + 1), module3DPrinter.printInfo.printTarget.slice); fflush(stdout);

            /* Slice Index Update */
            module3DPrinter.sliceIndex++;

            /* Next Cycle */
            retStatus = SN_SYS_TimerCreate(&timerPrint, module3DPrinter.printInfo.printParameter.liftTime, sTMR_NextCycle_Callback);
            SN_SYS_ERROR_CHECK(retStatus, "Timer Create Failed.");
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
        /** Pause Sequence **/
        s3DPrinterEnterState(DEVICE_PAUSE);

        SN_SYS_Log("Module => 3D Printer  => PRINT PAUSE");
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
        /** Resume Sequence **/
        s3DPrinterEnterState(DEVICE_RESUME);

        SN_SYS_Log("Module => 3D Printer  => PRINT RESUME");

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
        /** Stop Sequence **/
        s3DPrinterEnterState(DEVICE_STOP);

        /* Cancle Print Timer */
        retStatus = SN_SYS_TimerCancle(&timerPrint);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Create Failed.");

        /* UV Off */
        retStatus = sSendGCode(GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
        SN_SYS_ERROR_CHECK(retStatus, "Send GCode Failed.");

        /* Image Viewer Clear */
        retStatus = SN_MODULE_IMAGE_VIEWER_CLEAR();
        SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Clear Failed.");

        /* Remove TempFile & Print Info Uninit */
        retStatus = SN_MODULE_FILE_SYSTEM_PrintInfoUninit();
        SN_SYS_ERROR_CHECK(retStatus, "Target Data Unint Failed.");

        /* Target Value Uninit */
        pthread_mutex_lock(&ptm3DPrinter);
        module3DPrinter.sliceIndex = 0;
        strcpy(module3DPrinter.gcodeLiftUp,   GCODE_LIFT_UNINIT);
        strcpy(module3DPrinter.gcodeLiftDown, GCODE_LIFT_UNINIT);
        pthread_mutex_unlock(&ptm3DPrinter);

        /** Stop Device **/
        retStatus = s3DPrinter_StopDevice();
        SN_SYS_ERROR_CHECK(retStatus, "Device Stop Failed");
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "BAD WAY ACCESS TO PRINT STOP.");
    }

    return retStatus;
}

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
