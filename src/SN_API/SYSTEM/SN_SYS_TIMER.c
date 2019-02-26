/**
 * @file SN_SYS_TIMER.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @note signal timer => thread timer :: 16 nov 2018.
 */

#include "SN_API.h"
#include "SN_SYS_TIMER.h"

/* ******* STATIC DEFINE ******* */

/* ******* SYSTEM DEFINE ******* */
/* *** SYSTEM THREAD *** */
pthread_mutex_t ptmTimer = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptTimer;

/* ******* GLOBAL VARIABLE ******* */
static int guiNumTSR;
static sysTimerQ_t aTSR[MAX_NUM_OF_TSR];

/* ******* STATIC FUNCTIONS ******* */
/* *** THREAD *** */
static void* sTimerThread();

/* *** UTIL *** */
static long long sDiffTick(const struct timespec startTick, const struct timespec endTick);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_SYS_TIMER_Init(void)
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
        aTSR[idxTSR].uniqueCode = UNALLOCATED_TSR_ID;
    }

    if (pthread_mutex_init(&ptmTimer, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if((retStatus = pthread_create(&ptTimer, NULL, sTimerThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retStatus);
        fflush(stdout);
    }

    return retStatus;
}

SN_STATUS SN_SYS_TIMER_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_SYS_TIMER_Create(sysTimerId_t* pIdTSR, uint32_t msDuration, void* pfTSR)
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
        if(!aTSR[idxTSR].isOccupied)
        {
            clock_gettime(CLOCK_REALTIME, &aTSR[idxTSR].tickRequested);

            aTSR[idxTSR].isOccupied    = true;
            aTSR[idxTSR].msDuration    = msDuration;
            aTSR[idxTSR].pfTSRCallBack = pfTSR;
            aTSR[idxTSR].uniqueCode    = (uint32_t)aTSR[idxTSR].tickRequested.tv_nsec;

            *pIdTSR = aTSR[idxTSR].uniqueCode;
            guiNumTSR++;

            break;
        }
    }

    pthread_mutex_unlock(&ptmTimer);

    if(idxTSR >= MAX_NUM_OF_TSR)
    {
        *pIdTSR = UNALLOCATED_TSR_ID;
        retStatus = SN_STATUS_RESOURCE_NOT_AVAILABLE;
    }

    return retStatus;
}

SN_STATUS SN_SYS_TIMER_Cancel(sysTimerId_t* pIdTSR)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    uint8_t idxTSR = 0;

    if(pIdTSR == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    pthread_mutex_lock(&ptmTimer);

    if(*pIdTSR != UNALLOCATED_TSR_ID)
    {
        for(idxTSR = 0; idxTSR < MAX_NUM_OF_TSR; idxTSR++)
        {
            if (aTSR[idxTSR].isOccupied && aTSR[idxTSR].uniqueCode == *pIdTSR)
            {

                aTSR[idxTSR].isOccupied = false;
                aTSR[idxTSR].tickRequested.tv_sec = 0;
                aTSR[idxTSR].tickRequested.tv_nsec = 0;
                aTSR[idxTSR].msDuration = 0;
                aTSR[idxTSR].pfTSRCallBack = NULL;
                aTSR[idxTSR].uniqueCode = UNALLOCATED_TSR_ID;

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
SN_STATUS SN_SYS_TIMER_Delay(uint32_t msec)
{
    SN_STATUS retStatus = SN_STATUS_OK;

#if(IMAGE_VIEWER_USE_SDL)
    SDL_Delay(msec);
#else
    usleep(msec * 1000);
#endif

    return retStatus;
}

struct timespec SN_SYS_TIMER_GetTick(void)
{
    struct timespec tickNow;

    clock_gettime(CLOCK_REALTIME, &tickNow);

    return tickNow;
}

long long SN_SYS_TIMER_DiffTick(struct timespec prevTick, struct timespec currentTick)
{
    const long long NANOS = 1000000000LL;
    long long retDiff = 0;

    retDiff = (NANOS * (currentTick.tv_sec - prevTick.tv_sec)) + (currentTick.tv_nsec - prevTick.tv_nsec);

    retDiff /= 1000000;

    return retDiff;
}

static void* sTimerThread()
{
    uint8_t idxTSR;
    struct timespec tickNow;

    while(true)
    {
        for (idxTSR = 0; idxTSR < MAX_NUM_OF_TSR; idxTSR++)
        {
            pthread_mutex_lock(&ptmTimer);

            clock_gettime(CLOCK_REALTIME, &tickNow);

            if ((aTSR[idxTSR].isOccupied) && (aTSR[idxTSR].pfTSRCallBack != NULL) &&
                (SN_SYS_TIMER_DiffTick(aTSR[idxTSR].tickRequested, tickNow) >= (aTSR[idxTSR].msDuration)))
            {
                aTSR[idxTSR].isOccupied = false;
                aTSR[idxTSR].tickRequested.tv_sec = 0;
                aTSR[idxTSR].tickRequested.tv_nsec = 0;
                aTSR[idxTSR].msDuration = 0;
                aTSR[idxTSR].uniqueCode = UNALLOCATED_TSR_ID;
                if(guiNumTSR != 0)
                {
                    guiNumTSR--;
                }

                (void)(aTSR[idxTSR].pfTSRCallBack)();

                aTSR[idxTSR].pfTSRCallBack = NULL;
            }

            pthread_mutex_unlock(&ptmTimer);
        }

        SN_SYS_TIMER_Delay(10);
    }
    return NULL;
}
