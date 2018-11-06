/**
 * @file APP_FILE_SELECT.c
 * @author Bato
 * @date 24 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
#include "APPS.h"

/* ******* GLOBAL VARIABLE ******* */
static fs_t     fs;
static uint32_t pageIndex;
static uint32_t itemIndex;
static uint32_t optionIndex;


/* ******* STATIC FUNCTIONS ******* */
/* *** HANDLER *** */
static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage);
static SN_STATUS sDisplayHdlr(event_msg_t evtMessage);
static SN_STATUS sFileSystemHdlr(event_msg_t evtMessage);
static SN_STATUS sImageViewerHdlr(event_msg_t evtMessage);

/* *** BUTTON *** */
/* PAGE */
static void sUpPageIndex(void);
static void sDownPageIndex(void);
static uint32_t sGetPageIndex(void);

/* OPTION */
static void sUpOptionIndex(void);
static void sDownOptionIndex(void);

/* *** UTIL *** */
static bool sItemOverIndexCheck(uint32_t itemIndex);

/* *** ETC *** */
static void sResetIndexs(void);

SN_STATUS APP_FILE_SELECT_EvtHdlr(general_evt_t evt)
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

    SN_SYS_ERROR_CHECK(retStatus, "ERROR IN [ FILE SELECT ]");

    return retStatus;
}


SN_STATUS APP_STATE_EnterStateFileSelect(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("APP STATE => [ FILE SELECT ]");

    APP_SetAppState(APP_STATE_FILE_SELECT);
    SN_MODULE_DISPLAY_EnterState(APP_STATE_FILE_SELECT);

    sResetIndexs();

    retStatus = SN_MODULE_FILE_SYSTEM_Get(&fs);
    retStatus = SN_MODULE_DISPLAY_FileSelectUpdate(pageIndex);

    return retStatus;
}


static SN_STATUS s3DPrinterHdlr(event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    switch(evtMessage)
    {
    case APP_EVT_MSG_3D_PRINTER_HOMING_DONE:
        sResetIndexs();
        retStatus = APP_STATE_EnterStatePrinting();
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
                    sResetIndexs();
                    retStatus = APP_STATE_EnterStateWaiting();
                    break;
                case NX_ID_BUTTON_PRINT:
                    /* NOT USED */
                    break;
                    /* IN PAGE BUTTONS */
                case NX_ID_FILE_SELECT_BUTTON_OPTION_UP:
                    sUpOptionIndex();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_OPTION_DOWN:
                    sDownOptionIndex();
                    break;
                case NX_ID_FILE_SELECT_BUTTON_PAGE_UP:
                    if(fs.isItemExist)
                    {
                        sUpPageIndex();
                        retStatus = SN_MODULE_DISPLAY_FileSelectUpdate(pageIndex);
                    }

                    break;
                case NX_ID_FILE_SELECT_BUTTON_PAGE_DOWN:
                    if(fs.isItemExist)
                    {
                        sDownPageIndex();
                        retStatus = SN_MODULE_DISPLAY_FileSelectUpdate(pageIndex);
                    }
                    break;
                case NX_ID_FILE_SELECT_BUTTON_FILE_SELECT:
                    itemIndex = msgNXId.value;
                    break;
                case NX_ID_FILE_SELECT_BUTTON_PRINT_START:
                    if(fs.isItemExist && sItemOverIndexCheck(msgNXId.value - 1))
                    {
                        retStatus = SN_MODULE_DISPLAY_EnterState(NX_PAGE_LOADING);
                        SN_MODULE_3D_PRINTER_Start(sGetPageIndex(), (msgNXId.value - 1));
                    }
                    else
                    {
                        /* NO FILE OR INVAILD ITEM INDEX */
                    }
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

    SN_MODULE_FILE_SYSTEM_Get(&fs);
    SN_MODULE_DISPLAY_FileSelectUpdate(pageIndex);

    switch(evtMessage)
    {
    case APP_EVT_MSG_FILE_SYSTEM_USB_MOUNT:
        break;
    case APP_EVT_MSG_FILE_SYSTEM_READ_DONE:
        break;
    case APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT:
    case APP_EVT_MSG_FILE_SYSTEM_UPDATE:
        /* USB MOUNT or USER TOUCH PRINT BUTTON */
        retStatus = SN_MODULE_DISPLAY_FileSelectUpdate(pageIndex);
        break;
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



static uint32_t sGetPageIndex(void)
{
    return pageIndex;
}

static void sUpPageIndex(void)
{
    if(pageIndex < fs.pageCnt)
    {
        pageIndex++;
    }
}

static void sDownPageIndex(void)
{
    if(pageIndex > 0)
    {
        pageIndex--;
    }
}

static void sUpOptionIndex(void)
{

}

static void sDownOptionIndex(void)
{

}

static bool sItemOverIndexCheck(uint32_t itemIndex)
{
    return (fs.page[pageIndex].itemCnt > itemIndex);
}

static void sResetIndexs(void)
{
    pageIndex = 0;
    itemIndex = 0;
    optionIndex = 0;
}