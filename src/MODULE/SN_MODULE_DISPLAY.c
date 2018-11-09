/**
 * @file SN_MODULE_DISPLAY.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo Nextion display image drawing.
 */

#include "SN_API.h"
#include "SN_MODULE_DISPLAY.h"

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
#define DEFAULT_BUFFER_SIZE 256
#define TIMER_INDICATE_INTERVAL 1000 /**< 1 second */
///@}

typedef struct time_display {
    uint32_t sec;
    uint32_t min;
    uint32_t hour;
} timeInfo_t;

/* *** MODULE *** */
typedef struct moduel_display {

    /** Timer Info **/
    timeInfo_t estimatedBuildTime;
    timeInfo_t nowTime;
    uint32_t   secNowTime;
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
    MSG_DISPLAY_TIME_INFO_TIMER_UPDATE,     /**< Time Info Update */
    MSG_DISPLAY_NONE,                       /**< BAD ACCESS */
} evtDisplay_t;

/* *** MODULE TIMER *** */
static sysTimerId_t timerTimeIndicate;

/* *** MODULE HANDLER  *** */
static moduleDisplay_t moduleDisplay;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** SYSTEM *** */
static void* sDisplayThread();
static SN_STATUS sDisplayMessagePut(event_id_t evtDisplay_t, event_msg_t evtMessage);

/* *** TIMER *** */
static void sTMR_TimerUpdate_UpdateCallback(void);

/* *** SERIAL *** */
static void* sSerialRx_Callback(char *rxBuffer);
static SN_STATUS sSendCommand(char* command, size_t bufferSize);

/* *** TIME INFO *** */
static SN_STATUS sDisplay_TimerInfoUpdate(void);

/* *** UTIL *** */
static timeInfo_t sSecToTimeInfo(uint32_t sec);

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

    switch(state)
    {
    case NX_PAGE_WAITING:
        retStatus = sSendCommand(NX_PAGE_WAITING_COMMAND, sizeof(NX_PAGE_WAITING_COMMAND));
        break;
    case NX_PAGE_CONTROl:
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
    case NX_PAGE_INIT:
        retStatus = sSendCommand(NX_COMMAND_RESET, sizeof(NX_COMMAND_RESET));
        SN_SYS_Delay(3000);
        break;
    case NX_PAGE_LOADING:
        retStatus = sSendCommand(NX_PAGE_LOADING_COMMAND, sizeof(NX_PAGE_LOADING_COMMAND));
        break;
    default:
        retStatus = SN_STATUS_INVALID_PARAM;
        break;
    }

    SN_SYS_ERROR_CHECK(retStatus, "ERROR IN DISPLAY PAGE UPDATE.");

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingInfoInit(const char* fileName, const char* optionName)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[256];

    if(fileName == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if(optionName == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    sprintf(buffer,"Print.FileName.txt=\"%s\"", fileName);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    sprintf(buffer,"Print.Option.txt=\"%s\"", optionName);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");


    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[DEFAULT_BUFFER_SIZE];

    sprintf(buffer,"Print.Image.txt=\"%3d/ %3d\"", slice, targetSlice);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");


    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimerInit(uint32_t sec)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[30];

    moduleDisplay.estimatedBuildTime = sSecToTimeInfo(sec);
    moduleDisplay.nowTime.hour       = 0;
    moduleDisplay.nowTime.min        = 0;
    moduleDisplay.nowTime.sec        = 0;

    moduleDisplay.IsTimerInfoInit = true;

    sprintf(buffer,"Print.FinishTime.txt=\">%02d:%02d:%02d\"", \
            moduleDisplay.estimatedBuildTime.hour, \
            moduleDisplay.estimatedBuildTime.min, \
            moduleDisplay.estimatedBuildTime.sec);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");


    retStatus = sDisplayMessagePut(MSG_DISPLAY_TIME_INFO_TIMER_UPDATE, 0);
    SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");

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


SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t pageIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char buffer[MAX_PATH_LENGTH];
    uint32_t itemIndex = 0;

    const fs_t fileSystem       = SN_MODULE_FILE_SYSTEM_GetFileSystem();
    const fsPage_t* currentPage = SN_MODULE_FILE_SYSTEM_GetPage(pageIndex);

    if(fileSystem.pageHeader->isItemExist)
    {
        /* Send to Nextion Display */
        if(pageIndex <= fileSystem.pageHeader->pageCnt)
        {
            for(itemIndex = 0; itemIndex < MAX_ITEM_SIZE; itemIndex++)
            {
                if(itemIndex <= currentPage->itemCnt)
                {
                    sprintf(buffer,"Index_%d.txt=\"%s\"", itemIndex, currentPage->item[itemIndex].name);
                }
                else
                {
                    sprintf(buffer,"Index_%d.txt=\" \"", itemIndex);
                }

                retStatus = sSendCommand(buffer, strlen(buffer) + 1);
                SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
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
        printf("DATA RX <= "); fflush(stdout);
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
        retStatus = sDisplayMessagePut(MSG_DISPLAY_EVENT_GET, msgNXId.NXmessage[0]);
        SN_SYS_ERROR_CHECK(retStatus, "Display Send Message Failed.");
        break;
    case NX_COMMAND_FINISHED:
#if(DISPLAY_RX_DEBUG)
        SN_SYS_Log("RESPONSE OK.");
#endif
        break;
    case NX_COMMAND_EVT_LAUNCHED:
#if(DISPLAY_RX_DEBUG)
        SN_SYS_Log("NEXTION DISPLAY LAUNCHED.");
#endif
        break;
    default:
        break;
    }

    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Timer Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sTMR_TimerUpdate_UpdateCallback(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* INCREASE ONE SECOND */
    moduleDisplay.secNowTime++;

    retStatus = sDisplayMessagePut(MSG_DISPLAY_TIME_INFO_TIMER_UPDATE, 0);
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

        sprintf(buffer,"Print.Time.txt=\"%02d:%02d:%02d\"", \
                moduleDisplay.nowTime.hour, \
                moduleDisplay.nowTime.min, \
                moduleDisplay.nowTime.sec);

        retStatus = sSendCommand(buffer, strlen(buffer) + 1);
        SN_SYS_ERROR_CHECK(retStatus, "Nextion Display Timer Update Failed.");

        retStatus = SN_SYS_TimerCreate(&timerTimeIndicate, TIMER_INDICATE_INTERVAL, sTMR_TimerUpdate_UpdateCallback);
        SN_SYS_ERROR_CHECK(retStatus, "Timer Cretae Failed.");
    }
    else
    {
        SN_SYS_Log("BAD WAY ACCESS Timer Info Update.");
    }

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

    pthread_mutex_lock(&ptmDisplay);
    retStatus = SN_SYS_SerialTx(serialIdDisplay, command, bufferSize);
    pthread_mutex_unlock(&ptmDisplay);

    return retStatus;
}
