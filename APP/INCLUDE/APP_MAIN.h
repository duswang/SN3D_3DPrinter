/*
 * APP_MAIN.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef APP_INCLUDE_APP_MAIN_H_
#define APP_INCLUDE_APP_MAIN_H_

extern SN_STATUS    APP_Main(general_evt_t evt);
extern app_state_t  APP_GetAppState(void);
extern void         APP_SetAppState(app_state_t tAppState);

#endif /* APP_INCLUDE_APP_MAIN_H_ */
