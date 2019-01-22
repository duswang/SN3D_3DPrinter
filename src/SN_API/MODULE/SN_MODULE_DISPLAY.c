/**
 * @file SN_MODULE_DISPLAY.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo Serial Communication Check and retry & timeout functions
 */

#include "SN_API.h"
#include "SN_MODULE_DISPLAY.h"

#ifdef __APPLE__

#endif

#ifdef linux
#include <iconv.h>
#endif

/* ******* STATIC DEFINE ******* */
/** @name Serial Config Define */ ///@{
#define BYTE_SIZE   SN_SYS_SERIAL_COMM_RX_REALTIME
#define BAUD_RATE   SN_SYS_SERIAL_COMM_BAUD_RATE_115200
#define RETURN_MODE SN_SYS_SERIAL_COMM_TX_NX_RETURN

#ifdef __APPLE__
#define UART_DEVICE "/dev/cu.usbmodem1421"
#endif
#ifdef linux
#define UART_DEVICE "/dev/serial0"
#endif

#define UART_OFLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK
///@}

/** @name Other Define */ ///@{
#define DEFAULT_BUFFER_SIZE             256
#define NEXTION_COMMAND_BUFFER_SIZE     DEFAULT_BUFFER_SIZE
#define TIMER_INDICATE_INTERVAL         1000 /**< 1 second */
#define NEXTION_INIT_PAGE_DELAY         1500 /**< 1.5 second */

///@}

typedef struct time_info {
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
} timeInfo_t;

/* *** MODULE *** */
typedef struct moduel_display {
    nx_page_t state;

    /** Timer Info **/
    timeInfo_t estimatedBuildTime;
    timeInfo_t nowTime;
    uint32_t   secNowTime;


    timeInfo_t totalTime;
    uint32_t   secTotalTime;

    bool       IsTimerInfoInit;
} moduleDisplay_t;

/* ******* SYSTEM DEFINE ******* */
/* *** MODULE THREAD *** */
static pthread_mutex_t ptmDisplay = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       ptDisplay;

