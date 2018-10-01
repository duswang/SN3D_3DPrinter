/*
 * APP_WAITING.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "APPS.h"

int APP_WAITING_EvtHdlr(uint32_t evtCode)
{
    switch(APP_EVENT_TYPE(evtCode))
    {
        case 0:
            switch(APP_EVENT_VALUE(evtCode))
            {
                case 0:
                    break;
            }
            break;
        default:
            break;
    }
    return 0;
}
int APP_STATE_EnterStateWaiting(void)
{
    return 0;
}




