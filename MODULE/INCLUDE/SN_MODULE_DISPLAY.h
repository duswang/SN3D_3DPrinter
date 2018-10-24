/**
 * @file SN_MODULE_DISPLAY.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup MODULE_DISPLAY Display
 * @ingroup MODULE
 * @brief Display Module Functions.
 * @{
 */

#ifndef SN_MODUEL_DISPLAY
#define SN_MODUEL_DISPLAY

/** @name Display Module
 *  @brief Description of Display Module Init and Uninit funtions.
 *////@{

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_Init(void);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_Uninit(void);

///@}

/** @name Display Module :: Page Change
 *  @brief
 *////@{

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t page);

///@}

/** @name Display Module :: Print Page Info
 *  @brief
 *////@{

/** @brief
 *
 *  @param fileName
 *  @param optionName
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoInit(const char* fileName, const char* optionName);


/** @brief
 *
 *  @param slice
 *  @param targetSlice
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice);

///@}

/** @name Display Module :: Print Page Time Info
 *  @brief
 *////@{

/** @brief
 *
 *  @param sec
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerInit(uint32_t sec);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerPause(void);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerResume(void);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerStop(void);

///@}

#endif /* SN_MODUEL_DISPLAY */
/**@}*/
