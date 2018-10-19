/*
 * SN_MODULE_DISPLAY.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#include "SN_API.h"
#include "SN_MODULE_DISPLAY.h"

/** System **/
/* Thread */
static pthread_mutex_t ptmDisplay = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       ptDisplay;

/* Serial */
sysSerialDef(DisplaySerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE, RETURN_MODE);
static sysSerialId serialIdDisplay;

/* Message Q */
static sysMessageQId   msgQIdDisplay;

/** Moduel Controller **/
static moduleDisplay_t moduleDisplay;

/** Static Funtions */
static void* sDisplayThread();
static void* sDisplaySerial_callback(char *rxBuffer);

static void sDisplayMessagePut(event_id_t evtDisplay_t, event_msg_t evtMessage);
static SN_STATUS sSendCommand(char* command, size_t bufferSize);

SN_STATUS SN_MODULE_DISPLAY_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    printf("START NEXTION DISPLAY INIT.\n"); fflush(stdout);

    /** MESSAGE Q INIT **/
    SN_SYS_MessageQInit(&msgQIdDisplay);

    serialIdDisplay = SN_SYS_SerialCreate(sysSerial(DisplaySerial), sDisplaySerial_callback);

    if (pthread_mutex_init(&ptmDisplay, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if((retStatus = pthread_create(&ptDisplay, NULL, sDisplayThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retStatus);
        fflush(stdout);
    }
    else
    {
        printf("Start Serial Monitor %s\n", UART_DEVICE);
        fflush(stdout);
    }

    sSendCommand(NX_COMMAND_RESET, sizeof(NX_COMMAND_RESET));

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state)
{
    switch(state)
    {
    case NX_PAGE_WAITING:
        sSendCommand(NX_PAGE_WAITING_COMMAND, sizeof(NX_PAGE_WAITING_COMMAND));
        break;
    case NX_PAGE_CONTROl:
        sSendCommand(NX_PAGE_CONTROL_COMMAND, sizeof(NX_PAGE_CONTROL_COMMAND));
        break;
    case NX_PAGE_FILE_SELECT:
        sSendCommand(NX_PAGE_FILE_SYSTEM_COMMAND, sizeof(NX_PAGE_FILE_SYSTEM_COMMAND));
        break;
    case NX_PAGE_PRINTING:
    case NX_PAGE_PAUSE:
        sSendCommand(NX_PAGE_PRINTING_COMMAND, sizeof(NX_PAGE_PRINTING_COMMAND));
        break;
    case NX_PAGE_SETUP:
    case NX_PAGE_INIT:
        sSendCommand(NX_COMMAND_RESET, sizeof(NX_COMMAND_RESET));
        break;
    case NX_PAGE_LOADING:
        sSendCommand(NX_PAGE_LOADING_COMMAND, sizeof(NX_PAGE_LOADING_COMMAND));
        break;
    default:
        break;
    }

    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_DISPLAY_PrintingUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t page)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int itemIndex = 0;
    char buffer[50];

    SN_MODULE_FILE_SYSTEM_Get(&moduleDisplay.fs);

    if(page <= moduleDisplay.fs.pageCnt)
    {
        for(itemIndex = 0; itemIndex < MAX_ITEM_SIZE; itemIndex++)
        {
            if(itemIndex <= moduleDisplay.fs.page[page].itemCnt)
            {
                sprintf(buffer,"Index_%d.txt=\"%s\"", itemIndex, moduleDisplay.fs.page[page].item[itemIndex].name);

                sSendCommand(buffer, strlen(buffer) + 1);
            }
            else
            {
                /* EMPTY ITEM */
            }

        }
    }

    return retStatus;
}

static void* sDisplaySerial_callback(char * rxBuffer)
{
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
        while(i < 8/* NEED DEF */)
        {
            msgNXId.NXmessage8bit[i] = rxBuffer[i];
            i++;
        }
#endif
    sDisplayMessagePut(MSG_DISPLAY_DATA_RX, msgNXId.NXmessage[0]);
        break;
    case NX_COMMAND_FINISHED:
        printf("GET RESPONSE <= NEXTION DISPLAY\n"); fflush(stdout);
        break;
    case NX_COMMAND_EVT_LAUNCHED:
        printf("NEXTION DISPLAY LAUNCHED.\n"); fflush(stdout);
        break;
    default:
        printf("GET MESSAGE  <= NEXTION DISPLAY\n"); fflush(stdout);
        break;
    }


    return 0;
}

static void* sDisplayThread()
{
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdDisplay);

        switch(evt.evt_id)
        {
            case MSG_DISPLAY_DATA_RX:
                SN_SYSTEM_SendAppMessage(APP_EVT_ID_DISPLAY, evt.evt_msg);
                break;
            case MSG_DISPLAY_DATA_TX:

                break;
            default:
                break;
        }
    }

    return 0;
}

static void sDisplayMessagePut(evtDisplay_t evtId, event_msg_t evtMessage)
{
    SN_SYS_MessagePut(&msgQIdDisplay, evtId, evtMessage);
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
    SN_SYS_SerialTx(serialIdDisplay, command, bufferSize);
    pthread_mutex_unlock(&ptmDisplay);

    return retStatus;
}

static SN_STATUS sResponseFinished(void)
{
    moduleDisplay.exitFlag = true;

    return SN_STATUS_OK;
}
