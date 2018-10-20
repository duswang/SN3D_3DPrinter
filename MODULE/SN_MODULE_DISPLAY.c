/*
 * SN_MODULE_DISPLAY.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#include "SN_API.h"
#include "SN_MODULE_DISPLAY.h"
/******** STATIC DEFINE ********/
/**** SERIAL CONFIG ****/
#define BYTE_SIZE   SN_SYS_SERIAL_COMM_RX_REALTIME
#define BAUD_RATE   SN_SYS_SERIAL_COMM_BAUD_RATE_9600
#define RETURN_MODE SN_SYS_SERIAL_COMM_TX_NX_RETURN

#ifdef __APPLE__
#define UART_DEVICE "/dev/cu.usbmodem1421"
#endif
#ifdef linux
#define UART_DEVICE "/dev/serial0"
#endif

#define UART_OFLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

/**** MODULE ****/
typedef struct moduel_display {
    uint32_t reserve;
    /** Reponse Finished Waiting **/
    bool exitFlag;
    fs_t fs;
} moduleDisplay_t;

/******** SYSTEM DEFINE ********/
/**** MODULE THREAD ****/
static pthread_mutex_t ptmDisplay = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       ptDisplay;

/**** MODULE SERIAL ****/
sysSerialDef(DisplaySerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialIdDisplay;

/**** MODULE MESSAGE Q ****/
static sysMessageQId   msgQIdDisplay;
    /**** MODULE MESSAGES ****/
typedef enum {
    MSG_DISPLAY_DATA_RX         = 0,
    MSG_DISPLAY_DATA_TX,
    MSG_DISPLAY_NONE,
    MSG_DISPLAY_IGNORE          = 0xFF01
} evtDisplay_t;

/**** MODULE HANDLER  ****/
static moduleDisplay_t moduleDisplay;

/******** GLOBAL VARIABLE ********/

/******** STATIC FUNCTIONS ********/
/**** SYSTEM ****/
static void* sDisplayThread();
static SN_STATUS sDisplayMessagePut(event_id_t evtDisplay_t, event_msg_t evtMessage);

/**** SERIAL ****/
static void* sSerialRx_Callback(char *rxBuffer);
static SN_STATUS sSendCommand(char* command, size_t bufferSize);


SN_STATUS SN_MODULE_DISPLAY_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("MODULE INIT => NEXTION DISPLAY.");

    /** MESSAGE Q INIT **/
    retStatus = SN_SYS_MessageQInit(&msgQIdDisplay);
    SN_SYS_ERROR_CHECK(retStatus, "Display Module Message Q Init Failed.");

    /** SERIAL INIT **/
    serialIdDisplay = SN_SYS_SerialCreate(sysSerial(DisplaySerial), sSerialRx_Callback);
    if(serialIdDisplay == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Display Serial Init Failed.");
    }

    /** MUTEX INIT **/
    if (pthread_mutex_init(&ptmDisplay, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Display Mutex Init Failed.");
    }

    /** THREAD INIT **/
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
        SDL_Delay(3000);
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

    sprintf(buffer,"FileName.txt=\"%s\"", fileName);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");

    sprintf(buffer,"Option.txt=\"%s\"", optionName);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");


    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[256];

    sprintf(buffer,"Image.txt=\"%4d/ %4d\"", slice, targetSlice);

    retStatus = sSendCommand(buffer, strlen(buffer) + 1);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");


    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingTimeUpdate(const char* timeStr)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char buffer[256];

    if(timeStr == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    return retStatus;
}



SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t page)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    int itemIndex = 0;
    char buffer[256];

    /** Get File System **/
    retStatus = SN_MODULE_FILE_SYSTEM_Get(&moduleDisplay.fs);
    SN_SYS_ERROR_CHECK(retStatus, "Get File System Failed.");

    /** Send to Nextion Display **/
    if(page <= moduleDisplay.fs.pageCnt)
    {
        for(itemIndex = 0; itemIndex < MAX_ITEM_SIZE; itemIndex++)
        {
            if(itemIndex <= moduleDisplay.fs.page[page].itemCnt)
            {
                sprintf(buffer,"Index_%d.txt=\"%s\"", itemIndex, moduleDisplay.fs.page[page].item[itemIndex].name);

                retStatus = sSendCommand(buffer, strlen(buffer) + 1);
                SN_SYS_ERROR_CHECK(retStatus, "Nextion Display File System Update Failed.");
            }
            else
            {
                break;
            }

        }
    }

    return retStatus;
}

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
        retStatus = sDisplayMessagePut(MSG_DISPLAY_DATA_RX, msgNXId.NXmessage[0]);
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

static void* sDisplayThread()
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdDisplay);

        switch(evt.evt_id)
        {
            case MSG_DISPLAY_DATA_RX:
                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_DISPLAY, evt.evt_msg);
                SN_SYS_ERROR_CHECK(retStatus, "APP Send Message Failed.");
                break;
            case MSG_DISPLAY_DATA_TX:
                break;
            case MSG_DISPLAY_IGNORE:
                break;
            default:
                SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "Display Get Unknown Message.");
                break;
        }
        SN_SYS_ERROR_CHECK(retStatus, "Display Module Get Error.");
    }

    return NULL;
}

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
