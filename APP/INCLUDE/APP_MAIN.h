/**
 * @file APP_MAIN.h
 * @author Bato
 * @date 24 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */
#ifndef APP_INCLUDE_APP_MAIN_H_
#define APP_INCLUDE_APP_MAIN_H_

extern SN_STATUS    APP_Main(general_evt_t evt);
extern app_state_t  APP_GetAppState(void);
extern void         APP_SetAppState(app_state_t tAppState);

#endif /* APP_INCLUDE_APP_MAIN_H_ */
