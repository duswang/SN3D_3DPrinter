/*
 * SN_MODUEL_DISPLAY.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#ifndef SN_MODUEL_DISPLAY
#define SN_MODUEL_DISPLAY

/**** MODULE ****/
extern SN_STATUS SN_MODULE_DISPLAY_Init(void);
extern SN_STATUS SN_MODULE_DISPLAY_Uninit(void);

/**** PAGE ****/
extern SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state);


/**** FILE SYSTEM ****/
extern SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t page);

/**** PRINITING INFO ****/
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoInit(const char* fileName, const char* optionName);
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice);
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimeUpdate(const char* timeStr);

/**** PRINITING TIME INFO****/
SN_STATUS SN_MODULE_DISPLAY_PrintingTimerInit(uint32_t sec);
SN_STATUS SN_MODULE_DISPLAY_PrintingTimerPause(void);
SN_STATUS SN_MODULE_DISPLAY_PrintingTimerResume(void);
SN_STATUS SN_MODULE_DISPLAY_PrintingTimerStop(void);




#endif /* SN_MODUEL_DISPLAY */
