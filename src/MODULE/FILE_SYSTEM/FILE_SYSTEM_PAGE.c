/**
 * @file SN_MODULE_FILE_SYSTEM_PAGE.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_PAGE.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS FileSystem_PageInit(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS FileSystem_PageUninit(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}


SN_STATUS FileSystem_PageDestroy(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS FileSystem_AddPage(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS FileSystem_RemovePage(fsPageHeader_t* pageHeader, int pageIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS FileSystem_GetPage(fsPageHeader_t* pageHeader, int pageIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS FileSystem_GetItem(fsPageHeader_t* pageHeader, int pageIndex, int itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

bool FileSystem_IsPageEmpty(fsPageHeader_t* pageHeader)
{
    bool isEmpty = true;

    return isEmpty;
}
