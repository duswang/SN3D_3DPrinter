/*
 * SN_MODUEL_3D_PRINTER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#include "SN_API.h"
#include "SN_MODUEL_3D_PRINTER.h"

/** System **/
/* Thread */
pthread_mutex_t ptm3DPrinter = PTHREAD_MUTEX_INITIALIZER;
pthread_t       pt3DPrinter;

/* Serial */
sysSerialDef(3DPrinterSerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE);
sysSerialId serialId3DPrinter;

/* Timer */
sysTimerId_t timerId3DPrinter;

/* Message Q */
sysMessageQId msgQId3DPrinter;

/** Moduel Controller **/
static module3DPrinter_t module3DPrinter;

/** Global Variable **/

/** Static Functions **/
/* callback */
static void* s3DPrinter_callback();

/* system */
static void  s3DPrinterMessagePut(evt3DPrinter_t evtId, event_msg_t evtMessage);
static void* s3DPrinterThread();

/* timer callback */
static void s3DPrinter_HW_Slice_callback(void);
static void s3DPrinter_HW_Lift_callback(void);

/* local */
static void s3DPrinter_HW_Homming(void);

static void s3DPrinter_HW_Pause(void);
static void s3DPrinter_HW_Resume(void);
static void s3DPrinter_HW_Stop(void);

static void s3DPrinter_HW_Init(void);
static void s3DPrinter_HW_MotorInit(void);
static void s3DPrinter_HW_MotorUninit(void);

static void s3DPrinter_HW_Finish(void);

static void s3DPrinter_HW_FirstSlice(void);
static void s3DPrinter_HW_Slice(void);
static void s3DPrinter_HW_Lift(void);

/* Util */
static void sGcodeParser_ZMove(char* pGcode, float liftDistance, float layerThickness,float liftFeedRate, bool isUp);

SN_STATUS SN_MODUEL_3D_PRINTER_Z_HOMING(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinter_HW_MotorInit();
    s3DPrinter_HW_Homming();

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Z_UP(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        sGcodeParser_ZMove(module3DPrinter.gcodeLiftUp,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                true);

        s3DPrinter_HW_MotorInit();
        SN_SYS_SerialTx(serialId3DPrinter, module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
    }

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Z_DOWN(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        sGcodeParser_ZMove(module3DPrinter.gcodeLiftDown,     \
                (float)mm, \
                (float)0, \
                (float)DEFAULT_FEEDRATE,
                false);

        s3DPrinter_HW_MotorInit();
        SN_SYS_SerialTx(serialId3DPrinter, module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));
    }

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** PrintInfo Init **/
    SN_MODULE_FILE_SYSTEM_PrintInfoInit(pageIndex, itemIndex);


    /** Start Hardware Sequence **/
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_INIT, 0);

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Pause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_PAUSE, 0);

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Resume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_RESUME, 0);

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Stop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_STOP, 0);

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** MESSAGE Q INIT **/
    SN_SYS_MessageQInit(&msgQId3DPrinter);

    serialId3DPrinter = SN_SYS_SerialCreate(sysSerial(3DPrinterSerial),s3DPrinter_callback);

    if (pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
    {
        printf("\n mutex init failed\n"); fflush(stdout);
    }

    if((retStatus = pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retStatus); fflush(stdout);
    }
    else
    {
        printf("Start Serial Monitor %s\n", UART_DEVICE); fflush(stdout);
    }

    /** Module Init **/
    module3DPrinter.state = DEVICE_NONE;

    s3DPrinter_HW_MotorInit();

    s3DPrinterMessagePut(MSG_3D_PRINTER_STANDBY, 0);

    return retStatus;
}

SN_STATUS SN_MODUEL_3D_PRINTER_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

