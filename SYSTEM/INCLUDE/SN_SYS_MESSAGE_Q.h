/*
 * SN_SYS_MESSAGE_Q.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef INCLUDE_SN_SYS_MESSAGE_Q_H_
#define INCLUDE_SN_SYS_MESSAGE_Q_H_

typedef uint16_t    event_id;

typedef struct{
    event_id evt_id;
    uint16_t evt_len;
} general_evt_header_t;

typedef struct{
    event_id            evt_id;
    uint32_t            value;
} general_evt_t;

typedef struct message
{
    long mtype;
    general_evt_t mevt;
    key_t keyId;
} sysMessageQId;

/* Types, Structures, Variables */
/* general application event to application task */
typedef enum
{
    EVT_ID_APP = 1,
    EVT_ID_MODUEL_3D_PRINTER,
    EVT_ID_MODUEL_DISPLAY,
    EVT_ID_MODUEL_FILE_SYSTEM,
    EVT_ID_MODUEL_IMAGE_VIEWR,
    N_EVT_ID,
} event_id_t;

extern key_t SN_SYS_MessageInit(sysMessageQId *msgQId, key_t keyID, int32_t mtype);
extern int SN_SYS_MessagePut(sysMessageQId *msgQId, event_id evtId, uint32_t value);
extern general_evt_t SN_SYS_MessageGet(sysMessageQId *msgQId);

#endif /* INCLUDE_SN_SYS_MESSAGE_Q_H_ */
