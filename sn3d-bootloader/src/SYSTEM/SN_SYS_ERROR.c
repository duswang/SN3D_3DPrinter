    /**
 * @file SN_SYS_ERROR.c
 * @author Bato
 * @date 24 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_BOOTLOADER.h"
#include "SN_SYS_ERROR.h"

/* ******* STATIC DEFINE ******* */

/* ******* SYSTEM DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
static void sDebugging(void);
static void sReboot(void);
static void sExit(void);

void SN_SYS_ERROR_SystemLog(const char* message)
{
    printf("%s\n", message); fflush(stdout);
}

inline void SN_SYS_ERROR_StatusCheck_Inline(SN_STATUS errorStatus, const char* errorMessage, const char* _file, const char* _func, const int _line)
{
    if(errorStatus != SN_STATUS_OK)
    {
        printf("\n* SN ERROR * \nFILE : %s\nFUNC : %s\nLINE : %d\nMSG  : %s\nCODE : %d\n",_file, _func, _line, errorMessage, errorStatus); fflush(stdout);

        sExit();
    }
}

static void sDebugging(void)
{
    while(true);
}

static void sReboot(void)
{
    SN_SYS_ERROR_SystemLog("System Reboot. \n\n");

    sync();
    reboot(RB_AUTOBOOT);
}

static void sExit(void)
{
    exit(-1);
}
