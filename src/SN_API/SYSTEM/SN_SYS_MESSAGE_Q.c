/**
 * @file SN_SYS_MESSAGE_Q.c
 * @author Bato
 * @date 16 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_SYS_MESSAGE_Q.h"

/* ******* SYSTEM DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
sysMessageQId SN_SYS_MessageQInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return msgget(IPC_PRIVATE, IPC_CREAT | 0666);
}

SN_STATUS SN_SYS_MessageQRemove(sysMessageQId msgQId)
{
    msgQId = msgctl(msgQId, IPC_RMID, 0);

    /* @TODO */
    return SN_STATUS_OK;
}

SN_STATUS SN_SYS_MessagePut(sysMessageQId msgQId, event_id_t evtId, event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    sysMessageBuffer msgBuffer;

    msgBuffer.mtype = msgQId;
    msgBuffer.mevt.evt_id   = evtId;
    msgBuffer.mevt.evt_msg  = evtMessage;

    if((msgsnd(msgQId, (void *)&msgBuffer, sizeof(msgBuffer) - sizeof(long), IPC_NOWAIT) == -1))
    {
        perror("msgsnd error : ");
        return SN_STATUS_NOT_OK;
    }

    return retStatus;
}


general_evt_t SN_SYS_MessageGet(sysMessageQId msgQId)
{
    sysMessageBuffer msgBuffer;

    if((msgrcv(msgQId, (void *)&msgBuffer, sizeof(msgBuffer) - sizeof(long), msgQId, 0) == -1))
    {
            perror("msgrcv error : ");
    }

    return msgBuffer.mevt;
}

