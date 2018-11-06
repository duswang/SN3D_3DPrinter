/**
 * @file SN_SYS_ERROR.h
 * @author Bato
 * @date 16 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_ERROR Error Handler
 * @ingroup SYSTEM
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

/** @name Error System
 *  @brief
 */

/** @name System Error Handler
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
extern void SN_SYS_ErrorCheck(SN_STATUS errorStatus, const char* errorMessage, const char* _file, const char* _func, const int _line);


/** @brief
 *
 *  @param log
 *
 *  @return SN_STATUS
 *  @note
 */
extern void SN_SYS_Log(const char* log);


/** @def SN_SYS_ERROR_CHECK(error, msg)
 *  @brief
 *
 *  @param error
 *  @param msg
 *
 *  @return SN_STATUS
 *  @note
 */
#define SN_SYS_ERROR_CHECK(error, msg) \
SN_SYS_ErrorCheck((error), (msg), __FILE__, __FUNCTION__, __LINE__)

///@}

#endif /* SN_ERROR_H_ */
/**@}*/
