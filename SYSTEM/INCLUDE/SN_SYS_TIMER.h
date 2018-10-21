/*
 * SN_SYS_TIMER.h
 *
 *  Created on: Sep 27, 2018
 *      Author: pi
 */
#ifndef SN_SYS_TIMER_H_
#define SN_SYS_TIMER_H_

/** Static Define **/
#define MAX_NUM_OF_TSR 32
#define UNALLOCATED_TSR_ID          (0xFFFFFFFF)


typedef struct sys_timer_id {
    bool    isOccupied;
    struct timespec tickRequested;
    long     msDuration;
    uint32_t uniqueCode;
    void* (*pfTSRCallBack)();
} sysTimerQ_t;

typedef uint32_t sysTimerId_t;

extern SN_STATUS SN_SYS_TimerInit(void);
extern SN_STATUS SN_SYS_TimerCreate(sysTimerId_t* pIdTSR, unsigned int msDuration, void* pfTSR);
extern SN_STATUS SN_SYS_TimerCancle(sysTimerId_t* pIdTSR);

extern SN_STATUS SN_SYS_Delay(uint32_t msec);

#endif /* SN_SYS_TIMER_H_ */
