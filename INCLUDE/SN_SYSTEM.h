/*
 * SN_SYSTEM.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef INCLUDE_SN_SYSTEM_H_
#define INCLUDE_SN_SYSTEM_H_

#include "SN_SYS_SERIAL_COMM.h"
#include "SN_SYS_MESSAGE_Q.h"

extern int SN_SYSTEM_SendApplMessage(event_id evtId, uint32_t evtValue);

#endif /* SN_SYSTEM */
