/**
 * @file FILE_SYSTEM_XML
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup BOOTLOADER_FILE_SYSTEM_XML XML
 * @ingroup BOOTLOADER_FILE_SYSTEM
 * @brief File System XML Functions.
 * @{
 */

#ifndef SYSTEM_XML_H_
#define SYSTEM_XML_H_

/*************************************************************
 * @struct Machine Info Structure
 *////@{
typedef struct machine_information {
    char    name[MAX_FILENAME_LENGTH];  /**< str */
    long                  screenWidth;  /**< px */
    long                 screenHeight;  /**< px */
    long                machineHeight;  /**< mm */
    char    displayScreenSize[MAX_FILENAME_LENGTH];  /** inch **/
    char    touchScreenSize[MAX_FILENAME_LENGTH];/** inch **/
} machineInfo_t;
/*************************************************************@}*/

/*************************************************************
 * @struct Version Info Structure
 *////@{
typedef struct version_information {
    char           name[MAX_FILENAME_LENGTH];  /**< str */
    long                       releaseNumber;  /**< num */
    long                         majorNumber;  /**< num */
    long                         minorNumber;  /**< num */
    char      timestamp[MAX_FILENAME_LENGTH];  /**< str */
    char     binaryName[MAX_FILENAME_LENGTH];  /**< str */
    long                                hash;
} versionInfo_t;
/*************************************************************@}*/

/*************************************************************
 * @name File System XML
 * @brief
 * @{
 */

/** @brief Machine info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return machineInfo_t* - Return pointer of machineInfo structure.
 *
 *  @note
 */
extern machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath);

/** @brief Firmware version info XML file Reader.
 *
 *  @param srcPath - Source path
 *  @return versionInfo_t* - Return pointer of machineInfo structure.
 *
 *  @note
 */
extern versionInfo_t* FileSystem_versionInfoXMLLoad(const char *srcPath);

/*************************************************************@}*/

#endif /* SN_MODULE_FILE_SYSTEM_XML_H_ */
