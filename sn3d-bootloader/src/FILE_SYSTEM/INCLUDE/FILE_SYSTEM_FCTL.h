/**
 * @file FILE_SYSTEM_FCTL.h
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup BOOTLOADER_FILE_SYSTEM_FCTL File Control
 * @ingroup BOOTLOADER_FILE_SYSTEM
 * @brief ref 'SN API Document' or 'SN_API/Module/File_System'.
 * @{
 */

#ifndef FILE_SYSTEM_FCTL_H_
#define FILE_SYSTEM_FCTL_H_

/*************************************************************
 * @name File System File Control
 * @brief Copy File.
 * @{
 */

/** @brief Copy File
 *
 *  @param srcPath - Source path
 *  @param desPath - Destination path
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS FileSystem_fctl_CopyFile(const char* srcPath, const char* desPath);


/** @brief Remove All File in Directory.
 *
 *  @param dir - Directory path.
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS FileSystem_fctl_RemoveFiles(const char* dir);


/** @brief Extract '.zip' file.
 *
 *  @param srcPath - Source path
 *  @param desPath - Destination path
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS FileSystem_fctl_ExtractFile(const char* srcPath, const char* desPath);


/** @brief Make directory.
 *
 *  @param dir - Directory path.
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS FileSystem_fctl_MakeDirectory(const char* dir);

/** @brief Create directorys by Path.
 *
 *  @param dir - Directory path.
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS FileSystem_fctl_CreateDircetoryTree(const char* path);

/*************************************************************@}*/

/*************************************************************
 * @name File System File Control :: Util
 * @brief
 * @{
 */

/** @brief Extarct filename from filename with extention name.
 *
 *  @param filename - file name with extention [ ex) test.txt ]
 *  @return It return only filename [ ex) test ]
 *
 *  @note
 */
extern char* FileSystem_fctl_Extarct_FileName(const char* filename);


/** @brief Extarct file extention from filename with extention name.
 *
 *  @param filename - file name with extention [ ex) test.txt ]
 *  @return It return only extention name [ ex) .txt ]
 *
 *  @note
 */
extern const char* FileSystem_fctl_ExtractFileExtention(const char* filename);

/** @brief Count File by condition string
 *
 *  @param srcPath - Source path
 *  @param condStr - Condtition string
 *
 *  @return Counted number
 *
 *  @note
 */
uint32_t FileSystem_CountFileWithStr(const char* srcPath, const char* condStr);


/*************************************************************@}*/

#endif /*FILE_SYSTEM_FCTL_H_ */