/* *** MODULE SERIAL *** */
sysSerialDef(DisplaySerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialIdDisplay;

/* *** MODULE MESSAGE Q *** */
static sysMessageQId   msgQIdDisplay;
    /* *** MODULE MESSAGES *** */
typedef enum event_display_message {
    MSG_DISPLAY_EVENT_GET         = 0,      /**< Serial message get from nextion display */
    MSG_DISPLAY_UPDATE_IMAGE,               /**< Update Image on printing page */
    MSG_DISPLAY_TIME_INFO_TIMER_START,
    MSG_DISPLAY_TIME_INFO_TIMER_STOP,
    MSG_DISPLAY_TIME_INFO_TIMER_UPDATE,     /**< Time Info Update */
    MSG_DISPLAY_TOTAL_TIME_UPDATE,
    MSG_DISPLAY_NONE,                       /**< BAD ACCESS */
} evtDisplay_t;

/* *** MODULE TIMER *** */
static sysTimerId_t timerTimeIndicate;
static sysTimerId_t timerTotalTime;

/* *** MODULE HANDLER  *** */
static moduleDisplay_t moduleDisplay;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** SYSTEM *** */
static void* sDisplayThread();
static SN_STATUS sDisplayMessagePut(event_id_t evtDisplay_t, event_msg_t evtMessage);
static void      sDisplayEnterState(nx_page_t state);

/* *** TIMER *** */
static void sTMR_Timer_UpdateCallback(void);
static void sTMR_TotalTime_UpdateCallback(void);

/* *** SERIAL *** */
static void* sSerialRx_Callback(char *rxBuffer);
static SN_STATUS sSendCommand(char* command, size_t bufferSize);

/* *** NEXTION *** */
static SN_STATUS sDisplay_NextionInit(void);

/* *** TIME INFO *** */
static SN_STATUS sDisplay_TimerInfoUpdate(void);
static SN_STATUS sDisplay_TotalTimeUpdate(void);

/* *** UTIL *** */
static timeInfo_t sSecToTimeInfo(uint32_t sec);
static char* UTF8toEUC_KR(char* utf8String);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_MODULE_DISPLAY_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("MODULE INIT => NEXTION DISPLAY.");

    /* MESSAGE Q INIT */
    retStatus = SN_SYS_MessageQInit(&msgQIdDisplay);
    SN_SYS_ERROR_CHECK(retStatus, "Display Module Message Q Init Failed.");

    /* SERIAL INIT */
    serialIdDisplay = SN_SYS_SerialCreate(sysSerial(DisplaySerial), sSerialRx_Callback);
    if(serialIdDisplay == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Display Serial Init Failed.");
    }

    /* MUTEX INIT */
    if (pthread_mutex_init(&ptmDisplay, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Display Mutex Init Failed.");
    }

    /* THREAD INIT */
    if((retStatus = pthread_create(&ptDisplay, NULL, sDisplayThread, NULL)))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Display Thread Init Failed.");
    }

    /* Nextion Display LCD Reset */
    retStatus = sSendCommand(NX_COMMAND_RESET, sizeof(NX_COMMAND_RESET));
    SN_SYS_Delay(NEXTION_INIT_PAGE_DELAY);

    SN_MODULE_DISPLAY_EnterState(NX_PAGE_SETUP);

    SN_MODULE_DISPLAY_BootProgressUpdate(30, "Display Module Loading...");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    sDisplayEnterState(state);

    switch(state)
    {
    case NX_PAGE_WAITING:
        retStatus = sSendCommand(NX_PAGE_WAITING_COMMAND, sizeof(NX_PAGE_WAITING_COMMAND));
        break;
    case NX_PAGE_CONTROL:
        retStatus = sSendCommand(NX_PAGE_CONTROL_COMMAND, sizeof(NX_PAGE_CONTROL_COMMAND));
        break;
    case NX_PAGE_FILE_SELECT:
        retStatus = sSendCommand(NX_PAGE_FILE_SYSTEM_COMMAND, sizeof(NX_PAGE_FILE_SYSTEM_COMMAND));
        break;
    case NX_PAGE_PRINTING:
    case NX_PAGE_PAUSE:
        retStatus = sSendCommand(NX_PAGE_PRINTING_COMMAND, sizeof(NX_PAGE_PRINTING_COMMAND));
        break;
    case NX_PAGE_SETUP:
        retStatus = sSendCommand(NX_PAGE_BOOT_COMMAND, sizeof(NX_PAGE_BOOT_COMMAND));
        SN_SYS_Delay(NEXTION_INIT_PAGE_DELAY);
        break;
    case NX_PAGE_INIT:
        sDisplay_NextionInit();
        break;
    case NX_PAGE_INFO:
        retStatus = sSendCommand(NX_PAGE_INFO_COMMAND, sizeof(NX_PAGE_INFO_COMMAND));
        break;
    case NX_PAGE_LOADING:
        retStatus = sSendCommand(NX_PAGE_LOADING_COMMAND, sizeof(NX_PAGE_LOADING_COMMAND));
        break;
    case NX_PAGE_NOTIFY:
        retStatus = sSendCommand(NX_PAGE_NOTIFY_COMMAND, sizeof(NX_PAGE_NOTIFY_COMMAND));
        break;
    default:
        retStatus = SN_STATUS_INVALID_PARAM;
        break;
    }

    SN_SYS_ERROR_CHECK(retStatus, "ERROR IN DISPLAY PAGE UPDATE.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_BootProgressUpdate(uint32_t progressValue, const char* progressStr)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    if(progressStr == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    sprintf(buffer,"Boot.CurrentStatus.val=%d", progressValue);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    sprintf(buffer,"Boot.ProgressText.txt=\"%s\"", progressStr);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingInfoInit(const char* fileName, const char* optionName)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[NEXTION_COMMAND_BUFFER_SIZE];
    char* korBuffer = NULL;

    if(fileName == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if(optionName == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    sprintf(buffer,"Print.FileName.txt=\"%s\"", fileName);
    korBuffer = UTF8toEUC_KR(buffer);

    retStatus = sSendCommand(korBuffer, strlen(korBuffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
    free(korBuffer);

    sprintf(buffer,"Print.Option.txt=\"%s\"", optionName);
    korBuffer = UTF8toEUC_KR(buffer);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
    free(korBuffer);



    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[DEFAULT_BUFFER_SIZE];

    sprintf(buffer,"Print.Slice.txt=\"%3d / %3d\"", slice, targetSlice);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerInit(uint32_t sec)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    moduleDisplay.estimatedBuildTime = sSecToTimeInfo(sec);
    moduleDisplay.nowTime.hour       = 0;
    moduleDisplay.nowTime.min        = 0;
    moduleDisplay.nowTime.sec        = 0;

    moduleDisplay.IsTimerInfoInit = true;

    sprintf(buffer,"Print.ETA_Time.txt=\"> %02d:%02d:%02d\"", \
            moduleDisplay.estimatedBuildTime.hour, \
            moduleDisplay.estimatedBuildTime.min, \
            moduleDisplay.estimatedBuildTime.sec);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");


    retStatus = sDisplayMessagePut(MSG_DISPLAY_TIME_INFO_TIMER_UPDATE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerSync(uint32_t sec)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleDisplay.IsTimerInfoInit)
    {
        moduleDisplay.secNowTime = sec;
    }
    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerPause(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleDisplay.IsTimerInfoInit)
    {
        retStatus = SN_SYS_TimerCancle(&timerTimeIndicate);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cancle Failed.");
    }
    else
    {

    }

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerResume(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleDisplay.IsTimerInfoInit)
    {
        retStatus = sDisplayMessagePut(MSG_DISPLAY_TIME_INFO_TIMER_UPDATE, 0);
        SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");
    }
    else
    {

    }

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerStop(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleDisplay.IsTimerInfoInit)
    {
        moduleDisplay.estimatedBuildTime = sSecToTimeInfo(0);
        moduleDisplay.nowTime            = sSecToTimeInfo(0);
        moduleDisplay.secNowTime         = 0;
        moduleDisplay.IsTimerInfoInit    = false;

        retStatus = SN_SYS_TimerCancle(&timerTimeIndicate);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cancle Failed.");
    }

    return retStatus;
}


SN_STATUS SN_MODULE_DISPLAY_TotalTimeInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    moduleDisplay.totalTime.hour       = 0;
    moduleDisplay.totalTime.min        = 0;
    moduleDisplay.totalTime.sec        = 0;

    retStatus = sDisplayMessagePut(MSG_DISPLAY_TOTAL_TIME_UPDATE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_ControlZPosition(float mm)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    moduleDisplay.IsTimerInfoInit = true;

    sprintf(buffer,"Control.ZPosition.txt=\"%0.2f mm\"", mm);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display ZPosition Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_FileSelectOptionUpdate(uint32_t optionIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    const printOption_t* printOption = NULL;
    char buffer[NEXTION_COMMAND_BUFFER_SIZE];
    char* korBuffer = NULL;

    SN_MODULE_FILE_SYSTEM_OptionLoad(optionIndex);
    printOption = SN_MODULE_FILE_SYSTEM_OptionGet();

    if(SN_MODULE_FILE_SYSTEM_isOptionExist())
    {
        sprintf(buffer,"Option.txt=\"%s\"", printOption->name);
    }
    else
    {
        sprintf(buffer,"Option.txt=\" \"");
    }

    korBuffer = UTF8toEUC_KR(buffer);

    retStatus = sSendCommand(korBuffer, strlen(korBuffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
    free(korBuffer);

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_FileSelectPageUpdate(uint32_t pageIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char buffer[NEXTION_COMMAND_BUFFER_SIZE];
    char* korBuffer = NULL;
    uint32_t itemIndex = 0;

    const fsPage_t* currentPage      = NULL;

    currentPage = SN_MODULE_FILE_SYSTEM_GetFilePage(pageIndex);
    if(currentPage == NULL)
    {
        SN_SYS_ERROR_CHECK(retStatus, "page not initialzied.");
    }

    if(SN_MODULE_FILE_SYSTEM_isPrintFileExist())
    {
        /* Send to Nextion Display */
        if(pageIndex <= SN_MODULE_FILE_SYSTEM_GetFilePageCnt())
        {
            for(itemIndex = 0; itemIndex < MAX_ITEM_SIZE; itemIndex++)
            {
                if(itemIndex <= currentPage->itemCnt)
                {
                    sprintf(buffer,"Index_%d.txt=\"  %s\"", itemIndex, currentPage->item[itemIndex].name);
                }
                else
                {
                    sprintf(buffer,"Index_%d.txt=\" \"", itemIndex);
                }
                korBuffer = UTF8toEUC_KR(buffer);

                retStatus = sSendCommand(korBuffer, strlen(korBuffer) + 1);
                SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
                free(korBuffer);
            }
        }
    }
    else
    {
        for(itemIndex = 0; itemIndex < MAX_ITEM_SIZE; itemIndex++)
        {
                sprintf(buffer,"Index_%d.txt=\"\"", itemIndex);

                retStatus = sSendCommand(buffer, strlen(buffer) + 1);
                SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
        }
    }

    /* Item Count Send */
    sprintf(buffer,"Item_Cnt.val=%d", currentPage->itemCnt);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Nextion GUI Tool
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS SN_MODULE_DISPLAY_NextionDrawLine(int startX, int startY, int endX, int endY, int color)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    if(moduleDisplay.state != NX_PAGE_LOADING && \
       moduleDisplay.state != NX_PAGE_REQUEST)
    {
        sprintf(buffer,"line %d,%d,%d,%d,%d", startX, startY, endX, endY, color);
        retStatus = sSendCommand(buffer, strlen(buffer) + 1);
        SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Draw Vertical Line Failed.");
    }
    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_NextionDrawDot(int coorX, int coorY, int color)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    if(moduleDisplay.state != NX_PAGE_LOADING && \
       moduleDisplay.state != NX_PAGE_REQUEST)
    {
        sprintf(buffer,"draw %d,%d,%d,%d,%d", coorX, coorY, coorX, coorY, color);
        retStatus = sSendCommand(buffer, strlen(buffer) + 1);
        SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Draw Dot Failed.");
    }

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_NextionDrawFill(int startX, int startY, int width, int height, int color)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char buffer[NEXTION_COMMAND_BUFFER_SIZE];

    if(moduleDisplay.state != NX_PAGE_LOADING && \
       moduleDisplay.state != NX_PAGE_REQUEST)
    {
        sprintf(buffer,"fill %d,%d,%d,%d,%d", startX, startY, width, height, color);
        retStatus = sSendCommand(buffer, strlen(buffer) + 1);
        SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Draw Fill Failed.");
    }

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Module Thread
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static void* sDisplayThread()
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdDisplay);

        switch(evt.evt_id)
        {
            case MSG_DISPLAY_EVENT_GET:
                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_DISPLAY, evt.evt_msg);
                SN_SYS_ERROR_CHECK(retStatus, "APP Send Message Failed.");
                break;
            case MSG_DISPLAY_UPDATE_IMAGE:
                break;
            case MSG_DISPLAY_TIME_INFO_TIMER_UPDATE:
                sDisplay_TimerInfoUpdate();
                break;
            case MSG_DISPLAY_TOTAL_TIME_UPDATE:
                sDisplay_TotalTimeUpdate();
                break;
            default:
                SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "Display Get Unknown Message.");
                break;
        }
        SN_SYS_ERROR_CHECK(retStatus, "Display Module Get Error.");
    }

    return NULL;
}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Serial Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void* sSerialRx_Callback(char * rxBuffer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int i = 0;
    msgNX_t msgNXId;

    switch(rxBuffer[0])
    {
    case NX_COMMAND_SN_MESSAGE_HEAD:
#if(DISPLAY_RX_DEBUG)
        printf("Module => Display => Nextion RX : "); fflush(stdout);
        while(rxBuffer[i] != '\0')
        {
            if(i == 0)
            {
                msgNXId.command = rxBuffer[0];
            }
            else
            {
                msgNXId.NXmessage8bit[i - 1] = rxBuffer[i];
            }

            printf("0x%02x ", rxBuffer[i]);

            i++;
        }
        printf("\n"); fflush(stdout);
#else
        while(rxBuffer[i] != '\0')
        {
            if(i == 0)
            {
                msgNXId.command = rxBuffer[0];
            }
            else
            {
                msgNXId.NXmessage8bit[i - 1] = rxBuffer[i];
            }

            i++;
        }
#endif
        if(msgNXId.type == NX_TYPE_NOTIFY)
        {
            sDisplayEnterState(msgNXId.page);
        }

        retStatus = sDisplayMessagePut(MSG_DISPLAY_EVENT_GET, msgNXId.NXmessage[0]);
        SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");
        break;
    case NX_COMMAND_INVALID_CMD:
#if(DISPLAY_RX_DEBUG)
        SN_SYS_Log("Module => Display => Nextion RX : NEXTION INVALID COMMAND.");
    case NX_COMMAND_FINISHED:
        SN_SYS_Log("Module => Display => Nextion RX : NEXTION RESPONSE OK.");
        break;
    case NX_COMMAND_EVT_LAUNCHED:
        SN_SYS_Log("Module => Display => Nextion RX : NEXTION DISPLAY LAUNCHED.");
        break;
    case NX_COMMAND_INVALID_BAUD:
        SN_SYS_Log("Module => Display => Nextion RX : NEXTION BAUD RATE NOT MATCHING.");
        break;
    case NX_COMMAND_INVALID_VARIABLE:
        SN_SYS_Log("Module => Display => Nextion RX : NEXTION INVALID VARIABLE.");
        break;
    default:
        printf("Module => Display => Nextion RX : 0x%02x\n", rxBuffer[0]);
        break;
#else
    default:
        break;
#endif
    }

    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Timer Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sTMR_Timer_UpdateCallback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* INCREASE ONE SECOND */
    moduleDisplay.secNowTime++;

    retStatus = sDisplayMessagePut(MSG_DISPLAY_TIME_INFO_TIMER_UPDATE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "Time Info Timer Stop Failed.");
}

static void sTMR_TotalTime_UpdateCallback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* INCREASE ONE SECOND */
    moduleDisplay.secTotalTime++;

    retStatus = sDisplayMessagePut(MSG_DISPLAY_TOTAL_TIME_UPDATE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "Time Info Timer Stop Failed.");
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Time Info
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sDisplay_TimerInfoUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[DEFAULT_BUFFER_SIZE];

    if(moduleDisplay.IsTimerInfoInit)
    {
        moduleDisplay.nowTime = sSecToTimeInfo(moduleDisplay.secNowTime);

        sprintf(buffer,"Print.CurrentTime.txt=\"%02d:%02d:%02d\"", \
                moduleDisplay.nowTime.hour, \
                moduleDisplay.nowTime.min, \
                moduleDisplay.nowTime.sec);

        retStatus = sSendCommand(buffer, strlen(buffer) + 1);
        SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

        retStatus = SN_SYS_TimerCreate(&timerTimeIndicate, TIMER_INDICATE_INTERVAL, sTMR_Timer_UpdateCallback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_Log("BAD WAY ACCESS Timer Info Update.");
    }

    return retStatus;
}

static SN_STATUS sDisplay_TotalTimeUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[DEFAULT_BUFFER_SIZE];


    moduleDisplay.totalTime = sSecToTimeInfo(moduleDisplay.secTotalTime);

    sprintf(buffer,"Info.Total Time.txt=\"%02d:%02d:%02d\"", \
            moduleDisplay.totalTime.hour, \
            moduleDisplay.totalTime.min, \
            moduleDisplay.totalTime.sec);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

    retStatus = SN_SYS_TimerCreate(&timerTotalTime, TIMER_INDICATE_INTERVAL, sTMR_TotalTime_UpdateCallback);
    SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");

    return retStatus;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Nextion Display Init
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sDisplay_NextionInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    const machineInfo_t* machineInfo = NULL;
    const versionInfo_t* versionInfo = NULL;
    char buffer[DEFAULT_BUFFER_SIZE];

    int thumbnail_offset_x = 0;
    int thumbnail_offset_y = 0;
    int thumbnail_width    = 0;
    int thumbnail_height   = 0;

    SN_SYS_Delay(3);

    /* Get Mahcine Info */
    machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(machineInfo == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "machineInfo not initialized.");
    }

    versionInfo = SN_MODULE_FILE_SYSTEM_VersionInfoGet();
    if(versionInfo == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "machineInfo not initialized.");
    }

    switch(machineInfo->machineHeight)
    {
        case 200:
            thumbnail_offset_x = NEXTION_THUMBNAIL_3_2_OFFSET_X;
            thumbnail_offset_y = NEXTION_THUMBNAIL_3_2_OFFSET_Y;
            thumbnail_width    = NEXTION_THUMBNAIL_3_2_WIDTH;
            thumbnail_height   = NEXTION_THUMBNAIL_3_2_HEIGHT;
            break;

        case 250:
            thumbnail_offset_x = NEXTION_THUMBNAIL_4_3_OFFSET_X;
            thumbnail_offset_y = NEXTION_THUMBNAIL_4_3_OFFSET_Y;
            thumbnail_width    = NEXTION_THUMBNAIL_4_3_WIDTH;
            thumbnail_height   = NEXTION_THUMBNAIL_4_3_HEIGHT;
            break;

        case 400:
            thumbnail_offset_x = NEXTION_THUMBNAIL_5_0_OFFSET_X;
            thumbnail_offset_y = NEXTION_THUMBNAIL_5_0_OFFSET_Y;
            thumbnail_width    = NEXTION_THUMBNAIL_5_0_WIDTH;
            thumbnail_height   = NEXTION_THUMBNAIL_5_0_HEIGHT;
            break;

        case 500:
            thumbnail_offset_x = NEXTION_THUMBNAIL_7_0_OFFSET_X;
            thumbnail_offset_y = NEXTION_THUMBNAIL_7_0_OFFSET_Y;
            thumbnail_width    = NEXTION_THUMBNAIL_7_0_WIDTH;
            thumbnail_height   = NEXTION_THUMBNAIL_7_0_HEIGHT;
            break;

        default:
            thumbnail_offset_x = NEXTION_THUMBNAIL_3_2_OFFSET_X;
            thumbnail_offset_y = NEXTION_THUMBNAIL_3_2_OFFSET_Y;
            thumbnail_width    = NEXTION_THUMBNAIL_3_2_WIDTH;
            thumbnail_height   = NEXTION_THUMBNAIL_3_2_HEIGHT;
            break;
    }

    sprintf(buffer,"Boot.DeviceName.txt=\"%s\"", machineInfo->name);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");
    SN_SYS_Delay(3);

    sprintf(buffer,"Boot.thumbnail_W.val=%d", thumbnail_width);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");
    SN_SYS_Delay(3);

    sprintf(buffer,"Boot.thumbnail_H.val=%d", thumbnail_height);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");
    SN_SYS_Delay(3);

    sprintf(buffer,"Boot.thumbnail_X.val=%d", thumbnail_offset_x);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");
    SN_SYS_Delay(3);

    sprintf(buffer,"Boot.thumbnail_Y.val=%d", thumbnail_offset_y);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

    sprintf(buffer,"Info.Hash.txt=\"%ld\"", versionInfo->hash);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

    sprintf(buffer,"Info.Inch.txt=\"%.1f Inch\"", machineInfo->inch);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

    sprintf(buffer,"Info.Version.txt=\"%ld.%ld.%ldv\"", versionInfo->releaseNumber, versionInfo->majorNumber, versionInfo->minorNumber);
    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

    SN_SYS_Delay(3);

    SN_MODULE_DISPLAY_TotalTimeInit();

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Util
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static timeInfo_t sSecToTimeInfo(uint32_t sec)
{
    timeInfo_t temp;

    temp.min  = sec / 60;
    temp.hour = temp.min / 60;
    temp.sec  = sec % 60;
    temp.min  = temp.min % 60;

    return temp;
}

static char* UTF8toEUC_KR(char* utf8String)
{
    size_t in_size = strlen(utf8String);
    size_t out_size = sizeof(wchar_t) * in_size * 4;
    char* out_buf = malloc(out_size);
    if(out_buf == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "memory allocate failed");
    }

    memset(out_buf, 0x00, out_size);

    iconv_t ic = iconv_open("EUC-KR" /*tocode*/, "UTF-8" /*fromcode*/ );
    if(ic == (iconv_t)-1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "iconv init failed");
    }

    char* in_ptr = utf8String;
    char* out_ptr = out_buf;

    size_t out_buf_left = out_size;

    size_t result = iconv(ic, &in_ptr, &in_size, &out_ptr, &out_buf_left);
    if(result == -1)
    {
        //SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Convert to EUC-KR from UTF-8 failed");
    }

    iconv_close(ic);

    return out_buf;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  System
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sDisplayMessagePut(evtDisplay_t evtId, event_msg_t evtMessage)
{
    return SN_SYS_MessagePut(&msgQIdDisplay, evtId, evtMessage);
}



static SN_STATUS sSendCommand(char* command, size_t bufferSize)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if((serialIdDisplay == NULL))
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    if(command == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

#if(DISPLAY_TX_DEBUG)
        printf("Module => Display => Nextion TX : %s\n", command); fflush(stdout);
#endif

    pthread_mutex_lock(&ptmDisplay);
    retStatus = SN_SYS_SerialTx(serialIdDisplay, command, bufferSize);
    pthread_mutex_unlock(&ptmDisplay);

    return retStatus;
}

static void sDisplayEnterState(nx_page_t state)
{
    pthread_mutex_lock(&ptmDisplay);
    moduleDisplay.state = state;
    pthread_mutex_unlock(&ptmDisplay);
}
