/*
 ============================================================================
 Name        : Nextion_Project
 Author      : bato
 Version     : 0.1
 Copyright   :
 Description :
 ============================================================================
 */
/*
 * main.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */
#include "SN_API.h"

#define SN_APP_QKEY      IPC_PRIVATE
#define SN_APP_MSG_TYEP  EVT_ID_APP

sysMessageQId msgQIdApp;


extern int APP_Main(general_evt_t evt);     /* To execute Main application thread */
extern void APP_Init(void);


/** static function definitions */
static int sSN_SYSTEM_Init(void);

int main(void)
{
    general_evt_t evt;

    /* Init */
    sSN_SYSTEM_Init();

    APP_Init();

    //MODUELS_DISPLAY_Serial_Init();

    //SN_MODUEL_3D_PRINTER_Init();

    SN_MODUEL_FILE_SYSTEM_Init();

    while(true)
    {
        /* get Message */
        evt = SN_SYS_MessageGet(&msgQIdApp);

        APP_Main(evt);
    }
}


int sSN_SYSTEM_Init(void)
{
    /** MESSAGE Q INIT **/
    SN_SYS_MessageInit(&msgQIdApp, SN_APP_QKEY, SN_APP_MSG_TYEP);

    return 0;
}

int SN_SYSTEM_SendApplMessage(event_id evtId, uint32_t evtValue)
{
    SN_SYS_MessagePut(&msgQIdApp, evtId, evtValue);

    return 0;
}


