/*
 * APP_CONTROL.c
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

int APP_CONTROL_EvtHdlr(general_evt_t evt)
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
    return 0;
}

SN_STATUS APP_STATE_EnterStateControl(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    APP_SetAppState(APP_STATE_CONTROL);

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
    msgNXId.NXmessage[1] = NX_ENDCODE;

    switch(msgNXId.type)
    {
        case NX_TYPE_BUTTON:
            switch(msgNXId.id)
            {
                /* MENU */
                case NX_ID_BUTTON_HOME:
                    APP_STATE_EnterStateWaiting();
                    break;
                case NX_ID_BUTTON_PRINT:
                    /* BLOCK */
                    break;
                /* IN PAGE BUTTONS */
                case NX_ID_CONTROL_BUTTON_Z_UP:
                    SN_MODUEL_3D_PRINTER_Z_UP(msgNXId.value);
                    break;
                case NX_ID_CONTROL_BUTTON_Z_DOWN:
                    SN_MODUEL_3D_PRINTER_Z_DOWN(msgNXId.value);
                    break;
                case NX_ID_CONTROL_BUTTON_Z_HOMMING:
                    SN_MODUEL_3D_PRINTER_Z_HOMING();
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

    return retStatus;
}

static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

