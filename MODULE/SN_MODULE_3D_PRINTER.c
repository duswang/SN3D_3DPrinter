/*
 * SN_MODULE_3D_PRINTER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#include "SN_API.h"
#include "SN_MODULE_3D_PRINTER.h"

/** System **/
/* Thread */
static pthread_mutex_t ptm3DPrinter = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       pt3DPrinter;

/* Serial */
sysSerialDef(3DPrinterSerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialId3DPrinter;

/* Timer */
static sysTimerId_t timerPrint;

/* Message Q */
static sysMessageQId msgQId3DPrinter;

/** Moduel Controller **/
static module3DPrinter_t module3DPrinter;

/** Global Variable **/

/** Static Functions **/
/* serial callback */
static void* s3DPrinter_callback(char* rxBuffer);
/* TMR callback */
static void sTMR_Callback_NextCycle(void);
static void sTMR_Callback_Lift(void);
static void sTMR_Callback_StopDevice(void);

/* system */
static void  s3DPrinterEnterState(deviceState_t state);
static void  s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage);
static void* s3DPrinterThread();

/* local */
// ramps board.
static void s3DPrinter_Homming(void);
static void s3DPrinter_MotorInit(void);
static void s3DPrinter_MotorUninit(void);
static void s3DPrinter_StopDevice(void);

// for control
static void s3DPrinter_PrintInit(void);
static void s3DPrinter_PrintPause(void);
static void s3DPrinter_PrintResume(void);
static void s3DPrinter_PrintStop(void);
static void s3DPrinter_PrintCycle(void);
static void s3DPrinter_PrintLift(void);

/* Util */
// for gcode
static void sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp);
// for serial
static SN_STATUS sSendGCode(char* command, size_t bufferSize);



SN_STATUS SN_MODULE_3D_PRINTER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** MESSAGE Q INIT **/
    SN_SYS_MessageQInit(&msgQId3DPrinter);

    serialId3DPrinter = SN_SYS_SerialCreate(sysSerial(3DPrinterSerial),s3DPrinter_callback);

    if(pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
    {
        printf("\n mutex init failed\n"); fflush(stdout);
    }

    if((retStatus = pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retStatus); fflush(stdout);
    }

    SDL_Delay(3000);

    /** Module Init **/
    s3DPrinterEnterState(DEVICE_STANDBY);

    s3DPrinterMessagePut(MSG_3D_PRINTER_STANDBY, 0);

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

static void* s3DPrinter_callback(char* rxBuffer)
{
    if(strstr(rxBuffer, RESPONSE_OK) != NULL)
    {
        switch(module3DPrinter.state)
        {
        case DEVICE_HOMING:
            printf("HOMING DONE.\n"); fflush(stdout);
            SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_HOMING_DONE);
            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case DEVICE_BUSY:
            printf("TASK DONE.\n"); fflush(stdout);
            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        case DEVICE_PRINTING:
        case DEVICE_PAUSE:
        case DEVICE_RESUME:
            break;
        default:
              printf("RESPONSE OK.\n"); fflush(stdout);
              break;
        }
    }
    else if(strstr(rxBuffer, INIT_OK) != NULL)
    {
        printf("RAMPS BOARD INIT.\n"); fflush(stdout);
        SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE);
    }
    else
    {
        //SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE);
        printf("\n%s", rxBuffer); fflush(stdout);
    }

    return 0;
}

SN_STATUS SN_MODULE_3D_PRINTER_MOTOR_INIT(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinter_MotorInit();

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_HOMING(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_UP(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        sGcodeParser_ZMove(module3DPrinter.gcodeLiftUp,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                true);

        sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
    }
    else
    {
        printf("Z UP ERROR-!!\n");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Z_DOWN(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_BUSY);

        sGcodeParser_ZMove(module3DPrinter.gcodeLiftDown,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                false);
        sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
    }
    else
    {
        printf("Z DOWN ERROR-!!\n");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** PrintInfo Init **/
    SN_MODULE_FILE_SYSTEM_PrintInfoInit(pageIndex, itemIndex);

    /** Start Hardware Sequence **/
    s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_INIT, 0);
    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Pause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_PAUSE, 0);

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Resume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_RESUME, 0);

    return retStatus;
}

SN_STATUS SN_MODULE_3D_PRINTER_Stop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_STOP, 0);

    return retStatus;
}

