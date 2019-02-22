/**
 * @file SN_SYS_MESSAGE_Q.h
 * @author Bato
 * @date 16 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_MESSAGE_Q Message Q
 * @ingroup SYSTEM
 * @brief System Message Q Functions.
 * @{
 */

#ifndef SN_SYS_MESSAGE_Q_H_
#define SN_SYS_MESSAGE_Q_H_

typedef uint32_t    event_id_t;
typedef uint32_t    event_msg_t;
typedef int         sysMessageQId;

typedef struct general_event_header {
    event_id_t evt_id;
    uint16_t   evt_len;
} general_evt_header_t;

typedef struct general_event {
    event_id_t  evt_id;
    event_msg_t evt_msg;
} general_evt_t;

typedef struct sys_message_buffer {
    long mtype;
    general_evt_t mevt;
} sysMessageBuffer;

/*************************************************************
 * @name System Message Queue
 *  Description of Message Queue Init and Uninit funtions.
 *////@{

/** @brief
 *
 *  @param msgQId
 *
 *  @return SN_STATUS
 *
 *  @bug First message Q Init is not Working.
 *  @note
 */
extern sysMessageQId SN_SYS_MessageQInit(void);

/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS     SN_SYS_MessageQRemove(sysMessageQId msgQId);

/*************************************************************@}*/

/*************************************************************
 *  @name System Message Queue :: Send & Receive Message
 *  Description of Message Queue Init and Uninit funtions.
 *////@{

/** @brief
 *
 *  @param msgQId
 *  @param evtId
 *  @param evtMessage
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS     SN_SYS_MessagePut(sysMessageQId msgQId, event_id_t evtId, event_msg_t evtMessage);

/** @brief
 *
 *  @param msgQId
 *
 *  @return general_evt_t
 *
 *  @note
 */
extern general_evt_t SN_SYS_MessageGet(sysMessageQId msgQId);

/*************************************************************@}*/


#endif /* INCLUDE_SN_SYS_MESSAGE_Q_H_ */
/**@}*/
