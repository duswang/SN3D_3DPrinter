/*
 * SN_MODUEL_FILE_SYSTEM.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_
#define MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_

#define MAX_PAGE_SIZE               10
#define MAX_ITEM_SIZE                5
#define MAX_OPTION_SIZE              5

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256

/**** FILE SYSTEM ****/
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

/**** INFO ****/
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

typedef struct resolution_information {
    int height;
    int weight;
} resolution_t;
typedef struct machine_information {
    char             name[100];
    char                height;
    resolution_t       display;
    bool                isInit;
} machineInfo_t;

/**** MODULE ****/
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void);

/**** FILE SYSTEM ****/
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Get(fs_t* pFs);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Update(void);

/**** INFO ****/
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoUninit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoUninit(void);

extern machineInfo_t SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);
extern printInfo_t   SN_MODULE_FILE_SYSTEM_PrintInfoGet(void);

#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