static void* s3DPrinterThread()
{
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQId3DPrinter);

        switch(evt.evt_id)
        {
        case MSG_3D_PRINTER_PRINTING_INIT:
            s3DPrinter_PrintInit();
            break;
        case MSG_3D_PRINTER_PRINTING_CYCLE:
            s3DPrinter_PrintCycle();
            break;
	case MSG_3D_PRINTER_PRINTING_Z_LIFT:
	    printf("============="); fflush(stdout);
	    s3DPrinter_PrintLift();
	    break;
        case MSG_3D_PRINTER_PRINTING_PAUSE:
            s3DPrinter_PrintPause();
            break;
        case MSG_3D_PRINTER_PRINTING_RESUME:
            s3DPrinter_PrintResume();
            break;
        case MSG_3D_PRINTER_PRINTING_STOP:
            s3DPrinter_PrintStop();
            break;
        case MSG_3D_PRINTER_PRINTING_FINISH:
            printf("PRINTING FINISH.\n");
            SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PRINTING_FINISH);
            break;
        case MSG_3D_PRINTER_HOMMING:
            s3DPrinter_Homming();
            break;
        case MSG_3D_PRINTER_STANDBY:
            s3DPrinterEnterState(DEVICE_STANDBY);
            break;
        default:
            break;
        }
    }

  return 0;
}

