/*
 ============================================================================
 Name        : SN3D_PROJECT
 Author      : bato
 Version     : 0.1
 Copyright   :
 Description :
 ============================================================================
 */
/*
 * SN_SYS_INIT.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */
#include "SN_API.h"

/**** APP Main Call ****/
extern SN_STATUS APP_Main(general_evt_t evt);     /* To execute Main application thread */
extern SN_STATUS APP_Init(void);

/******** SYSTEM DEFINE ********/
/**** MODULE MESSAGE Q ****/
sysMessageQId msgQIdApp;
    /**** MODULE MESSAGES ****/
    //"APP_MESSAGES.h"

/******** STATIC FUNCTIONS ********/
static SN_STATUS sSN_SYSTEM_Init(void);

int main(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    /**** SYSTEM INIT ****/
    SN_SYS_Log("==>SYSTEM INIT");

    retStatus = sSN_SYSTEM_Init();
    SN_SYS_ERROR_CHECK(retStatus, "SYSTEM INIT FAILD.");

    /**** MODULE INIT ****/
    SN_SYS_Log("==>MODULE INIT");

    /** FILE SYSTEM INIT **/
    retStatus = SN_MODULE_FILE_SYSTEM_Init();
    SN_SYS_ERROR_CHECK(retStatus, "FILE SYSTEM INIT FAILD.");

    /** NEXTION DISPLAY INIT **/
    retStatus = SN_MODULE_DISPLAY_Init();
    SN_SYS_ERROR_CHECK(retStatus, "NEXTION DISPLAY INIT FAILD.");

    /** 3D PRINTER INIT ( IMAGE VIEWER INIT ) **/
    retStatus = SN_MODULE_3D_PRINTER_Init();
    SN_SYS_ERROR_CHECK(retStatus, "3D PRINTER INIT FAILD.");
    
    /**** APP INIT ****/
    SN_SYS_Log("==>APP INIT");

    retStatus = APP_Init();
    SN_SYS_ERROR_CHECK(retStatus,"APP INIT FAILD.");

    /**** To APP Main ****/
    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdApp);

        switch(evt.evt_id)
        {
            case APP_EVT_ID_IGNORE:
                /* FROM TIMER message => Interrupted Signal */
                break;
            case APP_EVT_ID_3D_PRINTER:
            case APP_EVT_ID_DISPLAY:
            case APP_EVT_ID_FILE_SYSTEM:
            case APP_EVT_ID_IMAGE_VIEWER:
                APP_Main(evt);
                break;

            default:
                SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "UNKNOWN APP MESSAGE_ID.");
                break;
        }
    }

}

static SN_STATUS sSN_SYSTEM_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** @DEBUG **/
    sysMessageQId msgTest;
    SN_SYS_MessageQInit(&msgTest);

    /** APP Message Q Init **/
    SN_SYS_Log("SYSTEM INIT => MESSAGE QUEUE.");
    SN_SYS_MessageQInit(&msgQIdApp);

    /** Timer Init **/
    SN_SYS_Log("SYSTEM INIT => TIMER.");
    SN_SYS_TimerInit();

    /** Serial Init **/
    SN_SYS_Log("SYSTEM INIT => SERIAL.");
    SN_SYS_SerialInit();

    return retStatus;
}

SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = SN_SYS_MessagePut(&msgQIdApp, evtId, evtMessage);

    return retStatus;
}
