/*
 * SN_SYS_TIMER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_TIMER.h"

/** System **/
/* Thread */
pthread_mutex_t ptmTimer = PTHREAD_MUTEX_INITIALIZER;

/** Global Variables **/
static int guiNumTSR;
static sysTimerQ_t aTSR[MAX_NUM_OF_TSR];

/** Global Variables **/

/** Static Funtions **/
/* callback */
static void sCallBackTSR();

/* local */
static void sTimerStart(void);
static void sTimerStop(void);

/* util */
static long long sDiffTick(const struct timespec startTick, const struct timespec endTick);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_SYS_TimerInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    int idxTSR = 0;

    guiNumTSR = 0;

    for (idxTSR = 0; idxTSR < MAX_NUM_OF_TSR; idxTSR++)
    {
        aTSR[idxTSR].isOccupied = false;
        aTSR[idxTSR].tickRequested.tv_sec = 0;
        aTSR[idxTSR].tickRequested.tv_nsec = 0;
        aTSR[idxTSR].msDuration = 0;
        aTSR[idxTSR].pfTSRCallBack = NULL;
        aTSR[idxTSR].uniqueCode = 0;
    }

    if (pthread_mutex_init(&ptmTimer, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Timer Mutex Init Faild.");
    }

    return retStatus;
}

SN_STATUS SN_SYS_TimerCreate(sysTimerId_t* pIdTSR, unsigned int msDuration, void* pfTSR)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint8_t idxTSR = 0;

    if(pIdTSR == NULL)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }
    pthread_mutex_lock(&ptmTimer);

    for(idxTSR = 0; idxTSR < MAX_NUM_OF_TSR; idxTSR++)
    {
        if (!aTSR[idxTSR].isOccupied)
        {
            aTSR[idxTSR].isOccupied    = true;
            clock_gettime(CLOCK_REALTIME, &aTSR[idxTSR].tickRequested);
            aTSR[idxTSR].msDuration    = msDuration;
            aTSR[idxTSR].pfTSRCallBack = pfTSR;

            srand(aTSR[idxTSR].tickRequested.tv_nsec + idxTSR);

            aTSR[idxTSR].uniqueCode = (uint32_t)rand();

            *pIdTSR = aTSR[idxTSR].uniqueCode;
            guiNumTSR++;

            if (guiNumTSR == 1)
            {
                sTimerStart();
            }
            break;
        }
    }

    pthread_mutex_unlock(&ptmTimer);

    if(idxTSR >= MAX_NUM_OF_TSR)
    {
        *pIdTSR = 0xFF;
        retStatus = SN_STATUS_RESOURCE_NOT_AVAILABLE;
    }

    return retStatus;
}

SN_STATUS SN_SYS_TimerCancle(sysTimerId_t* pIdTSR)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint8_t i = 0;

    if(pIdTSR == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    pthread_mutex_lock(&ptmTimer);

    if(*pIdTSR != UNALLOCATED_TSR_ID)
    {
        for(i = 0; i < MAX_NUM_OF_TSR; i++)
        {
            if (aTSR[i].isOccupied && aTSR[i].uniqueCode == *pIdTSR)
            {
                aTSR[i].isOccupied = false;
                aTSR[i].tickRequested.tv_sec = 0;
                aTSR[i].tickRequested.tv_nsec = 0;
                aTSR[i].msDuration = 0;
                aTSR[i].pfTSRCallBack = NULL;
                aTSR[i].uniqueCode = 0;

                if(guiNumTSR != 0)
                {
                    guiNumTSR--;
                }
            }
        }
        (*pIdTSR) = UNALLOCATED_TSR_ID;
    }

    pthread_mutex_unlock(&ptmTimer);

    return retStatus;
}
SN_STATUS SN_SYS_Delay(uint32_t msec)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SDL_Delay(msec);

    return retStatus;
}

static void sCallBackTSR()
{
    uint8_t idxTSR;
    struct timespec tickNow;

    pthread_mutex_lock(&ptmTimer);

    for (idxTSR = 0; idxTSR < MAX_NUM_OF_TSR; idxTSR++)
    {
        clock_gettime(CLOCK_REALTIME, &tickNow);
        if ((aTSR[idxTSR].isOccupied) && (aTSR[idxTSR].pfTSRCallBack != NULL) &&
            (sDiffTick(aTSR[idxTSR].tickRequested, tickNow) >= ((aTSR[idxTSR].msDuration))))
    	{

            (void)(aTSR[idxTSR].pfTSRCallBack)();

            aTSR[idxTSR].isOccupied = false;
            aTSR[idxTSR].tickRequested.tv_sec = 0;
            aTSR[idxTSR].tickRequested.tv_nsec = 0;
            aTSR[idxTSR].msDuration = 0;
            aTSR[idxTSR].uniqueCode = 0;
            if(guiNumTSR != 0)
            {
                guiNumTSR--;
            }
            aTSR[idxTSR].pfTSRCallBack = NULL;

        }
    }

    pthread_mutex_unlock(&ptmTimer);

    if (guiNumTSR == 0)
    {
        sTimerStop();
    }
}

static void sTimerStart(void)
{
    struct sigaction sa;
    struct itimerval timerId;

    /* Configure the timer to expire after 1 msec... */
    timerId.it_value.tv_sec = 0;
    timerId.it_value.tv_usec = 1000;

    /* ... and every 1 msec after that. */
    timerId.it_interval.tv_sec = 0;
    timerId.it_interval.tv_usec = 1000;

    if (signal(SIGALRM, (void (*)(int)) sCallBackTSR) == SIG_ERR) {
      perror("Unable to catch SIGALRM");
      exit(1);
    }

    setitimer(ITIMER_REAL, &timerId, NULL);
}

static void sTimerStop(void)
{
    struct sigaction sa;
    struct itimerval timerId;

    /* Configure the timer to expire after 250 msec... */
    timerId.it_value.tv_sec = 0;
    timerId.it_value.tv_usec = 0;

    /* ... and every 250 msec after that. */
    timerId.it_interval.tv_sec = 0;
    timerId.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &timerId, NULL);
}

static long long sDiffTick(const struct timespec startTick, const struct timespec endTick)
{
    const long long NANOS = 1000000000LL;
    long long retDiff = 0;

    retDiff = (NANOS * (endTick.tv_sec - startTick.tv_sec)) + (endTick.tv_nsec - startTick.tv_nsec);

    retDiff /= 1000000;

    return retDiff;
}
