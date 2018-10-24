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

/** @name File Sysetm Define *////@{
#define MAX_PAGE_SIZE               10
#define MAX_ITEM_SIZE                5
#define MAX_OPTION_SIZE              5

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256
///@}


/** @name File System Structure *////@{
typedef struct file_system_option {
    char name[MAX_FILENAME_LENGTH];
} fsOption_t;

typedef struct file_system_item {
    char name[MAX_FILENAME_LENGTH];
} fsItem_t;

typedef struct file_system_page {
    fsItem_t item[MAX_ITEM_SIZE];
    uint32_t itemCnt;
} fsPage_t;

typedef struct file_system_target {
    uint32_t item_index;
    uint32_t page_index;
} fsTarget_t;

typedef struct file_system {
    fsTarget_t  target;
    fsOption_t  option[MAX_OPTION_SIZE];
    fsPage_t    page[MAX_PAGE_SIZE];
    uint32_t    pageCnt;
    bool        isItemExist;
} fs_t;
///@}


/** @name Print Info Structure *////@{
typedef struct print_prameter {
    float    layerThickness;            //mm

    /** Bottom Layer Parameter **/
    long     bottomLayerExposureTime;   //ms
    long     bottomLayerNumber;         //Layer
    float    bottomLiftFeedRate;        //mm/s

    /** Normal Layer Paramter **/
    long     layerExposureTime;         //ms
    float    liftFeedRate;              //mm/s
    long     liftTime;                  //ms - Need Auto Calculate
    long     liftDistance;              //mm

    float    retractFeedRate;           //mm/s

    float    stopPosition;              //mm
} printParm_t;


typedef struct print_target {
    char* sourceFilePath;
    char* tempFilePath;
    char* tempFileName;
    uint32_t slice;
} printTarget_t;

typedef struct print_information {
    printParm_t   printParameter;
    printTarget_t    printTarget;
    bool                  isInit;
} printInfo_t;
///@}

/** @name Machine Info Structure *////@{
typedef struct machine_information {
    char             name[MAX_FILENAME_LENGTH];
    char                height;
    bool                isInit;
} machineInfo_t;
///@}

/*************************************************************
 * @name File System Module
 * @brief Description of Display Module Init and Uninit funtions.
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
 *  @param pFs
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Get(fs_t* pFs);


/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Update(void);

/*************************************************************@}*/

/*************************************************************
 * @name File System Module :: Machine Info
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoUninit(void);

/** @brief
 *
 *  @return machineInfo_t
 *  @note
 */
extern machineInfo_t SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);

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
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoUninit(void);

/** @brief
 *
 *  @return printInfo_t
 *  @note
 */
extern printInfo_t   SN_MODULE_FILE_SYSTEM_PrintInfoGet(void);

/*************************************************************@}*/

#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
/**@}*/