static void s3DPrinter_Homming(void)
{
    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        s3DPrinterEnterState(DEVICE_HOMING);

        printf("HOMING START.\n"); fflush(stdout);
        sSendGCode(GCODE_HOMING, sizeof(GCODE_HOMING));

        while(module3DPrinter.state == DEVICE_HOMING); /** NEED MORE TEST **/
    }
    else
    {
        printf("PRINT HOMMING ERROR-!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_PrintInit(void)
{
    char* liftGcode = NULL;

    if(module3DPrinter.state != DEVICE_BUSY || module3DPrinter.state != DEVICE_INIT)
    {

        s3DPrinterEnterState(DEVICE_INIT);

        module3DPrinter.printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();

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

        /** Motor Init **/
        s3DPrinter_MotorInit();

        SN_MODULE_IMAGE_VIEWER_CLEAR();

        printf("PRINT START : %s\n", module3DPrinter.printInfo.printTarget.tempFileName); fflush(stdout);

        SN_SYS_TimerCreate(&timerPrint, FIRST_SLICE_DELAY_TIME, sTMR_Callback_NextCycle);
    }
    else
    {
        printf("PRINT INIT ERROR-!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_MotorInit(void)
{
    printf("MOTOR INIT.\n"); fflush(stdout);
    sSendGCode(GCODE_INIT_SET_MM, sizeof(GCODE_INIT_SET_MM));
    sSendGCode(GCODE_INTT_POSITION, sizeof(GCODE_INTT_POSITION));
    sSendGCode(GCODE_INIT_MOTOR, sizeof(GCODE_INIT_MOTOR));
}

static void s3DPrinter_MotorUninit(void)
{
    printf("MOTOR UNINIT.\n"); fflush(stdout);
    sSendGCode(GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
    sSendGCode(GCODE_UNINIT_POSITION, sizeof(GCODE_UNINIT_POSITION));
    sSendGCode(GCODE_UNINIT_MOTOR, sizeof(GCODE_UNINIT_POSITION));
}

static void s3DPrinter_StopDevice(void)
{
    printf("DEVICE STOP.\n"); fflush(stdout);
    sSendGCode(GCODE_DEVICE_STOP, sizeof(GCODE_DEVICE_STOP));

    SN_SYS_TimerCreate(&timerPrint, 10000, sTMR_Callback_StopDevice);
}

static void sTMR_Callback_NextCycle(void)
{
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE,0);
}

static void sTMR_Callback_Lift(void)
{
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_Z_LIFT,0);
}

static void sTMR_Callback_StopDevice(void)
{
    /** Motor Uninit **/
    s3DPrinter_MotorUninit();

    SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_DEVICE_STOP_DONE);

}
static void s3DPrinter_PrintCycle(void)
{
    uint32_t exposureTime = 0;

    //printf("\n%s %d %s\n",__FILE__,  __LINE__, __func__); fflush(stdout);

    /** One Cylce Start **/
    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        s3DPrinterEnterState(DEVICE_PRINTING);

        printf("PRINT %04d CYCLE =====", module3DPrinter.sliceIndex); fflush(stdout);

        /** Slice **/
        SN_MODULE_IMAGE_VIEWER_UPDATE(module3DPrinter.sliceIndex);


        SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_ON, sizeof(GCODE_LCD_ON));

        if(module3DPrinter.sliceIndex < module3DPrinter.printInfo.printParameter.bottomLayerNumber)
        {
            exposureTime = module3DPrinter.printInfo.printParameter.bottomLayerExposureTime;
        }
        else
        {
            exposureTime = module3DPrinter.printInfo.printParameter.layerExposureTime;
        }

        SN_SYS_TimerCreate(&timerPrint , exposureTime, sTMR_Callback_Lift);
    }
    else if(module3DPrinter.state == DEVICE_PAUSE)
    {
        printf("PRINT CYCLE PAUSE NOW. CAN'T PRINT."); fflush(stdout);
        SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PAUSE_DONE);
    }
    else if(module3DPrinter.state == DEVICE_STOP)
    {
        printf("PRINT CYCLE STOP NOW. CAN'T PRINT."); fflush(stdout);
    }
    else
    {
        printf("PRINT CYCLE ERROR-!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_PrintLift(void)
{

    //printf("\n%s %d %s\n",__FILE__,  __LINE__, __func__); fflush(stdout);

    if((module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE))
    {
        /** Lift Sequence **/
        SN_SYS_SerialTx(serialId3DPrinter,GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
        SN_MODULE_IMAGE_VIEWER_CLEAR();

        //s3DPrinter_HW_MotorInit(); /** NEED TEST **/
        sSendGCode(module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
        sSendGCode(module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));

        /** One Cycle End **/
        /* Slice Index Update */
        module3DPrinter.sliceIndex++;
        //module3DPrinter.sliceIndex += 50;

        printf("====> DONE.\n"); fflush(stdout);

        if(module3DPrinter.sliceIndex > module3DPrinter.printInfo.printTarget.slice)
        {
            s3DPrinter_PrintStop();
            /* Printing Finish */
            s3DPrinterEnterState(DEVICE_FINISH);
            s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FINISH, 0);
        }
        else
        {
            /* Next Cycle */
            SN_SYS_TimerCreate(&timerPrint, module3DPrinter.printInfo.printParameter.liftTime, sTMR_Callback_NextCycle);
        }
    }
    else
    {
        printf("PRINT LIFT ERROR-!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_PrintPause(void)
{
    if(module3DPrinter.state == DEVICE_PRINTING || \
       module3DPrinter.state == DEVICE_INIT)
    {
        s3DPrinterEnterState(DEVICE_PAUSE);
        printf("\nPRINT PAUSE.\n"); fflush(stdout);
    }
    else
    {
        printf("PRINT PAUSE ONLY CALL WHEN PRINTING!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_PrintResume(void)
{
    if(module3DPrinter.state == DEVICE_PAUSE)
    {
        printf("\nPRINT RESUME.\n"); fflush(stdout);
        s3DPrinterEnterState(DEVICE_RESUME);
        s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_CYCLE, 0);
    }
    else
    {
        printf("PRINT RESUME ONLY CALL WHEN PAUSE-!!\n"); fflush(stdout);
    }
}

static void s3DPrinter_PrintStop(void)
{
    if((module3DPrinter.state == DEVICE_INIT)     || \
       (module3DPrinter.state == DEVICE_PRINTING) || \
       (module3DPrinter.state == DEVICE_PAUSE)    || \
       (module3DPrinter.state == DEVICE_RESUME))
    {
        s3DPrinterEnterState(DEVICE_STOP);

        SN_SYS_TimerCancle(&timerPrint);

        SN_MODULE_IMAGE_VIEWER_CLEAR();

        SN_MODULE_FILE_SYSTEM_PrintInfoUninit();

        // Uninit Target Value
        pthread_mutex_lock(&ptm3DPrinter);
        module3DPrinter.sliceIndex = 0;
        strcpy(module3DPrinter.gcodeLiftUp,   GCODE_LIFT_UNINIT);
        strcpy(module3DPrinter.gcodeLiftDown, GCODE_LIFT_UNINIT);
        pthread_mutex_unlock(&ptm3DPrinter);

        /** Stop Device **/
        s3DPrinter_StopDevice();
    }
    else
    {
        printf("PRINT STOP ONLY CALL WHEN PRINTING-!!\n"); fflush(stdout);
    }
}

static void s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage)
{
    SN_SYS_MessagePut(&msgQId3DPrinter, evtId, evtMessage);
}


static void sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp)
{
    if(isUp)
    {
        sprintf(pGcode,"G1 X%s Z%.2f F%.2f", "1.0", liftDistance, liftFeedRate);
    }
    else
    {
        sprintf(pGcode,"G1 X-%s Z-%.2f F%.2f", "1.0", (liftDistance - layerThickness), liftFeedRate);
    }
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

static void s3DPrinterEnterState(deviceState_t state)
{
    pthread_mutex_lock(&ptm3DPrinter);
    module3DPrinter.state = state;
    pthread_mutex_unlock(&ptm3DPrinter);
}
