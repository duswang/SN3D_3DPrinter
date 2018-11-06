/**
 * @file APP_STATE.h
 * @author Bato
 * @date 2 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#ifndef APP_APP_STATE_H_
#define APP_APP_STATE_H_

typedef enum
{
    APP_STATE_WAITING       = 0x01,
    APP_STATE_CONTROL       = 0x02,
    APP_STATE_FILE_SELECT   = 0x03,
    APP_STATE_PRINTING      = 0x04,
    APP_STATE_PAUSE         = 0x05,
    APP_STATE_SETUP         = 0x06,
    APP_STATE_INIT          = 0x07,
    N_APP_STATE
} app_state_t;


extern SN_STATUS APP_STATE_EnterStateWaiting(void);
extern SN_STATUS APP_STATE_EnterStateControl(void);
extern SN_STATUS APP_STATE_EnterStateFileSelect(void);
extern SN_STATUS APP_STATE_EnterStatePrinting(void);
extern SN_STATUS APP_STATE_EnterStatePause(void);
extern SN_STATUS APP_STATE_EnterStateInit(void);

#endif /* APP_APP_STATE_H_ */
