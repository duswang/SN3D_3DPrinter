/*
 * SYS_MESSAGE_Q.c
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
int SN_SYS_MessageQInit(sysMessageQId *msgQId)
{

    msgQId->keyId = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    msgQId->mtype = msgQId->keyId;

    if(msgQId->keyId == -1)
    {
        //@DEBUG
        perror("msgget error :");
        exit(1);
    }

    return 0;
}

int SN_SYS_MessageQRemove(sysMessageQId *msgQId)
{
    msgctl(msgQId->keyId, IPC_RMID, 0);

    return 0;
}

int SN_SYS_MessagePut(sysMessageQId *msgQId, event_id_t evtId, event_msg_t evtMessage)
{
    msgQId->mevt.evt_id   = evtId;
    msgQId->mevt.evt_msg  = evtMessage;

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
        if (errno == EINTR)
        {
            msgQId->mevt.evt_id = APP_EVT_ID_IGNORE;
        }
        else
        {
            perror("msgrcv error : ");
            exit(0);
        }
    }

    return msgQId->mevt;
}

