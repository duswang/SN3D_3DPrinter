/**
 * @file SN_SYS_TIMER.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_TIMER Timer
 * @ingroup SYSTEM
 * @brief System Timer Functions.
 * @{
 */

#ifndef SN_SYS_TIMER_H_
#define SN_SYS_TIMER_H_

/** @name Timer Config
 *
 *////@{
#define MAX_NUM_OF_TSR 32
#define UNALLOCATED_TSR_ID          (0xFFFFFFFF)
///@}

/** @name Timer Structure
 *
 *////@{
typedef struct sys_timer_id {
    bool    isOccupied;
    struct timespec tickRequested;
    long     msDuration;
    uint32_t uniqueCode;
    void* (*pfTSRCallBack)();
} sysTimerQ_t;

typedef uint32_t sysTimerId_t;
///@}

/** @name Timer System
 *  @brief Description of Timer System Init and Uninit funtions.
 *////@{
extern SN_STATUS SN_SYS_TimerInit(void);
extern SN_STATUS SN_SYS_TimerUninit(void);
///@}

/** @name Serial System :: Create & Cancle
 *  @brief
 *////@{

/** @brief
 *
 *  @param pIdTSR
 *  @param msDuration
 *  @param pfTSR
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_TimerCreate(sysTimerId_t* pIdTSR, uint32_t msDuration, void* pfTSR);


/** @brief
 *
 *  @param pIdTSR
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_TimerCancle(sysTimerId_t* pIdTSR);
///@}

/** @name Serial System :: Delay
 *  @brief
 *  @note Now use SDL_Delay()
 *////@{

/** @brief
 *
 *  @param msec
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_Delay(uint32_t msec);
///@}


#endif /* SN_SYS_TIMER_H_ */
/**@}*/
