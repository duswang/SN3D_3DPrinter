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
pthread_mutex_t ptmTimer;
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
        printf("\n mutex init failed\n");
    }

    if((retStatus = pthread_create(&ptTimer, NULL, sTimerThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retStatus);
        fflush(stdout);
    }

    return retStatus;
}

SN_STATUS SN_SYS_TimerUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_SYS_TimerCreate(sysTimerId_t* pIdTSR, uint32_t msDuration, void* pfTSR)
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

#if(IMAGE_VIEWER_USE_SDL)
    SDL_Delay(msec);
#else
    usleep(msec * 1000);
#endif

    return retStatus;
}

static void* sTimerThread()
{
    uint8_t idxTSR;
    struct timespec tickNow;

    while(true)
    {
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

        SN_SYS_Delay(10);
    }
    return NULL;
}

static long long sDiffTick(const struct timespec startTick, const struct timespec endTick)
{
    const long long NANOS = 1000000000LL;
    long long retDiff = 0;

    retDiff = (NANOS * (endTick.tv_sec - startTick.tv_sec)) + (endTick.tv_nsec - startTick.tv_nsec);

    retDiff /= 1000000;

    return retDiff;
}
