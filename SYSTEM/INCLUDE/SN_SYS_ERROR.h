/*
 * SN_ERROR.h
 *
 *  Created on: Oct 16, 2018
 *      Author: pi
 */

#ifndef SN_ERROR_H_
#define SN_ERROR_H_

#define SN_STATUS_BASE      0

typedef enum sn_error {
    SN_STATUS_OK                           = (SN_STATUS_BASE + 0),
    SN_STATUS_TIMEOUT,
    SN_STATUS_INVALID_PARAM,
    SN_STATUS_NOT_SUPPORTED,
    SN_STATUS_UNKNOWN_MESSAGE,
    SN_STATUS_OUT_OF_MEM,
    SN_STATUS_NOT_INITIALIZED,
    SN_STATUS_ALREADY_INITIALIZED,
    SN_STATUS_RESOURCE_NOT_AVAILABLE,
    SN_STATUS_SDL_ERROR,
    SN_STATUS_NOT_OK
} sn_error_t;

typedef sn_error_t SN_STATUS;
typedef int        ERROR_T;

/** Functions **/
extern void SN_SYS_ErrorCheck(SN_STATUS errorStatus, const char* errorMessage, const char* _file, const char* _func, const int _line);
extern void SN_SYS_Log(const char* log);

#define SN_SYS_ERROR_CHECK(error, msg) \
SN_SYS_ErrorCheck((error), (msg), __FILE__, __FUNCTION__, __LINE__)

#endif /* SN_ERROR_H_ */
