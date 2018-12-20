/**
 * @file SN_BOOTLOADER_MAIN.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_BOOTLOADER.h"


/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* ******* UTIL :: FILE CONTROL FUNCTIONS ******* */

/* ******* MAIN FUNCTIONS ******* */

/**
*  @brief main
*/
int main(void)
{
    const char* firmwareFile = NULL;

    /* Firmware Update */
    firmwareFile = SN_VERSION_CONTROL_VersionCheck();
    if(firmwareFile != NULL)
    {
        SN_VERSION_CONTROL_VersionUpdate(firmwareFile);
    }

    return 0;
}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
