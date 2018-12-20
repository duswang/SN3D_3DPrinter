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

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern printTarget_t* FileSystem_targetXMLLoad(const char *srcPath);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern printOption_t* FileSystem_optionXMLLoad(const char *srcPath);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath);

/*************************************************************@}*/

#endif /* SN_MODULE_FILE_SYSTEM_XML_H_ */
