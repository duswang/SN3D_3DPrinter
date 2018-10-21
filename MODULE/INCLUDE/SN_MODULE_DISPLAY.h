/**
 * @file SN_MODUEL_DISPLAY.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#ifndef SN_MODUEL_DISPLAY
#define SN_MODUEL_DISPLAY

/**** MODULE ****/
/**
*@brief
*@details
*@param args
*@return
*
*@bug
*@todo
*@exception
*
*@see
*@see
*/
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
