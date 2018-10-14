/*
 * APP_MAIN.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "APPS.h"

/** Global Variables **/
//static app_state_t gAppState = APP_STATE_INIT;
static app_state_t gAppState = APP_STATE_WAITING;

SN_STATUS APP_Main(general_evt_t evt)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    app_state_t tAppState;

    tAppState = APP_GetAppState();

    printf("MSG APP <= MODULE\n"); fflush(stdout);

    switch(tAppState)
    {
        case APP_STATE_WAITING:
            APP_WAITING_EvtHdlr(evt);
            break;
        case APP_STATE_CONTROL:
            APP_CONTROL_EvtHdlr(evt);
            break;
        case APP_STATE_FILE_SELECT:
            APP_FILE_SELECT_EvtHdlr(evt);
            break;
        case APP_STATE_PRINTING:
            APP_PRINTING_EvtHdlr(evt);
            break;
        case APP_STATE_PAUSE:
            APP_PAUSE_EvtHdlr(evt);
            break;
        case APP_STATE_SETUP:
        case APP_STATE_INIT:
            APP_INIT_EvtHdlr(evt);
            break;

        default:
            break;
    }
    return retStatus;
}

void APP_Init(void)
{
    /** Enter Waiting **/
}

app_state_t  APP_GetAppState(void)
{
    return gAppState;
}

void APP_SetAppState(app_state_t tAppState)
{
    gAppState = tAppState;
}