static void*  s3DPrinter_callback()
{
    char* buffer = SN_SYS_SerialRx(serialId3DPrinter);

    if(strstr(buffer, RESPONSE_OK) != NULL)
    {
        switch(module3DPrinter.state)
        {
            case DEVICE_INIT:
              s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FIRST_SLICE, 0);
              break;
            case DEVICE_HOMING:
              s3DPrinterMessagePut(MSG_3D_PRINTER_STANDBY, 0);
              break;
            default:
              s3DPrinterMessagePut(MSG_3D_PRINTER_GET_RESPONSE, 0);
              break;
        }
    }
    else
    {
        /** Unknown Command **/
    }

    return 0;
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
                printf("3D Printing Start.\n"); fflush(stdout);
                s3DPrinter_HW_Init();
                break;
            case MSG_3D_PRINTER_PRINTING_FIRST_SLICE:
                s3DPrinter_HW_FirstSlice();
                break;
            case MSG_3D_PRINTER_PRINTING_SLICE:
                s3DPrinter_HW_Slice();
                break;
            case MSG_3D_PRINTER_PRINTING_LIFT:
                s3DPrinter_HW_Lift();
                break;
            case MSG_3D_PRINTER_PRINTING_PAUSE:
                s3DPrinter_HW_Pause();
                //SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, 0);
                break;
            case MSG_3D_PRINTER_PRINTING_RESUME:
                s3DPrinter_HW_Resume();
                break;
            case MSG_3D_PRINTER_PRINTING_STOP:
                s3DPrinter_HW_Stop();
                //SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, 0);
                break;
            case MSG_3D_PRINTER_PRINTING_FINISH:
                printf("PRINTING FINISH-!\n");
                s3DPrinter_HW_Finish();
                SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_PRINTING_FINISH);
                break;
            case MSG_3D_PRINTER_HOMMING:
                s3DPrinter_HW_Homming();
                break;
            case MSG_3D_PRINTER_SEND_COMMAND:
                break;
            case MSG_3D_PRINTER_GET_RESPONSE:
              printf("Response\n"); fflush(stdout);
              //SN_SYSTEM_SendAppMessage(APP_EVT_ID_3D_PRINTER, APP_EVT_MSG_3D_PRINTER_REPONSE_OK);
                break;
            case MSG_3D_PRINTER_STANDBY:
                module3DPrinter.state = DEVICE_STANDBY;
                break;
            default:
                break;
        }
    }

  return 0;
}

static void s3DPrinter_HW_Homming(void)
{
    if((module3DPrinter.state != DEVICE_BUSY) && (module3DPrinter.state != DEVICE_HOMING))
    {
        if(module3DPrinter.state != DEVICE_INIT)
        {
            module3DPrinter.state = DEVICE_HOMING;
            printf("BASIC HOMING START.\n"); fflush(stdout);
        }
        else
        {
            module3DPrinter.state = DEVICE_INIT;
            printf("HOMING START.\n"); fflush(stdout);
        }

        SN_SYS_SerialTx(serialId3DPrinter, GCODE_HOMING, sizeof(GCODE_HOMING));
    }
}

static void s3DPrinter_HW_Slice_callback(void)
{
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_LIFT, 0);
}

static void s3DPrinter_HW_Lift_callback(void)
{
    s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_SLICE, 0);
}

static void s3DPrinter_HW_Init(void)
{
    module3DPrinter.printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();

    char* liftGcode = NULL;

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

    if(module3DPrinter.state != DEVICE_BUSY || module3DPrinter.state != DEVICE_INIT)
    {
        module3DPrinter.state = DEVICE_INIT;

        SN_MODUEL_IMAGE_VIEWER_CLEAR();

        s3DPrinterMessagePut(MSG_3D_PRINTER_HOMMING, 0);

        /** NEED FIX **/
        s3DPrinter_HW_FirstSlice();
    }
}

static void s3DPrinter_HW_MotorInit(void)
{
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_INIT_SET_MM, sizeof(GCODE_INIT_SET_MM));
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_INTT_POSITION, sizeof(GCODE_INTT_POSITION));
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_INIT_MOTOR, sizeof(GCODE_INIT_MOTOR));
}

static void s3DPrinter_HW_MotorUninit(void)
{
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_UNINIT_POSITION, sizeof(GCODE_UNINIT_POSITION));
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_UNINIT_MOTOR, sizeof(GCODE_UNINIT_MOTOR));
}

