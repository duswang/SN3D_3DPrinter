/*
 * APP_WAITING.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "APPS.h"

/** Static Functions **/
/* Hdlr */
static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage);
static SN_STATUS sDisplayHdlr(event_msg_t evtMessage);
static SN_STATUS sFileSystemHdlr(event_msg_t evtMessage);
static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage);

SN_STATUS APP_WAITING_EvtHdlr(general_evt_t evt)
{
    switch(evt.evt_id)
    {
        case APP_EVT_ID_3D_PRINTER:
            s3DPrinterHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_DISPLAY:
            sDisplayHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_FILE_SYSTEM:
            sFileSystemHdlr(evt.evt_msg);
            break;
        case APP_EVT_ID_IMAGE_VIEWER:
            sImageViewerHdlr(evt.evt_msg);
            break;
        default:
            break;
    }
    return SN_STATUS_OK;
}

SN_STATUS APP_STATE_EnterStateWaiting(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    printf("APP STATE => APP_STATE_WAITING\n"); fflush(stdout);
    APP_SetAppState(APP_STATE_WAITING);
    SN_MODULE_DISPLAY_EnterState(APP_STATE_WAITING);

    return retStatus;
}

static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

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
            case NX_ID_WAITING_BUTTON_PRINT:
                SN_MODULE_FILE_SYSTEM_Update();
                break;
            case NX_ID_WAITING_BUTTON_CONTROL:
                APP_STATE_EnterStateControl();
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
    case APP_EVT_MSG_FILE_SYSTEM_USB_MOUNT:
        break;
    case APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT:
        break;
    case APP_EVT_MSG_FILE_SYSTEM_READ_DONE:
        break;
    case APP_EVT_MSG_FILE_SYSTEM_UPDATE_DONE:
        /* USB MOUNT or USER TOUCH PRINT BUTTON */
        APP_STATE_EnterStateFileSelect();
        break;
    default :
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


