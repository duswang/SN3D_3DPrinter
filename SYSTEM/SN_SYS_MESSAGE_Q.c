/*
 * SN_SYS_MESSAGE_Q.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_MESSAGE_Q.h"

/** Static Define **/

/** System **/

/** Global Variables **/

/** Static Funtions **/
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
    sysMessageQId buffer;

    if(msgQId == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Message Q id is not Initialized.");
    }

    if((msgrcv(msgQId->keyId, (void *)msgQId, sizeof(sysMessageQId), msgQId->mtype, 0) == -1))
    {
        if (errno == EINTR)
        {
            msgQId->mevt.evt_id = APP_EVT_ID_IGNORE;
        }
        else
        {
            perror("msgrcv error : ");
        }
    }

    return msgQId->mevt;
}

