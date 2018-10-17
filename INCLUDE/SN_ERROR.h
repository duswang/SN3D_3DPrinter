/*
 * SN_ERROR.h
 *
 *  Created on: Oct 16, 2018
 *      Author: pi
 */

#ifndef SN_ERROR_H_
#define SN_ERROR_H_

#define SN_STATUS           uint16_t

#define SN_STATUS_BASE      0

#define SN_STATUS_OK                        (SN_STATUS_BASE + 0)
#define SN_STATUS_TIMEOUT                   (SN_STATUS_BASE + 1)
#define SN_STATUS_INVALID_PARAM             (SN_STATUS_BASE + 2)
#define SN_STATUS_NOT_SUPPORTED             (SN_STATUS_BASE + 3)
#define SN_STATUS_OUT_OF_MEM                (SN_STATUS_BASE + 4)
#define SN_STATUS_NOT_INITIALIZED           (SN_STATUS_BASE + 5)
#define SN_STATUS_ALREADY_INITIALIZED       (SN_STATUS_BASE + 6)
#define SN_STATUS_RESOURCE_NOT_AVAILABLE    (SN_STATUS_BASE + 7)
#define SN_STATUS_NOT_OK                    (SN_STATUS_BASE + 8)       /* Unknown or undefined reason */



#endif /* SN_ERROR_H_ */
