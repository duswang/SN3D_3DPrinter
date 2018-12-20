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

/** @name USB Driver Config *////@{
#ifdef __APPLE__
#define USB_PATH                      "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_PATH                      "/mnt/volume"
#endif

#ifdef __APPLE__
#define TARGET_PATH                   "../res/target"
#endif
#ifdef linux
#define TARGET_PATH                   "/SN3D/sn3d-project/res/target"
#endif

#ifdef __APPLE__
#define OPTION_FILE_PATH               "../res/optionConfig"
#endif
#ifdef linux
#define OPTION_FILE_PATH               "/SN3D/sn3d-project/res/optionConfig"
#endif

#ifdef __APPLE__
#define MACHINE_FILE_PATH              "../res/machineConfig"
#endif
#ifdef linux
#define MACHINE_FILE_PATH              "/SN3D/sn3d-project/res/machineConfig"
#endif
///@}

/*************************************************************@}*/

/*************************************************************
 * @name File Sysetm Define
 *
 *////@{
#define MANIFEST_FILE_NAME      "manifest"
#define MANIFEST_FILE_EXT       "xml"

#define MACHINE_FILE_EXT        "xml"

#define OPTION_FILE_EXT         "xml"

#define TARGET_CWS_FILE_EXT     "cws"
#define TARGET_ZIP_FILE_EXT     "zip"
#define TARGET_IMAGE_EXT        "png"

#define SN3D_FW_STR             "sn3d_fw"
#define SN3D_FW_EXTENTION       "zip"

#define SN3D_OPTION_STR             "sn3d_option"
#define SN3D_OPTION_EXTENTION       "zip"

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
    TARGET_DEFAULT_TYPE = CWS//!< TARGET_DEFAULT_TYPE
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
    char     name[MAX_FILENAME_LENGTH];

    float    layerThickness;            /**< mm */

    /* Bottom Layer Parameter */
    long     bottomLayerExposureTime;   /**< ms */
    long     bottomLayerNumber;         /**< Layer */
    float    bottomLiftFeedRate;        /**< mm / s */

    /* Normal Layer Paramter */
    long     layerExposureTime;         /**< ms */
    float    liftFeedRate;              /**< mm / s */
    long     liftTime;                  /**< ms - Need Auto Calculate */
    long     liftDistance;              /**< mm */

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
 * @name File System Module
 * @brief
 * @{
 */


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void);

/** @brief
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


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_FilePageUpdate(void);

/** @brief
 *
 *
 *  @return SN_STATUS
 *  @note
 */
extern const fsPage_t* SN_MODULE_FILE_SYSTEM_GetFilePage(int pageIndex);

/** @brief

 *  @return SN_STATUS
 *  @note
 */
extern int SN_MODULE_FILE_SYSTEM_GetFilePageCnt(void);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern bool SN_MODULE_FILE_SYSTEM_isPrintFileExist(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Machine Info
 * @brief
 * @{
 */

/** @brief
 *
 *  @param optionIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern const machineInfo_t* SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);


/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Print Option
 * @brief
 * @{
 */

/** @brief

 *  @return SN_STATUS
 *  @note
 */
extern int SN_MODULE_FILE_SYSTEM_GetOptionCnt(void);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern bool SN_MODULE_FILE_SYSTEM_isOptionExist(void);

/** @brief
 *
 *  @param optionIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_OptionLoad(uint32_t optionIndex);

/** @brief
 *
 *  @param optionIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern const printOption_t* SN_MODULE_FILE_SYSTEM_OptionGet(void);


/*************************************************************@}*/


/*************************************************************
 * @name File System Module :: Print Target Info
 * @brief
 * @{
 */
/** @brief
 *
 *  @param pageIndex
 *  @param itemIndex
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_TargetLoad(uint32_t pageIndex, uint32_t itemIndex);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_TargetDestroy(void);

/** @brief
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

/** @brief
 *
 *  @param
 *
 *  @return SN_STATUS
 *  @note
 */
extern char* SN_MODULE_FILE_SYSTEM_TargetSlicePathGet(uint32_t sliceIndex);

/*************************************************************@}*/


#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
/**@}*/
