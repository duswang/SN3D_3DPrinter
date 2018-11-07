/**
 * @file FILE_SYSTEM_PAGE.h
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup FILE_SYSTEM_PAGE Page List
 * @ingroup MODULE_FILE_SYSTEM
 * @brief File System Page List Functions.
 * @{
 */

#ifndef FILE_SYSTEM_PAGE_H_
#define FILE_SYSTEM_PAGE_H_

SN_STATUS FileSystem_PageInit(fsPageHeader_t* pageHeader);
SN_STATUS FileSystem_PageUninit(fsPageHeader_t* pageHeader);


SN_STATUS FileSystem_PageDestroy(fsPageHeader_t* pageHeader);

SN_STATUS FileSystem_AddPage(fsPageHeader_t* pageHeader);
SN_STATUS FileSystem_RemovePage(fsPageHeader_t* pageHeader, int pageIndex);

SN_STATUS FileSystem_GetPage(fsPageHeader_t* pageHeader, int pageIndex);
SN_STATUS FileSystem_GetItem(fsPageHeader_t* pageHeader, int pageIndex, int itemIndex);

bool FileSystem_IsPageEmpty(fsPageHeader_t* pageHeader);

#endif /* SN_MODUL_FILE_SYSTEM_PAGE_H_ */
