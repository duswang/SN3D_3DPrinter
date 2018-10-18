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

/** APP Main Call **/
extern SN_STATUS APP_Main(general_evt_t evt);     /* To execute Main application thread */
extern void APP_Init(void);

/** System **/
/* Message Q */
sysMessageQId msgQIdApp;

/** Global Variables **/


/** Static Funtions **/
static SN_STATUS sSN_SYSTEM_Init(void);

int main(void)
{
    general_evt_t evt;

    /* Init */
    sSN_SYSTEM_Init();

    /** FILE SYSTEM INIT  - IT MUST BE FIRST **/
    SN_MODULE_FILE_SYSTEM_Init();

    /** IMAGE VIEWER INIT - IT NEED MACHINE INFO FROM "FILE SYSTEM" FOR INIT **/
    SN_MODULE_IMAGE_VIEWER_Init();

    /** DISPLAY INIT      - NOPE **/
    SN_MODULE_DISPLAY_Init();

    /** 3D PRINTER INIT   - NOPE **/
    SN_MODULE_3D_PRINTER_Init();

    APP_Init();

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdApp);

        switch(evt.evt_id)
        {
            case APP_EVT_ID_IGNORE:
                break;

            case APP_EVT_ID_3D_PRINTER:
            case APP_EVT_ID_DISPLAY:
            case APP_EVT_ID_FILE_SYSTEM:
            case APP_EVT_ID_IMAGE_VIEWER:
                APP_Main(evt);
                break;

            default:
                break;
        }
    }

}

static SN_STATUS sSN_SYSTEM_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    sysTimerId_t testTimerId;
  
    sysMessageQId msgTest; //@DEBUG
  
    /** @DEBUG **/
    SN_SYS_MessageQInit(&msgTest);

    /** APP Message Q Init **/
    SN_SYS_MessageQInit(&msgQIdApp);
    
    /** Timer Init **/
    SN_SYS_TimerInit();

    /** Serial Init **/
    SN_SYS_SerialInit();

    return retStatus;
}

SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //printf("MSG MODULE => APP\n"); fflush(stdout);
    retStatus = SN_SYS_MessagePut(&msgQIdApp, evtId, evtMessage);

    return retStatus;
}


