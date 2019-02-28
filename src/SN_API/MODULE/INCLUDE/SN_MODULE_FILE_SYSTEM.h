/**
 * @file SN_MODULE_FILE_SYSTEM.h
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup MODULE_FILE_SYSTEM File System
 * @ingroup MODULE
 * @brief File System Module Functions.
 * @{
 */

#ifndef MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_
#define MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_

/*************************************************************
 * @name File System Define
 *
 *////@{
#define MAX_ITEM_SIZE                5

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256
#define MAX_OPTION_FILENAME          8

/** @name USB Driver Config *////@{
#ifdef __APPLE__
#define USB_FOLDER_PATH                      "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_FOLDER_PATH                      "/mnt/volume"
#endif

#ifdef __APPLE__
#define TARGET_FOLDER_PATH                   "../res/target"
#endif
#ifdef linux
#define TARGET_FOLDER_PATH                   "/SN3D/sn3d-project/res/target"
#endif

#ifdef __APPLE__
#define OPTION_FOLDER_PATH               "../res/optionConfig"
#endif
#ifdef linux
#define OPTION_FOLDER_PATH               "/SN3D/sn3d-project/res/optionConfig"
#endif

#ifdef __APPLE__
#define MACHINE_FOLDER_PATH              "../res/machineConfig"
#endif
#ifdef linux
#define MACHINE_FOLDER_PATH              "/SN3D/sn3d-project/res/machineConfig"
#endif

#ifdef __APPLE__
#define VERSION_FOLDER_PATH              "../sn3d-bootloader/res/firmware"
#endif
#ifdef linux
#define VERSION_FOLDER_PATH              "/SN3D/sn3d-project/sn3d-bootloader/res/firmware"
#endif

#ifdef __APPLE__
#define BINARY_FOLDER_PATH              "../bin/build/sn3d"
#endif
#ifdef linux
#define BINARY_FOLDER_PATH              "/SN3D/sn3d-project/bin/build/sn3d"
#endif

#ifdef __APPLE__
#define DEVICE_FOLDER_PATH              "../res/deviceConfig"
#endif
#ifdef linux
#define DEVICE_FOLDER_PATH              "/SN3D/sn3d-project/res/deviceConfig"
#endif

#ifdef __APPLE__
#define DEFAULT_MACHINE_FILE_PATH      "../lib/machineConfig/machineConfig.xml"
#define DEFAULT_VERSION_FILE_PATH      "../sn3d-bootloader/lib/default/version.xml"
#define DEFAULT_OPTION_FILE_PATH       "../lib/optionConfig/default_option.xml"
#define DEFAULT_DEVICE_FILE_PATH       "../lib/default/deviceInfo.xml"
#endif
#ifdef linux
#define DEFAULT_VERSION_FILE_PATH      "/SN3D/sn3d-project/sn3d-bootloader/lib/default/version.xml"
#define DEFAULT_MACHINE_FILE_PATH      "/SN3D/sn3d-project/sn3d-bootloader/lib/default/machineInfo.xml"
#define DEFAULT_OPTION_FILE_PATH       "/SN3D/sn3d-project/lib/default/option_default.s3c"
#define DEFAULT_DEVICE_FILE_PATH       "/SN3D/sn3d-project/lib/default/deviceInfo.xml"
#endif

///@}

/*************************************************************@}*/

/*************************************************************
 * @name File Sysetm Define
 *
 *////@{
#define MANIFEST_FILE_NAME      "manifest"
#define MANIFEST_FILE_EXT       "xml"

#define MACHINE_FILE_NAME       "machineInfo"
#define MACHINE_FILE_EXT        "xml"

#define DEVICE_FILE_NAME        "deviceInfo"
#define DEVICE_FILE_EXT         "xml"

#define OPTION_FILE_NAME        "option"
#define OPTION_FILE_EXT         "s3c"

#define TARGET_CWS_FILE_EXT     "cws"
#define TARGET_ZIP_FILE_EXT     "zip"
#define TARGET_IMAGE_EXT        "png"

