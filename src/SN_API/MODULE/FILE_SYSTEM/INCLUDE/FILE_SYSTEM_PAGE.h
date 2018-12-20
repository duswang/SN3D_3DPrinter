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

/** @brief Init page data structure
 *
 *  @return fsPageHeader_t pointer
 *  @note
 */
fsPageHeader_t* FileSystem_PageInit(void);

/** @brief Destory page data structure
 *
 *  @param pageHeader - fsPageHeader_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
SN_STATUS FileSystem_PageDestroy(fsPageHeader_t* pageHeader);

/** @brief Add new page in pageHeader
 *
 *  @param pageHeader - fsPageHeader_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
SN_STATUS FileSystem_AddPage(fsPageHeader_t* pageHeader);

/** @brief Remove one page
 *
 *  @param pageHeader - fsPageHeader_t pointer
 *  @param pageIndex - Page index number
 *  @return SN_STATUS
 *
 *  @note
 */
SN_STATUS FileSystem_RemovePage(fsPageHeader_t* pageHeader, uint32_t pageIndex);

/*************************************************************@}*/

/*************************************************************
 * @name File System Page :: Get Contents
 * @brief
 * @{
 */

/** @brief Get one page Data
 *
 *  @param pageHeader - fsPageHeader_t pointer
 *  @param pageIndex - Page index number
 *  @return SN_STATUS
 *  @note
 */
fsPage_t* FileSystem_GetPage(fsPageHeader_t* pageHeader, uint32_t pageIndex);

/** @brief Get one item
 *
 *  @param pageHeader - fsPageHeader_t pointer
 *  @param pageIndex - Page index of number
 *  @param itemIndex - Item index of number
 *  @return SN_STATUS
 *
 *  @note Item index can't over index MAX_ITEM_SIZE
 */
fsItem_t FileSystem_GetItem(fsPageHeader_t* pageHeader, uint32_t pageIndex, uint32_t itemIndex);

/*************************************************************@}*/

#endif /* SN_MODUL_FILE_SYSTEM_PAGE_H_ */
