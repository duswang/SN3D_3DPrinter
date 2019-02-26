/**
 * @file SN_SYS_ERROR.h
 * @author Bato
 * @date 16 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup BOOTLOADER_SYSTEM_ERROR Bootloader Error Handler
 * @ingroup BOOTLOADER_SYSTEM
 * @brief System Error Functions.
 * @{
 */

#ifndef SN_ERROR_H_
#define SN_ERROR_H_

#define SN_STATUS_BASE      0

/** @enum SN_STATUS */
typedef enum sn_status {
    SN_STATUS_OK                           = (SN_STATUS_BASE + 0),  /**< 0 */
    SN_STATUS_TIMEOUT,                                              /**< 1 */
    SN_STATUS_INVALID_PARAM,                                        /**< 2 */
    SN_STATUS_NOT_SUPPORTED,                                        /**< 3 */
    SN_STATUS_UNKNOWN_MESSAGE,                                      /**< 4 */
    SN_STATUS_OUT_OF_MEM,                                           /**< 5 */
    SN_STATUS_NOT_INITIALIZED,                                      /**< 6 */
    SN_STATUS_ALREADY_INITIALIZED,                                  /**< 7 */
    SN_STATUS_RESOURCE_NOT_AVAILABLE,                               /**< 8 */
    SN_STATUS_SDL_ERROR,                                            /**< 9 */
    SN_STATUS_NOT_OK                                                /**< 10 */
} SN_STATUS;                                                        /**< SN3D Error Code */

/** @typedef ERROR_T */
typedef int        ERROR_T;


/*************************************************************
 * @name System Error Handler
 *  Description of Message Queue Init and Uninit funtions.
 *////@{

/** @brief
 *
 *  @param errorStatus
 *  @param errorMeesage
 *  @param _file
 *  @param _func
 *  @param _line
 *
 *  @return SN_STATUS
 *  @note
 */
extern void SN_SYS_ERROR_StatusCheck_Inline(SN_STATUS errorStatus, const char* errorMessage, const char* _file, const char* _func, const int _line);


#define SN_SYS_ERROR_StatusCheck(errorStatus, errorMessage) \
{SN_SYS_ERROR_StatusCheck_Inline(errorStatus, errorMessage, __FILE__, __FUNCTION__, __LINE__);}

/** @brief
 *
 *  @param log
 *
 *  @return SN_STATUS
 *  @note
 */
extern void SN_SYS_ERROR_SystemLog(const char* log);

/*************************************************************@}*/

#endif /* SN_ERROR_H_ */
/**@}*/
