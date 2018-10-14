/*
 * APP_FILE_SELECT.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "APPS.h"

/** Global Variables **/
static uint32_t pageIndex;
static uint32_t itemIndex;
static uint32_t optionIndex;

/** Static Functions **/
/* Hdlr */
static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage);
static SN_STATUS sDisplayHdlr(event_msg_t evtMessage);
static SN_STATUS sFileSystemHdlr(event_msg_t evtMessage);
static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage);

int APP_FILE_SELECT_EvtHdlr(general_evt_t evt)
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

SN_STATUS APP_STATE_EnterStateFileSelect(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;


    pageIndex   = 0;
    itemIndex   = 0;
    optionIndex = 0;

    SN_MODUEL_DISPLAY_FileSelectUpdate();

    APP_SetAppState(APP_STATE_FILE_SELECT);

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
                    /* NOT USED */
                    break;
                    /* IN PAGE BUTTONS */
                case NX_ID_FILE_SELECT_BUTTON_OPTION_UP:
                    SN_MODUEL_DISPLAY_FileSelectUpdate();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_OPTION_DOWN:
                    SN_MODUEL_DISPLAY_FileSelectUpdate();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_PAGE_UP:
                    SN_MODUEL_DISPLAY_FileSelectUpdate();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_PAGE_DOWN:
                    SN_MODUEL_DISPLAY_FileSelectUpdate();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_FILE_SELECT:
                    itemIndex = msgNXId.value;
                    break;
                case NX_ID_FILE_SELECT_BUTTON_PRINT_START:
                    SN_MODUEL_3D_PRINTER_Start(0, 0);
                    APP_STATE_EnterStatePrinting();
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

