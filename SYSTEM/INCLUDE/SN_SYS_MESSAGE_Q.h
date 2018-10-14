/*
 * SN_SYS_MESSAGE_Q.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef SN_SYS_MESSAGE_Q_H_
#define SN_SYS_MESSAGE_Q_H_

typedef uint32_t    event_id_t;
typedef uint32_t    event_msg_t;

typedef struct {
    event_id_t evt_id;
    uint16_t   evt_len;
} general_evt_header_t;

typedef struct {
    event_id_t  evt_id;
    event_msg_t evt_msg;
} general_evt_t;

typedef struct message {
    long mtype;
    general_evt_t mevt;
    key_t keyId;
} sysMessageQId;


extern int           SN_SYS_MessageQInit(sysMessageQId *msgQId);
extern int           SN_SYS_MessageQRemove(sysMessageQId *msgQId);
extern int           SN_SYS_MessagePut(sysMessageQId *msgQId, event_id_t evtId, event_msg_t evtMessage);
extern general_evt_t SN_SYS_MessageGet(sysMessageQId *msgQId);

/******* *******/
typedef struct  {
  uint32_t                 status;     ///< status code: event or error information
  union  {
    uint32_t                    v;     ///< message as 32-bit value
    void                       *p;     ///< message or mail as void pointer
    int32_t               signals;     ///< signal flags
  } value;                             ///< event value
  union  {
    //sysMailQId             mail_id;     ///< mail id obtained by \ref osMailCreate
    sysMessageQId       message_id;     ///< message id obtained by \ref osMessageCreate
  } def;                               ///< event definition
} sysEvent;

typedef struct mailbox_id{
    int iFrom;          /* who sent message (0 .. number-of-threads) */
    int type;           /* its type */
    int num;            /* number of items in payload (0 .. MAXPAYLOAD) */
    void* payload;      /* data */
}sysMailboxId;

typedef struct sys_mailbox_def  {
  uint32_t                queue_sz;    ///< number of elements in the queue
  uint32_t                item_sz;
  void                       *pool;    ///< memory array for messages
} sysMailboxDef_t;

#define sysMailboxDef(name, queue_sz, type) \
uint32_t sys_mailbox_q_##name[4+(queue_sz)] = {0, }; \
const sysMailboxDef_t sys_mailgox_def##name = \
{(queue_sz), (os_mailbox_q_##name) }

#define sysMailbox(name) \
sys_mailbox_def##name

extern sysMailboxId* SN_SYS_MailboxCreate(const sysMailboxDef_t *queue_def);
extern int           SN_SYS_MailboxPut(const sysMailboxId queue_id, uint32_t info, uint32_t millisec);
extern void         *SN_SYS_MailboxGet(const sysMailboxId queue_id, uint32_t millisec);

#endif /* INCLUDE_SN_SYS_MESSAGE_Q_H_ */
