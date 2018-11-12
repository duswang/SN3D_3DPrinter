/**
 * @file APP_INIT.c
 * @author Bato
 * @date 24 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo bootUp and Resolution Settup.
 */
#include "APPS.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** HANDLER *** */
static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage);
static SN_STATUS sDisplayHdlr(event_msg_t evtMessage);
static SN_STATUS sFileSystemHdlr(event_msg_t evtMessage);
static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage);

static SN_STATUS sBootup(void);

SN_STATUS APP_INIT_EvtHdlr(general_evt_t evt)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evt.evt_id)
    {
        case APP_EVT_ID_3D_PRINTER:
            retStatus = s3DPrinterHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_DISPLAY:
            retStatus = sDisplayHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_FILE_SYSTEM:
            retStatus = sFileSystemHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_IMAGE_VIEWER:
            retStatus = sImageViewerHdlr(evt.evt_msg);
            break;
        default:
            break;
    }

    SN_SYS_ERROR_CHECK(retStatus, "ERROR IN [ INIT ]");

    return retStatus;
}

SN_STATUS APP_STATE_EnterStateInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("APP STATE => [ INIT ]\n");

    APP_SetAppState(APP_STATE_INIT);
    SN_MODULE_DISPLAY_EnterState(APP_STATE_INIT);

    sBootup();

    return retStatus;
}

static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evtMessage)
    {
    case APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE:
        break;
    default:
        break;
    }

    return retStatus;
}

static SN_STATUS sDisplayHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    msgNX_t msgNXId;

    /** Message parsing **/
    msgNXId.NXmessage[0] = evtMessage;

    switch(msgNXId.type)
    {
        case NX_TYPE_BUTTON:
            switch(msgNXId.id)
            {
                /* MENU */
                case NX_ID_BUTTON_HOME:
                    /* NOT USED */
                    break;
                case NX_ID_BUTTON_PRINT:
                    /* NOT USED */
                    break;
                /* IN PAGE BUTTONS */
                case NX_ID_INIT_BUTTON_RESERVE:
                    break;
                default:
                    break;
            }
            break;
        case NX_TYPE_TOGGLE:
        case NX_TYPE_SCREEN_TOUCH:
            break;
        default:
            break;
    }

    return retStatus;
}

static SN_STATUS sFileSystemHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evtMessage)
    {
    default:
        break;
    }

    return retStatus;
}

static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evtMessage)
    {
    default:
        break;
    }

    return retStatus;
}

static SN_STATUS sBootup(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_MODULE_DISPLAY_BootProgressUpdate(20, "System Booting...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(30, "Ramps Board Initialized...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(40, "Screen Initialized...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(45, "USB File Read...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(50, "Option File Read...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(55, "Device Concifg Read...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(80, "System Initialzied...");
    SN_SYS_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(100, "init finished...");
    SN_SYS_Delay(500);

    retStatus = APP_STATE_EnterStateWaiting();

    return retStatus;
}



