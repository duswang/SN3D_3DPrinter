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

/**** PRINITING ****/
extern SN_STATUS SN_MODULE_DISPLAY_PrintingUpdate(void);

#endif /* SN_MODUEL_DISPLAY */