#define SN3D_FW_STR             "sn3d_fw"
#define SN3D_FW_EXTENTION       "zip"

#define SN3D_OPTION_STR         "sn3d_option"
#define SN3D_OPTION_EXTENTION   "zip"

#define SN3D_VERSION_STR        "version"
#define SN3D_VERSION_EXTENTION  "xml"

#define NETFABB_CONDITION_STR   "index.xml"
#define MANGO_CONDITION_STR     ""
#define B9_CONDITION_STR        ""
#define CWS_CONDITION_STR       "cws"

typedef enum {
    SN3D,                    //!< SN3D
    NETFABB,                 //!< NETFABB
    MANGO,                   //!< MANGO
    B9,                      //!< B9
    CWS,                     //!< CWS
    TARGET_DEFAULT_TYPE = SN3D//!< SN3D
} targetType_t;

/*************************************************************@}*/


/*************************************************************
 * @name File System Structure
 *
 *////@{
typedef struct file_system_item {
    char name[MAX_FILENAME_LENGTH];
    void* contents;
} fsItem_t;

typedef struct file_system_page {
    fsItem_t item[MAX_ITEM_SIZE];
    uint32_t itemCnt;
    struct file_system_page* prevPage;
    struct file_system_page* nextPage;
} fsPage_t;

typedef struct file_system_page_header {
    fsPage_t* firstPage;
    fsPage_t* lastPage;

    uint32_t    pageCnt;
    uint32_t    itemCnt;
} fsPageHeader_t;

typedef struct file_system {
    fsPageHeader_t*    filePageHeader;
    fsPageHeader_t*    machineInfoPageHeader;
    fsPageHeader_t*    optionPageHeader;
} fileSystem_t;

/*************************************************************@}*/

/*************************************************************
 * @name Print Info Structure
 *
 *////@{

typedef struct print_option_prameter {
    char     name[MAX_OPTION_FILENAME];

    float    layerThickness;                /**< mm */

    /* Bottom Layer Parameter */
    uint32_t     bottomLayerExposureTime;   /**< ms */
    uint32_t     bottomLayerNumber;         /**< Layer */
    float        bottomLiftFeedRate;        /**< mm / s */

    /* Normal Layer Paramter */
    uint32_t     layerExposureTime;         /**< ms */
    float        liftFeedRate;              /**< mm / s */
    uint32_t     liftTime;                  /**< ms */
    uint32_t     liftDistance;              /**< mm */

    uint32_t     bright;                    /**< 0 ~ 255 **/
} printOption_t;

typedef struct print_target {
    char  targetPath[MAX_PATH_LENGTH];
    char  targetName[MAX_FILENAME_LENGTH];

    char  projectName[MAX_FILENAME_LENGTH];

    uint32_t slice;
    targetType_t targetType;
} printTarget_t;

/*************************************************************@}*/

/*************************************************************
 * @name Device Info Structure
 *////@{
#define DEVICE_LANGUAGE_ENG     "Eng"
#define DEVICE_LANGUAGE_KOR     "Kor"
#define DEVICE_LANGUAGE_DEFAULT DEVICE_LANGUAGE_KOR

typedef struct device_information {
    char  language[MAX_FILENAME_LENGTH];  /**< px */
    uint32_t                  totalTime;  /**< px */
} deviceInfo_t;


/*************************************************************@}*/


/*************************************************************
 * @name Machine Info Structure
 *////@{
typedef struct machine_information {
    char    name[MAX_FILENAME_LENGTH];  /**< str */
    uint32_t              screenWidth;  /**< px */
    uint32_t             screenHeight;  /**< px */
    uint32_t            machineHeight;  /**< mm */
    char    displayScreenSize[MAX_FILENAME_LENGTH];  /** inch **/
    char    touchScreenSize[MAX_FILENAME_LENGTH];/** inch **/
} machineInfo_t;
/*************************************************************@}*/

/*************************************************************
 * @struct Version Info Structure
 *////@{
