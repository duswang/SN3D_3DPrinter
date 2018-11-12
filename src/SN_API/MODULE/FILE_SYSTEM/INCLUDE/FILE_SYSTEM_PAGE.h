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

/*************************************************************
 * @name File System Page
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
fsPageHeader_t* FileSystem_PageInit(void);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
SN_STATUS FileSystem_PageDestroy(fsPageHeader_t* pageHeader);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
SN_STATUS FileSystem_AddPage(fsPageHeader_t* pageHeader);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
SN_STATUS FileSystem_RemovePage(fsPageHeader_t* pageHeader, int pageIndex);

/*************************************************************@}*/

/*************************************************************
 * @name File System Page :: Get Contents
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
fsPage_t* FileSystem_GetPage(fsPageHeader_t* pageHeader, int pageIndex);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
fsItem_t FileSystem_GetItem(fsPageHeader_t* pageHeader, int pageIndex, int itemIndex);

/*************************************************************@}*/

#endif /* SN_MODUL_FILE_SYSTEM_PAGE_H_ */
