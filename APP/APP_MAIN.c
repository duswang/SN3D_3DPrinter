/*
 * APP_MAIN.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "APPS.h"

int APP_Main(general_evt_t evt)
{
    uint16_t evtId = 0;
    uint16_t evtValue = 0;

    //@DEBUG
    printf("get APP Message.\n");
    fflush(stdout);

    switch(evtId)
    {
        case 0:
            APP_WAITING_EvtHdlr(evtValue);
            break;
        case 1:
            APP_CONTROL_EvtHdlr(evtValue);
            break;
        default:
            break;
    }
    return 0;
}

void APP_Init(void)
{

}
