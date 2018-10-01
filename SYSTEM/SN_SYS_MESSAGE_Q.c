/*
 * SYS_MESSAGE_Q.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_MESSAGE_Q.h"


key_t SN_SYS_MessageInit(sysMessageQId *msgQId, key_t keyId, int32_t mtype)
{
    msgQId->mtype = mtype;

    msgQId->keyId = msgget(keyId, IPC_CREAT | 0666);

    if(msgQId->keyId == -1)
    {
        //@DEBUG
        perror("msgget error :");
        exit(1);
    }

    return msgQId->keyId;
}
int SN_SYS_MessagePut(sysMessageQId *msgQId, event_id evtId, uint32_t value)
{
    msgQId->mevt.evt_id = evtId;
    msgQId->mevt.value  = value;

    if((msgsnd(msgQId->keyId, (void *)msgQId, sizeof(sysMessageQId), IPC_NOWAIT) == -1))
    {
        perror("msgsnd error : ");
        exit(0);
    }

    return 0;
}

general_evt_t SN_SYS_MessageGet(sysMessageQId *msgQId)
{
    sysMessageQId buffer;

    if((msgrcv(msgQId->keyId, (void *)msgQId, sizeof(sysMessageQId), msgQId->mtype, 0) == -1))
    {
        perror("msgrcv error : ");
        exit(0);
    }

    return msgQId->mevt;
}