typedef struct version_information {
    char           name[MAX_FILENAME_LENGTH];  /**< str */
    uint32_t                   releaseNumber;  /**< num */
    uint32_t                     majorNumber;  /**< num */
    uint32_t                     minorNumber;  /**< num */
    char      timestamp[MAX_FILENAME_LENGTH];  /**< str */
    char     binaryName[MAX_FILENAME_LENGTH];  /**< str */
    char             hash[(MD5_DIGEST_LENGTH * 2) + 1];
} versionInfo_t;
/*************************************************************@}*/

/*************************************************************
 * @name File System Module
 * @brief
 * @{
 */


/** @brief File System Init
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void);

/** @brief File System Uninit
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: FS
 * @brief
 * @{
 */


/** @brief Read USB File List and Update File System Strucutre
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_FilePageUpdate(void);

/** @brief Get one page
 *
 *  @param pageIndex - Page index
 *  @return SN_STATUS
 *
 *  @note
 */
extern const fsPage_t* SN_MODULE_FILE_SYSTEM_GetFilePage(int pageIndex);

/** @brief Get All file page number

 *  @return All File page number
 *
 *  @note
 */
extern uint32_t SN_MODULE_FILE_SYSTEM_GetFilePageCnt(void);

/** @brief Is any print file exist there?
 *
 *  @return bool
 *
 *  @note
 */
extern bool SN_MODULE_FILE_SYSTEM_isPrintFileExist(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Machine Info
 * @brief
 * @{
 */

/** @brief Get Machine Info
 *
 *  @return machineInfo_t pointer
 *
 *  @note
 */
extern const machineInfo_t* SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Device Info
 * @brief
 * @{
 */
/** @brief Device Info Load
 *
 *
 *  @return deviceInfo_t pointer
 *  @note
 */
extern const deviceInfo_t* SN_MODULE_FILE_SYSTEM_DeviceInfoGet(void);

/** @brief Device Info Update
 *
 *  @param deviceInfo_t
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_DeviceInfoUpdate(const deviceInfo_t deviceInfo);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Version Info
 * @brief
 * @{
 */

/** @brief Get Version Info
 *
 *  @return versionInfo_t pointer
 *
 *  @note
 */
extern const versionInfo_t* SN_MODULE_FILE_SYSTEM_VersionInfoGet(void);


/*************************************************************@}*/


/*************************************************************
 * @name File System Module :: Print Option
 * @brief
 * @{
 */

/** @brief Get Option Count

 *  @return SN_STATUS
 *  @note
 */
extern uint32_t SN_MODULE_FILE_SYSTEM_GetOptionCnt(void);

/** @brief Is any option file exist there?
 *
 *  @return bool
 *
 *  @note
 */
extern bool SN_MODULE_FILE_SYSTEM_isOptionExist(void);

/** @brief Option File Load
 *
 *  @param optionIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_OptionLoad(uint32_t optionIndex);

/** @brief Get Loaded Option Info
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern const printOption_t* SN_MODULE_FILE_SYSTEM_OptionGet(void);


/*************************************************************@}*/


/*************************************************************
 * @name File System Module :: Print Target Info
 * @brief
 * @{
 */
/** @brief Print Target Info Load
 *
 *  @param pageIndex
 *  @param itemIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_TargetLoad(uint32_t pageIndex, uint32_t itemIndex);

/** @brief Print Target Info Destory
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_TargetDestroy(void);

/** @brief Get Loaded Print Target Info
 *
 *  @param optionIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern const printTarget_t* SN_MODULE_FILE_SYSTEM_TargetGet(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Utils
 * @brief
 * @{
 */

/** @brief Get Path by sliceindex
 *
 *  @param sliceIndex - Image slice index
 *  @return Image path by file type
 *
 *  @ref targetType_t
 *
 *  @note
 */
extern char* SN_MODULE_FILE_SYSTEM_TargetSlicePathGet(uint32_t sliceIndex);

/*************************************************************@}*/


#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
/**@}*/
