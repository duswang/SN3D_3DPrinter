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
 * @name Machine Info Structure
 *////@{
typedef struct machine_information {
    char    name[MAX_FILENAME_LENGTH];  /**< str */
    long                  screenWidth;  /**< px */
    long                 screenHeight;  /**< px */
    long                machineHeight;  /**< mm */
    float                        inch;  /** inch **/
} machineInfo_t;
/*************************************************************@}*/

/*************************************************************
 * @name Version Info Structure
 *////@{
typedef struct version_information {
    char           name[MAX_FILENAME_LENGTH];
    long                       releaseNumber;
    long                         majorNumber;
    long                         minorNumber;
    char      timestamp[MAX_FILENAME_LENGTH];
    char     binaryName[MAX_FILENAME_LENGTH];
} versionInfo_t;
/*************************************************************@}*/

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
extern machineInfo_t* FileSystem_machineInfoXMLLoad(const char *srcPath);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern versionInfo_t* FileSystem_versionInfoXMLLoad(const char *srcPath);

/*************************************************************@}*/

#endif /* SN_MODULE_FILE_SYSTEM_XML_H_ */
