/*
 * SN_SYS_ERROR.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_ERROR.h"

/******** STATIC DEFINE ********/

/******** STATIC FUNCTIONS ********/
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
