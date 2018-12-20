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
 * @brief
 * @{
 */

extern SN_STATUS FileSystem_fctl_CopyFile(const char* srcPath, const char* desPath);


extern SN_STATUS FileSystem_fctl_RemoveFiles(const char* dir);


extern SN_STATUS FileSystem_fctl_ExtractFile(const char* srcPath, const char* desPath);


extern SN_STATUS FileSystem_fctl_MakeDirectory(const char* dir);


/*************************************************************@}*/

/*************************************************************
 * @name File System File Control :: Util
 * @brief
 * @{
 */

extern char* FileSystem_fctl_Extarct_FileName(const char* filename);

extern const char* FileSystem_fctl_ExtractFileExtention(const char* filename);

uint32_t FileSystem_CountFileWithStr(const char* srcPath, const char* condStr);


/*************************************************************@}*/

#endif /*FILE_SYSTEM_FCTL_H_ */
