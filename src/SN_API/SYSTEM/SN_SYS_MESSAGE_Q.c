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
SN_STATUS SN_SYS_MessageQInit(sysMessageQId *msgQId)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    msgQId->keyId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    msgQId->mtype = msgQId->keyId;

    if(msgQId->keyId == -1)
    {
        retStatus = SN_STATUS_NOT_INITIALIZED;
    }

    return retStatus;
}

SN_STATUS SN_SYS_MessageQRemove(sysMessageQId *msgQId)
{
    msgctl(msgQId->keyId, IPC_RMID, 0);

    return SN_STATUS_OK;
}

SN_STATUS SN_SYS_MessagePut(sysMessageQId *msgQId, event_id_t evtId, event_msg_t evtMessage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(msgQId == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    msgQId->mevt.evt_id   = evtId;
    msgQId->mevt.evt_msg  = evtMessage;

    if((msgsnd(msgQId->keyId, (void *)msgQId, sizeof(sysMessageQId), IPC_NOWAIT) == -1))
    {
        perror("msgsnd error : ");
        return SN_STATUS_NOT_OK;
    }

    return retStatus;
}


general_evt_t SN_SYS_MessageGet(sysMessageQId *msgQId)
{
    if(msgQId == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Message Q id is not Initialized.");
    }

    if((msgrcv(msgQId->keyId, (void *)msgQId, sizeof(sysMessageQId), msgQId->mtype, 0) == -1))
    {
            perror("msgrcv error : ");
    }

    return msgQId->mevt;
}

