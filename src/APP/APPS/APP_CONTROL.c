/**
 * @file APP_CONTROL.c
 * @author Bato
 * @date 24 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
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

/**** UTIL ****/
static float sGetZmmFromValue(uint8_t nxValue);

SN_STATUS APP_CONTROL_EvtHdlr(general_evt_t evt)
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

    SN_SYS_ERROR_CHECK(retStatus, "ERROR IN [ CONTROL ]");

    return retStatus;
}

SN_STATUS APP_STATE_EnterStateControl(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("APP STATE => [ CONTROL ]\n");

    APP_SetAppState(APP_STATE_CONTROL);
    SN_MODULE_DISPLAY_EnterState(APP_STATE_CONTROL);

    retStatus = SN_MODULE_3D_PRINTER_MotorInit();

    return retStatus;
}

/*** Public Functions ***/
static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evtMessage)
    {
    case APP_EVT_MSG_3D_PRINTER_HOMING_DONE:
        retStatus = SN_MODULE_DISPLAY_EnterState(NX_PAGE_CONTROL);
        break;
    case APP_EVT_MSG_3D_PRINTER_Z_MOVE_DONE:
        break;
    case APP_EVT_MSG_3D_PRINTER_PAUSE:
        break;
    case APP_EVT_MSG_3D_PRINTER_STOP:
        break;
    case APP_EVT_MSG_3D_PRINTER_FINISH:
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
                    retStatus = APP_STATE_EnterStateWaiting();
                    break;
                case NX_ID_BUTTON_PRINT:
                    /* BLOCK */
                    break;
                /* IN PAGE BUTTONS */
                case NX_ID_CONTROL_BUTTON_Z_UP:
                        retStatus = SN_MODULE_3D_PRINTER_Z_Up(sGetZmmFromValue(msgNXId.value));
                    break;
                case NX_ID_CONTROL_BUTTON_Z_DOWN:
                        retStatus = SN_MODULE_3D_PRINTER_Z_Down(sGetZmmFromValue(msgNXId.value));
                    break;
                case NX_ID_CONTROL_BUTTON_Z_HOMMING:
                    retStatus = SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);
                    retStatus = SN_MODULE_3D_PRINTER_Z_Homing();
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



/*** Private Functions ***/
static float sGetZmmFromValue(uint8_t nxValue)
{
    float retValue = 0.0;

    switch(nxValue)
    {
    case NX_VALUE_CONTROL_Z_000_1_MM:
        retValue = 0.1;
      break;
    case NX_VALUE_CONTROL_Z_001_0_MM:
        retValue = 1.0;
      break;
    case NX_VALUE_CONTROL_Z_005_0_MM:
        retValue = 5.0;
      break;
    case NX_VALUE_CONTROL_Z_010_0_MM:
        retValue = 10.0;
      break;
    case NX_VALUE_CONTROL_Z_050_0_MM:
        retValue = 50.0;
      break;
    case NX_VALUE_CONTROL_Z_100_0_MM:
        retValue = 100.0;
      break;
    default:
        retValue = 0.0;
      break;
    }

    return retValue;
}
