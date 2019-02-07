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

/*************************************************************
 * @name Display Module
 *  @brief Description of Display Module Init and Uninit funtions.
 *////@{

/** @brief Nextion Display Init with Serial Comm
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_Init(void);


/** @brief Nextion Display Uninit
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_Uninit(void);

/*************************************************************@}*/

/*************************************************************
 * @name Display Module :: Page Change
 *  @brief
 *////@{

/** @brief Nextoin Display Page Change
 *
 *  @param state - page state check header below ref link.
 *  @return SN_STATUS
 *
 *  @ref Reference => SN_API/SYSTEM/INCLUDE/SN_SYS_SERIAL_NEXTION.h
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state);

/*************************************************************@}*/

/*************************************************************
 * @name Display Module :: Boot Page
 *  @brief
 *////@{

/** @brief Boot ProgressBar Controller
 *
 *  @param progressValue - Current progress value.
 *  @param progressStr - Current progress information string.
 *  @return SN_STATUS
 *
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_BootProgressUpdate(uint32_t progressValue, const char* progressStr);

/*************************************************************@}*/

/*************************************************************
 * @name Display Module :: Waiting Page
 *  @brief
 *////@{

/** @brief Waiting Language Button
 *
 *  @param language_code - language code english or korean
 *  @return SN_STATUS
 *
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_WaitingLanguageUpdate(uint32_t language_code);

/*************************************************************@}*/


/*************************************************************
 * @name Display Module :: Print Page Info
 *  @brief
 *////@{

/** @brief Nextion Display Printing Page Info Init
 *
 *  @param fileName - Filename of string
 *  @param optionName - Optionname of string
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoInit(const char* fileName, const char* optionName);


/** @brief Nextion Display Printing Page Info Update
 *
 *  @param slice - Current slice index of number
 *  @param targetSlice - All slice of number
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingInfoUpdate(uint32_t slice, uint32_t targetSlice);

/*************************************************************@}*/

/*************************************************************
 * @name Display Module :: Print Page Time Info
 *  @brief
 *////@{

/** @brief Nextion Display Printing Page Timer Info Init
 *
 *  @param sec - ETA Time per sec
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerInit(uint32_t sec);

/** @brief Nextion Display Printing Page Timer Info Sync by Current Slice Info
 *
 *  @param sec - Synced Time per sec
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerSync(uint32_t sec);

/** @brief Nextoin Display Printing Page Timer Pause
 *
 *  @return SN_STATUS
 *
 *  @note This function called when printing pause event
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerPause(void);


/** @brief Nextoin Display Printing Page Timer Resume
 *
 *  @return SN_STATUS
 *
 *  @note This function called when printing resume event
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerResume(void);


/** @brief Nextoin Display Printing Page Timer Stop
 *
 *  @return SN_STATUS
 *
 *  @note This function called when printing stop event
 */
extern SN_STATUS SN_MODULE_DISPLAY_PrintingTimerStop(void);

/*************************************************************@}*/


/*************************************************************
 * @name Display Module :: Control Page
 *  @brief
 *////@{

/** @brief Nextion Display Control Page Current Z Position Update
 *
 *  @param mm - current z position
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_ControlZPosition(float mm);

/*************************************************************@}*/



/*************************************************************
 * @name Display Module :: File Select Page
 *  @brief
 *////@{

/** @brief Nextion Display File Select Page OptionMenu Update
 *
 *  @param optionIndex - Option index of showing Option Name
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_FileSelectOptionUpdate(uint32_t optionIndex);

/** @brief Nextion Display File Select Page FileList Update
 *
 *  @param pageIndexs - Page index of showing Files
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_FileSelectPageUpdate(uint32_t pageIndex);

/*************************************************************@}*/


/*************************************************************
 * @name Display Module :: Nextion GUI or Drawing Tool
 *  @brief
 *////@{

/** @brief Drawing Line on Nextion Display Screen
 *
 *
 *  @param startX
 *  @param startY
 *  @param endX
 *  @param endY
 *  @param color
 *  @return SN_STATUS
 *
 *  @note
 */
SN_STATUS SN_MODULE_DISPLAY_NextionDrawLine(int startX, int startY, int endX, int endY, int color);


/** @brief Drawing Dot on Nextion Display Screen
 *
 *  @param coorX
 *  @param coorY
 *  @param color
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_NextionDrawDot(int coorX, int coorY, int color);


/** @brief Drawing Fill on Nextion Display Screen
 *
 *  @param startX
 *  @param startY
 *  @param width
 *  @param height
 *  @param color
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_DISPLAY_NextionDrawFill(int startX, int startY, int width, int height, int color);


/*************************************************************@}*/

#endif /* SN_MODUEL_DISPLAY */
/**@}*/
