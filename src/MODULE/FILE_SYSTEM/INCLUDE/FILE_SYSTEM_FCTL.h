/**
 * @file FILE_SYSTEM_FCTL.h
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup FILE_SYSTEM_FCTL File Control
 * @ingroup MODULE_FILE_SYSTEM
 * @brief File System File Control Functions.
 * @{
 */

#ifndef FILE_SYSTEM_FCTL_H_
#define FILE_SYSTEM_FCTL_H_

/*************************************************************
 * @name File System File Control
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS FileSystem_fctl_CopyFile(const char* srcPath, const char* desPath);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS FileSystem_fctl_RemoveFiles(const char* folderPath);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS FileSystem_fctl_ExtractFile(const char* srcPath, const char* desPath);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS FileSystem_fctl_MakeDirectory(const char* dir);


/*************************************************************@}*/

/*************************************************************
 * @name File System File Control :: Util
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern const char* FileSystem_fctl_ExtractFileExtention(const char *filename);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern const char* FileSystem_fctl_Extarct_FileName(const char *filename);

/*************************************************************@}*/

#endif /*FILE_SYSTEM_FCTL_H_ */
