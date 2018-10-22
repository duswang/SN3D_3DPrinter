    /**
 * @file SN_SYS_ERROR.c
 * @author Bato
 * @date 24 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_SYS_ERROR.h"

/* ******* STATIC DEFINE ******* */

/* ******* STATIC FUNCTIONS ******* */
void sReboot(void);
void sExit(void);

void SN_SYS_ErrorCheck(SN_STATUS errorStatus, const char* errorMessage, const char* _file, const char* _func, const int _line)
{
    if(errorStatus != SN_STATUS_OK)
    {
        printf("\n* SN ERROR * \nFILE : %s\nFUNC : %s\nLINE : %d\nMSG  : %s\nCODE : %d\n",_file, _func, _line, errorMessage, errorStatus); fflush(stdout);

        sExit();
    }
}

void SN_SYS_Log(const char* message)
{
    printf("%s\n", message); fflush(stdout);
}

void sReboot(void)
{
    exit(-1);
}

void sExit(void)
{
    exit(-1);
}
