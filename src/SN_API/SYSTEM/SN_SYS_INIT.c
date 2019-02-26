/**
 * @file SN_SYS_INIT.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"

/* *** APP Main Call *** */
extern SN_STATUS APP_Main(general_evt_t evt);     /* To execute Main application thread */
extern SN_STATUS APP_Init(void);

/* ******* STATIC DEFINE ******* */

/* ******* SYSTEM DEFINE ******* */
/* *** MODULE MESSAGE Q *** */
sysMessageQId msgQIdApp;
    /* *** MODULE MESSAGES *** */
    //@ref "APP_MESSAGES.h"

/* ******* STATIC FUNCTIONS ******* */
static SN_STATUS sSN_SYSTEM_Init(void);

/**
*  @brief main
*/
int main(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;



    /* SYSTEM INIT */
    SN_SYS_ERROR_SystemLog("==>SYSTEM INIT");

    retStatus = sSN_SYSTEM_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "SYSTEM INIT FAILD.");

    /* MODULE INIT */
    SN_SYS_ERROR_SystemLog("==>MODULE INIT");

    /* NEXTION DISPLAY INIT */
    retStatus = SN_MODULE_DISPLAY_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "NEXTION DISPLAY INIT FAILD.");

    /* FILE SYSTEM INIT */
    retStatus = SN_MODULE_FILE_SYSTEM_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "FILE SYSTEM INIT FAILD.");

    /* 3D PRINTER INIT ( IMAGE VIEWER INIT ) */
    retStatus = SN_MODULE_3D_PRINTER_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "3D PRINTER INIT FAILD.");
    
    SN_MODULE_DISPLAY_BootProgressUpdate(70, "Module Loading Done...");
    SN_SYS_TIMER_Delay(1000);

    SN_MODULE_DISPLAY_BootProgressUpdate(100, "Start System Service...");
    SN_SYS_TIMER_Delay(1000);

    /* APP INIT */
    retStatus = APP_Init();
    SN_SYS_ERROR_StatusCheck(retStatus,"APP INIT FAILD.");

    /* APP main() */
    while(true)
    {
        evt = SN_SYS_MESSAGE_Q_Get(msgQIdApp);

        switch(evt.evt_id)
        {
            case APP_EVT_ID_3D_PRINTER:
            case APP_EVT_ID_DISPLAY:
            case APP_EVT_ID_FILE_SYSTEM:
            case APP_EVT_ID_IMAGE_VIEWER:
                APP_Main(evt);
                break;

            default:
                SN_SYS_ERROR_StatusCheck(SN_STATUS_UNKNOWN_MESSAGE, "UNKNOWN APP MESSAGE_ID.");
                break;
        }
    }

}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sSN_SYSTEM_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /** APP Message Q Init **/
    SN_SYS_ERROR_SystemLog("SYSTEM INIT => MESSAGE QUEUE.");
    msgQIdApp = SN_SYS_MESSAGE_Q_Init();

    /** Timer Init **/
    SN_SYS_ERROR_SystemLog("SYSTEM INIT => TIMER.");
    SN_SYS_TIMER_Init();

    /** Serial Init **/
    SN_SYS_ERROR_SystemLog("SYSTEM INIT => SERIAL.");
    SN_SYS_SERIAL_Init();

    return retStatus;
}

SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = SN_SYS_MESSAGE_Q_Put(msgQIdApp, evtId, evtMessage);

    return retStatus;
}
