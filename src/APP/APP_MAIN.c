/**
 * @file APP_MAIN.c
 * @author Bato
 * @date 24 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo
 */
#include "APPS.h"

/* **** GLOBAL VARIABLE **** */
static app_state_t gAppState = APP_STATE_INIT;

SN_STATUS APP_Main(general_evt_t evt)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    app_state_t tAppState;

    tAppState = APP_GetAppState();

    switch(tAppState)
    {
        case APP_STATE_WAITING:
            retStatus = APP_WAITING_EvtHdlr(evt);
            break;
        case APP_STATE_CONTROL:
            retStatus = APP_CONTROL_EvtHdlr(evt);
            break;
        case APP_STATE_FILE_SELECT:
            retStatus = APP_FILE_SELECT_EvtHdlr(evt);
            break;
        case APP_STATE_PRINTING:
            retStatus = APP_PRINTING_EvtHdlr(evt);
            break;
        case APP_STATE_PAUSE:
            retStatus = APP_PAUSE_EvtHdlr(evt);
            break;
        case APP_STATE_SETUP:
        case APP_STATE_INIT:
            retStatus = APP_INIT_EvtHdlr(evt);
            break;
        case APP_STATE_INFO:
            retStatus = APP_INFO_EvtHdlr(evt);
            break;
        default:
            SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "Unknown State.");
            break;
    }

    SN_SYS_ERROR_CHECK(retStatus, "Untrackted Error.");

    return retStatus;
}

SN_STATUS APP_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = APP_STATE_EnterStateInit();

    return retStatus;
}

app_state_t  APP_GetAppState(void)
{
    return gAppState;
}

void APP_SetAppState(app_state_t tAppState)
{
    gAppState = tAppState;
}
