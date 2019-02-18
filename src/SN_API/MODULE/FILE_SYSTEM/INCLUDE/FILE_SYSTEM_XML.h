/**
 * @file FILE_SYSTEM_XML
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup FILE_SYSTEM_XML XML
 * @ingroup MODULE_FILE_SYSTEM
 * @brief File System XML Functions.
 * @{
 */

#ifndef SYSTEM_XML_H_
#define SYSTEM_XML_H_

/*************************************************************
 * @name File System XML
 * @brief
 * @{
 */

/** @brief Target XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of printTarget_t structure.
 *
 *  @note only used 'cws' file.
 */
extern printTarget_t* FileSystem_targetXMLLoad(const char *srcPath);

/** @brief Option XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of printOption_t structure.
 *
 *  @note
 */
extern printOption_t* FileSystem_optionXMLLoad(const char *srcPath);

/** @brief Device info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of deviceInfo structure.
 *
 *  @note
 */
extern deviceInfo_t* FileSystem_deviceInfoXMLLoad(const char *srcPath);

/** @brief Device info Update XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of deviceInfo structure.
 *
 *  @note
 */
extern SN_STATUS FileSystem_deviceInfoXMLUpdate(const char *srcPath, const deviceInfo_t deviceInfo);

/** @brief Machine info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of machineInfo structure.
 *
 *  @note
 */
extern machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath);

/** @brief Version info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of machineInfo structure.
 *
 *  @note
 */
extern versionInfo_t* FileSystem_versionInfoXMLLoad(const char *srcPath);

/*************************************************************@}*/

/*************************************************************
 * @name File System MD5 Hash
 * @brief
 * @{
 */

#define HASH_MAGIC_NUMBER "0xDEADBEEF"

/** @brief
 *
 *  @param
 *  @return MD5 hash
 *
 *  @note
 */
extern unsigned char* FileSysetm_MD5_Hash_WithFile(char* path, char* salt);

/** @brief MD5 Hash to String Data
 *
 *  @param
 *  @return MD5 hash with a String
 *
 *  @note
 */
extern unsigned char* FileSystem_MD5_HashToString(unsigned char* hash);

/*************************************************************@}*/


#endif /* SN_MODULE_FILE_SYSTEM_XML_H_ */
