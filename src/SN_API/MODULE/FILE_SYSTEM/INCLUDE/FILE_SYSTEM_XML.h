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

/** @brief Machine info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return Return pointer of machineInfo structure.
 *
 *  @note
 */
extern machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath);

/*************************************************************@}*/

#endif /* SN_MODULE_FILE_SYSTEM_XML_H_ */