static void s3DPrinter_HW_Finish(void)
{
    /** Motor Uninit **/
    s3DPrinter_HW_MotorUninit();

    SN_MODUEL_IMAGE_VIEWER_CLEAR();

    SN_MODULE_FILE_SYSTEM_PrintInfoUninit();

    module3DPrinter.sliceIndex = 0;
    strcpy(module3DPrinter.gcodeLiftUp,   GCODE_LIFT_UNINIT);
    strcpy(module3DPrinter.gcodeLiftDown, GCODE_LIFT_UNINIT);

    /** TEST **/
    SDL_Delay(5000);
}

static void s3DPrinter_HW_FirstSlice(void)
{
    /** Motor Init **/
    s3DPrinter_HW_MotorInit();

    SN_SYS_TimerCreate(&timerId3DPrinter, FIRST_SLICE_DELAY_TIME, s3DPrinter_HW_Lift_callback);
}

static void s3DPrinter_HW_Slice(void)
{
    uint32_t exposureTime = 0;

    /** One Cylce Start **/
    if((module3DPrinter.state != DEVICE_PAUSE) && (module3DPrinter.state != DEVICE_STOP))
    {
        module3DPrinter.state = DEVICE_BUSY;

        /** Slice **/
        SN_MODUEL_IMAGE_VIEWER_UPDATE(module3DPrinter.sliceIndex);

        SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_ON, sizeof(GCODE_LCD_ON));

        if(module3DPrinter.sliceIndex < module3DPrinter.printInfo.printParameter.bottomLayerNumber)
        {
            exposureTime = module3DPrinter.printInfo.printParameter.bottomLayerExposureTime;
        }
        else
        {
            exposureTime = module3DPrinter.printInfo.printParameter.layerExposureTime;
        }

        SN_SYS_TimerCreate(&timerId3DPrinter, exposureTime, s3DPrinter_HW_Slice_callback);
    }
}

static void s3DPrinter_HW_Lift(void)
{
    /** Lift Sequence **/
    SN_SYS_SerialTx(serialId3DPrinter, GCODE_LCD_OFF, sizeof(GCODE_LCD_OFF));
    SN_MODUEL_IMAGE_VIEWER_CLEAR();

    //s3DPrinter_HW_MotorInit(); /** NEED TEST **/
    SN_SYS_SerialTx(serialId3DPrinter, module3DPrinter.gcodeLiftUp, sizeof(module3DPrinter.gcodeLiftUp));
    SN_SYS_SerialTx(serialId3DPrinter, module3DPrinter.gcodeLiftDown, sizeof(module3DPrinter.gcodeLiftDown));

    /** One Cycle End **/
    /* Slice Index Update */
    module3DPrinter.sliceIndex++;
    //module3DPrinter.sliceIndex += 50;

    if(module3DPrinter.sliceIndex > module3DPrinter.printInfo.printTarget.slice)
    {
        /* Printing Finish */
        module3DPrinter.state = DEVICE_FINISH;
        s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_FINISH, 0);
    }
    else
    {
        /* Next Cycle */
        SN_SYS_TimerCreate(&timerId3DPrinter, module3DPrinter.printInfo.printParameter.liftTime, s3DPrinter_HW_Lift_callback);
    }
}

static void s3DPrinter_HW_Pause(void)
{
    if(module3DPrinter.state == DEVICE_BUSY)
    {
        module3DPrinter.state = DEVICE_PAUSE;
    }
}

static void s3DPrinter_HW_Resume(void)
{
    if(module3DPrinter.state == DEVICE_PAUSE)
    {
        module3DPrinter.state = DEVICE_RESUME;
        s3DPrinterMessagePut(MSG_3D_PRINTER_PRINTING_SLICE, 0);
    }
}

static void s3DPrinter_HW_Stop(void)
{
    if((module3DPrinter.state == DEVICE_BUSY) || (module3DPrinter.state == DEVICE_PAUSE))
    {
        module3DPrinter.state = DEVICE_STOP;

        /** Stop Device **/
        s3DPrinter_HW_Finish();
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

