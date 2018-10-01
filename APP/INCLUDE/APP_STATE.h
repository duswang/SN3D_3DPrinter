/*
 * APP_STATE.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef APP_APP_STATE_H_
#define APP_APP_STATE_H_

typedef enum
{
    APP_STATE_WAITING = 0,
    APP_STATE_CONTROL,
    APP_STATE_FILE_SELECT,
    APP_STATE_PRINTING,
    APP_STATE_PAUSE,
    APP_STATE_SETUP,
    APP_STATE_INIT,
    N_APP_STATE
} app_state_t;




#endif /* APP_APP_STATE_H_ */
