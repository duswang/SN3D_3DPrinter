/*
 * SN_MODUEL_DISPLAY.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#include "SN_API.h"
#include "SN_MODUEL_DISPLAY.h"

/** System **/
/* Thread */
pthread_mutex_t ptmDisplay = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptDisplay;

/* Serial */
sysSerialDef(DisplaySerial, UART_DEVICE, UART_OFLAGS, BAUD_RATE, BYTE_SIZE);
sysSerialId serialIdDisplay;

/* Message Q */
sysMessageQId   msgQIdDisplay;

/** Moduel Controller **/

/** Static Funtions */
static void* sDisplayThread();
static void* sDisplaySerial_callback();

static void sDisplayMessagePut(event_id_t evtDisplay_t, event_msg_t evtMessage);



SN_STATUS SN_MODUEL_DISPLAY_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

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

    return retStatus;
}

SN_STATUS SN_MODUEL_DISPLAY_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODUEL_DISPLAY_PrintingUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODUEL_DISPLAY_FileSelectUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

static void* sDisplaySerial_callback()
{
    int i = 0;
    char* buffer = SN_SYS_SerialRx(serialIdDisplay);
    msgNX_t msgNXId;


    while(i < BYTE_SIZE)
    {
        msgNXId.NXmessage8bit[(BYTE_SIZE - 1) - i] = buffer[i];
        printf(" 0x%02x", msgNXId.NXmessage8bit[(BYTE_SIZE - 1) - i]);
        i++;
    }

    printf("\n");
    fflush(stdout);

    sDisplayMessagePut(MSG_DISPLAY_DATA_RX, msgNXId.NXmessage[0]);

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
