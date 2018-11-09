/**
 * @file SN_MODULE_FILE_SYSTEM_PAGE.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo memory test
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

fsPageHeader_t* FileSystem_PageInit(void)
{
    fsPageHeader_t* pageHeader = NULL;

    pageHeader = (fsPageHeader_t *)malloc(sizeof(fsPageHeader_t));
    if(pageHeader == NULL)
    {
        return NULL;
    }

    pageHeader->firstPage = NULL;
    pageHeader->lastPage  = NULL;
    pageHeader->isItemExist = false;
    pageHeader->pageCnt   = 0;

    return pageHeader;
}

SN_STATUS FileSystem_PageDestroy(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    fsPage_t* currentPage = NULL;
    fsPage_t* nextPage    = NULL;

    if(pageHeader != NULL)
    {
        currentPage = pageHeader->firstPage;

        while(currentPage != NULL)
        {
            if(currentPage->nextPage == NULL)
            {
                free(currentPage);
                break;
            }
            else
            {
                nextPage = currentPage->nextPage;
                free(currentPage);
            }

            currentPage = nextPage;
        }

        free(pageHeader);
    }
    else
    {
        retStatus = SN_STATUS_ALREADY_INITIALIZED;
    }

    return retStatus;
}

SN_STATUS FileSystem_AddPage(fsPageHeader_t* pageHeader)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    fsPage_t* newPage = NULL;

    if(pageHeader == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    newPage = (fsPage_t*)malloc(sizeof(fsPage_t));
    if(newPage == NULL)
    {
        return SN_STATUS_NOT_OK;
    }

    newPage->itemCnt = 0;

    if(pageHeader->firstPage == NULL)
    {
        pageHeader->firstPage = newPage;
        pageHeader->firstPage->nextPage = NULL;
        pageHeader->firstPage->prevPage = NULL;

        pageHeader->lastPage = pageHeader->firstPage;
    }
    else
    {
        newPage->nextPage = pageHeader->lastPage->nextPage;
        newPage->prevPage = pageHeader->lastPage;

        pageHeader->lastPage->nextPage = newPage;
        pageHeader->lastPage = newPage;
    }



    pageHeader->pageCnt++;

    return retStatus;
}

SN_STATUS FileSystem_RemovePage(fsPageHeader_t* pageHeader, int pageIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    fsPage_t* currentPage = FileSystem_GetPage(pageHeader, pageIndex);
    fsPage_t* prevPage    = currentPage->prevPage;
    fsPage_t* nextPage    = currentPage->nextPage;

    prevPage->nextPage = nextPage;
    nextPage->prevPage = prevPage;

    free(currentPage);

    return retStatus;
}

fsPage_t* FileSystem_GetPage(fsPageHeader_t* pageHeader, int pageIndex)
{
    int i = 0;
    fsPage_t* retPage = NULL;
    fsPage_t* currentPage = pageHeader->firstPage;

    if(pageHeader == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "pageHeader is invalid");
    }

    if(pageHeader->pageCnt <= pageIndex)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "pageIndex pointing over page.");
    }

    for(i = 0; i < pageIndex; i++)
    {
        currentPage = currentPage->nextPage;
    }

    retPage = currentPage;

    return retPage;
}

fsItem_t FileSystem_GetItem(fsPageHeader_t* pageHeader, int pageIndex, int itemIndex)
{
    fsPage_t* currentPage = FileSystem_GetPage(pageHeader, pageIndex);

    return currentPage->item[itemIndex];
}


