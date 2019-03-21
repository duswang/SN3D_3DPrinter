/**
 * @file SN_MODULE_FILE_SYSTEM.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_FCTL.h"
#include "FILE_SYSTEM_PAGE.h"
#include "FILE_SYSTEM_XML.h"

/* ******* STATIC DEFINE ******* */
/** @name Other Define *////@{
#define OPTION_DEFAULT_INDEX       (0)

#define MACHINE_DEFAULT_INDEX      (0)
///@}

/* *** MODULE *** */
typedef struct moduel_file_system {
    fileSystem_t    fileSystem;

    printTarget_t*   printTarget;
    printOption_t*   printOption;
    machineInfo_t*   machineInfo;
    versionInfo_t*   versionInfo;
    deviceInfo_t*    deviceInfo;

} moduleFileSystem_t;

/* ******* SYSTEM DEFINE ******* */
/* *** MODULE THREAD *** */
static pthread_mutex_t ptmFileSystem = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       ptFileSystem;

/* *** MODULE MESSAGE Q *** */
static sysMessageQId msgQIdFileSystem;
    /* *** MODULE MESSAGES *** */
typedef enum {
    MSG_FILE_SYSTEM_USB_MOUNT       = 0,        /**< USB Mount */
    MSG_FILE_SYSTEM_USB_UNMOUNT,                /**< USB Unmount */
    MSG_FILE_SYSTEM_READ,                       /**< Read USB - when USB Mounted */
    MSG_FILE_SYSTEM_UPDATE,                     /**< Update File System - when read USB Finish */
    MSG_FILE_SYSTEM_WAITING,                    /**< Waiting Next Event */
    MSG_FILE_SYSTEM_NONE,                       /**< BAD ACCESS */
} evtFileSystem_t;

/* *** MODULE HANDLER  *** */
static moduleFileSystem_t moduleFileSystem;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* **** USB Callback *** */
static void*            USBEvent_Callback(int evt);

/* *** SYSTEM *** */
static void*            sFileSystemThread();
static SN_STATUS        sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage);

/* *** FILE SYSTEM CONTROL *** */
static SN_STATUS sMachineInfoPageLoad(fileSystem_t* fileSystem);
static SN_STATUS sMachineInfoPageDestroy(fileSystem_t* fileSystem);

static SN_STATUS sOptionPageLoad(fileSystem_t* fileSystem);
static SN_STATUS sOptionPageDestroy(fileSystem_t* fileSystem);

static SN_STATUS sFilePageLoad(fileSystem_t* fileSystem);
static SN_STATUS sFilePageDestroy(fileSystem_t* fileSystem);

static SN_STATUS sFileSystemPrint(const fileSystem_t* fileSystem);

/* *** OPTION *** */
static SN_STATUS sOptionLoad(uint32_t optionIndex);

/* *** MAHCINE *** */
static SN_STATUS sMachineInfoLoad(uint32_t machineInfoIndex);

/* *** VERSION *** */
static SN_STATUS sVersionInfoLoad(void);
static bool      sVersionHashCheck(void);
/* *** DEVICE *** */
static SN_STATUS sDeviceInfoLoad(void);

/* *** TARGET *** */
static SN_STATUS sTargetLoad(uint32_t pageIndex, uint32_t itemIndex);
static SN_STATUS sTargetDestroy(void);
static SN_STATUS sTargetFileCreate(const char* fileName, const char* fileExtention);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("MODULE INIT => FILE SYSTEM.");

    SN_MODULE_DISPLAY_BootProgressUpdate(40, "FS Module Loading...");
    SN_SYS_TIMER_Delay(500);

    SN_MODULE_DISPLAY_BootProgressUpdate(42, "USB Read...");
    SN_SYS_TIMER_Delay(500);

    /* USB DRIVER INIT */
    SN_SYS_USBDriverInit(USBEvent_Callback);

    /* MESSAGE Q INIT */
    msgQIdFileSystem = SN_SYS_MESSAGE_Q_Init();
    SN_SYS_ERROR_StatusCheck(retStatus, "File System Module Message Q Init Failed.");

    /* MUTEX INIT */
    if (pthread_mutex_init(&ptmFileSystem, NULL) != 0)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System Mutex Init Failed.");
    }

    /* THREAD INIT */
    if((retStatus = pthread_create(&ptFileSystem, NULL, sFileSystemThread, NULL)))
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System Thread Init Failed.");
    }

    /* Default Machine Info Setup */
    SN_MODULE_DISPLAY_BootProgressUpdate(45, "Version Info Check...");
    SN_SYS_TIMER_Delay(250);

    sVersionInfoLoad();

    /* Default Machine Info Setup */
    SN_MODULE_DISPLAY_BootProgressUpdate(45, "Version Info Check...");
    SN_SYS_TIMER_Delay(250);

    sVersionInfoLoad();

    SN_MODULE_DISPLAY_BootProgressUpdate(50, "binary File Check...");
    if(sVersionHashCheck())
    {
        SN_MODULE_DISPLAY_BootProgressUpdate(52, "binary ok...");
    }
    else
    {
        SN_MODULE_DISPLAY_BootProgressUpdate(52, "bad binary detected !!");
        SN_SYS_TIMER_Delay(3000);
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "binary is changed not normaly !!");
    }

    SN_SYS_TIMER_Delay(250);

    /* Load Option */
    SN_MODULE_DISPLAY_BootProgressUpdate(50, "Mahcine Info Read...");
    SN_SYS_TIMER_Delay(250);

    sMachineInfoPageLoad(&moduleFileSystem.fileSystem);
    sMachineInfoLoad(MACHINE_DEFAULT_INDEX);

    /* Machine File*/
    SN_MODULE_DISPLAY_BootProgressUpdate(55, "Option File Read...");
    SN_SYS_TIMER_Delay(250);
    sOptionPageLoad(&moduleFileSystem.fileSystem);
    sOptionLoad(OPTION_DEFAULT_INDEX);

    /* Device File */
    SN_MODULE_DISPLAY_BootProgressUpdate(60, "Device File Read...");
    SN_SYS_TIMER_Delay(250);

    sDeviceInfoLoad();

    SN_MODULE_FILE_SYSTEM_DeviceInfoUpdate(*moduleFileSystem.deviceInfo);

    sFileSystemPrint(&moduleFileSystem.fileSystem);


    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void)
{
    /* Page Destroy */
    sOptionPageDestroy(&moduleFileSystem.fileSystem);
    sMachineInfoPageDestroy(&moduleFileSystem.fileSystem);
    sFilePageDestroy(&moduleFileSystem.fileSystem);

    /* Module Controller Destroy */
    moduleFileSystem.printOption = NULL;
    moduleFileSystem.machineInfo = NULL;
    SN_MODULE_FILE_SYSTEM_TargetDestroy();

    SN_SYS_MESSAGE_Q_Remove(msgQIdFileSystem);

    return SN_STATUS_OK;
}

bool SN_MODULE_FILE_SYSTEM_isPrintFileExist(void)
{
    if(moduleFileSystem.fileSystem.filePageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return moduleFileSystem.fileSystem.filePageHeader->itemCnt;
}

uint32_t SN_MODULE_FILE_SYSTEM_GetFilePageCnt(void)
{
    if(moduleFileSystem.fileSystem.filePageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return moduleFileSystem.fileSystem.filePageHeader->pageCnt;
}

const fsPage_t* SN_MODULE_FILE_SYSTEM_GetFilePage(int pageIndex)
{
    if(moduleFileSystem.fileSystem.filePageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return FileSystem_GetPage(moduleFileSystem.fileSystem.filePageHeader, pageIndex);
}

SN_STATUS SN_MODULE_FILE_SYSTEM_FilePageUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
    SN_SYS_ERROR_StatusCheck(retStatus, "File System Send Message Failed.");

    return retStatus;
}

const versionInfo_t* SN_MODULE_FILE_SYSTEM_VersionInfoGet(void)
{
    if(moduleFileSystem.versionInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "VersionInfo not loaded.");
    }

    return moduleFileSystem.versionInfo;
}

const deviceInfo_t* SN_MODULE_FILE_SYSTEM_DeviceInfoGet(void)
{
    int i = 0;

    while(i < 5)
    {
        if(SN_STATUS_OK == sDeviceInfoLoad())
        {
            break;
        }
        SN_SYS_TIMER_Delay(30);
    }

    if(moduleFileSystem.deviceInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "deviceInfo not loaded.");
    }

    return moduleFileSystem.deviceInfo;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_DeviceInfoReset(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    deviceInfo_t deviceInfo;
    char path[MAX_PATH_LENGTH];

    if(moduleFileSystem.deviceInfo == NULL)
    {
        strcpy(deviceInfo.language, DEVICE_LANGUAGE_ENG);
    }
    else
    {
        strcpy(deviceInfo.language, moduleFileSystem.deviceInfo->language);
    }

    deviceInfo.totalTime = 0;

    sprintf(path,"%s/%s.%s", DEVICE_FOLDER_PATH, DEVICE_FILE_NAME, DEVICE_FILE_EXT);

    FileSystem_deviceInfoXMLUpdate(path, deviceInfo);

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_DeviceInfoUpdate(const deviceInfo_t deviceInfo)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char path[MAX_PATH_LENGTH];

    sprintf(path,"%s/%s.%s", DEVICE_FOLDER_PATH, DEVICE_FILE_NAME, DEVICE_FILE_EXT);

    FileSystem_deviceInfoXMLUpdate(path, deviceInfo);

    return retStatus;
}

const machineInfo_t* SN_MODULE_FILE_SYSTEM_MachineInfoGet(void)
{
    if(moduleFileSystem.machineInfo == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "machineInfo not loaded.");
    }

    return moduleFileSystem.machineInfo;
}

const fsPage_t* SN_MODULE_FILE_SYSTEM_GetOptionPage(int pageIndex)
{
    if(moduleFileSystem.fileSystem.filePageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return FileSystem_GetPage(moduleFileSystem.fileSystem.filePageHeader, pageIndex);
}

bool SN_MODULE_FILE_SYSTEM_isOptionExist(void)
{
    if(moduleFileSystem.fileSystem.optionPageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return moduleFileSystem.fileSystem.optionPageHeader->itemCnt;
}

uint32_t SN_MODULE_FILE_SYSTEM_GetOptionCnt(void)
{
    if(moduleFileSystem.fileSystem.filePageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "File System not initialized.");
    }

    return moduleFileSystem.fileSystem.optionPageHeader->itemCnt;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_OptionLoad(uint32_t optionIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sOptionLoad(optionIndex);
    SN_SYS_ERROR_StatusCheck(retStatus, "option is not loaded.");

    return retStatus;
}

printOption_t SN_MODULE_FILE_SYSTEM_OptionGet(void)
{
    printOption_t loadedOption;

    if(moduleFileSystem.printOption == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "option is not loaded.");
    }

    loadedOption = *moduleFileSystem.printOption;

    return loadedOption;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_TargetLoad(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleFileSystem.printTarget != NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_ALREADY_INITIALIZED, "target is already initialized. plase destroy target.");
    }

    retStatus = sTargetLoad(pageIndex, itemIndex);
    SN_SYS_ERROR_StatusCheck(retStatus, "option is not loaded.");

    return retStatus;
}

char* SN_MODULE_FILE_SYSTEM_TargetSlicePathGet(uint32_t sliceIndex)
{
    char* path = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    unsigned int numberSpace = 1;
    unsigned int sliceNumber = 0;
    int sliceIndex_Offset = 0;

    if(moduleFileSystem.printTarget == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "target is not loaded.");
    }

    for (sliceNumber = moduleFileSystem.printTarget->slice; sliceNumber > 0; sliceNumber /= 10, numberSpace++)


    switch(moduleFileSystem.printTarget->targetType)
    {
        case CWS:
            sliceIndex_Offset = 0;

            sprintf(path,"%s/%s%0*d.png", moduleFileSystem.printTarget->targetPath, \
                                          moduleFileSystem.printTarget->projectName, \
                                          (int)4,
                                          sliceIndex_Offset + sliceIndex);
            break;
        case NETFABB:
            sliceIndex_Offset = 0;

            sprintf(path,"%s/layer_%0*d.png", moduleFileSystem.printTarget->targetPath, \
                                            (int)numberSpace, sliceIndex_Offset + sliceIndex);
            break;
        case B9:
            break;
        case SN3D:
            sliceIndex_Offset = 1;
            sprintf(path,"%s/%0*d.png", moduleFileSystem.printTarget->targetPath, \
                                      (int)0,
                                      sliceIndex_Offset + sliceIndex);
            break;
        default:
            SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_OK, "Invalid File Type.");
            break;
    }

    //printf("IMAGE PATH : %s\n", path);

    return path;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_TargetDestroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sTargetDestroy();

    return retStatus;
}

const printTarget_t* SN_MODULE_FILE_SYSTEM_TargetGet(void)
{
    if(moduleFileSystem.printTarget == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Target not loaded.");
    }

    return moduleFileSystem.printTarget;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Module Thread
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void* sFileSystemThread()
{
    SN_STATUS retStatus = SN_STATUS_OK;
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MESSAGE_Q_Get(msgQIdFileSystem);

        switch(evt.evt_id)
        {
            case MSG_FILE_SYSTEM_USB_MOUNT:
                SN_SYS_ERROR_SystemLog("File System => Module => USB Mount Event.");

                retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
                SN_SYS_ERROR_StatusCheck(retStatus, "File System Message Send Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_USB_MOUNT);
                SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");

                break;
            case MSG_FILE_SYSTEM_USB_UNMOUNT:
                SN_SYS_ERROR_SystemLog("File System => Module => USB Unmount Event.");

                retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
                SN_SYS_ERROR_StatusCheck(retStatus, "File System Message Send Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT);
                SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");

                break;

            case MSG_FILE_SYSTEM_READ:
                sFilePageDestroy(&moduleFileSystem.fileSystem);

                sFilePageLoad(&moduleFileSystem.fileSystem);

                sFileSystemPrint(&moduleFileSystem.fileSystem);

                retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, 0);
                SN_SYS_ERROR_StatusCheck(retStatus, "File System Message Send Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_READ_DONE);
                SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_UPDATE:
                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_UPDATE);
                SN_SYS_ERROR_StatusCheck(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_WAITING:
                break;
            default:
                SN_SYS_ERROR_StatusCheck(SN_STATUS_UNKNOWN_MESSAGE, "File System Get Unknown Message.");
                break;
        }

        SN_SYS_ERROR_StatusCheck(retStatus, "File System Module Get Error.");
    }

    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  USB Callback
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static void* USBEvent_Callback(int evt)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_ERROR_SystemLog("Get USB Event");

    switch(evt)
    {
        case MSG_USB_EVT_MOUNT:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_MOUNT, 0);
            SN_SYS_ERROR_StatusCheck(retStatus, "File System Send Message Fiailed.");
            break;
        case MSG_USB_EVT_UNMOUNT:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_UNMOUNT, 0);
            SN_SYS_ERROR_StatusCheck(retStatus, "File System Send Message Fiailed.");
            break;
        case MSG_USB_EVT_WAITING:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, 0);
            SN_SYS_ERROR_StatusCheck(retStatus, "File System Send Message Fiailed.");
            break;
        default:
            SN_SYS_ERROR_StatusCheck(SN_STATUS_UNKNOWN_MESSAGE, "Unknown USB Event.");
            break;
    }
    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  FILE SYSTEM
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sMachineInfoPageLoad(fileSystem_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;

    char path[MAX_PATH_LENGTH];
    char* nameBuffer = NULL;

    fsPageHeader_t* pageHeader = NULL;
    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "fileSysetm is NULL");
    }

    /* Page Init */
    pageHeader = FileSystem_PageInit();
    if(pageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "pageHeader init failed");
    }

    /* Add First Page */
    FileSystem_AddPage(pageHeader);
    if(pageHeader->firstPage == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "first page is can't open.");
    }
    currentPage = pageHeader->firstPage;

    numberOfnameList = scandir(MACHINE_FOLDER_PATH, &nameList, 0, alphasort);
    if(numberOfnameList <= 1)
    {
        perror("scandir");
        retStatus = FileSystem_fctl_CreateDircetoryTree(MACHINE_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        sprintf(path,"%s/%s.%s", MACHINE_FOLDER_PATH, MACHINE_FILE_NAME, MACHINE_FILE_EXT);
        printf("\n\n %s \n\n", path);

        FileSystem_fctl_CopyFile(DEFAULT_MACHINE_FILE_PATH, path);

        return sMachineInfoPageLoad(fileSystem);
    }
    else
    {
        for(i = 0; i < numberOfnameList; i++)
        {
            if(currentPage->itemCnt >= MAX_ITEM_SIZE)
            {
                FileSystem_AddPage(pageHeader);
                currentPage = currentPage->nextPage;
            }

            if(!strcmp(MACHINE_FILE_EXT, FileSystem_fctl_ExtractFileExtention(nameList[i]->d_name)))
            {
                nameBuffer = FileSystem_fctl_Extarct_FileName(nameList[i]->d_name);
                strcpy(currentPage->item[currentPage->itemCnt].name, nameBuffer);
                free(nameBuffer);
                nameBuffer = NULL;

                sprintf(path,"%s/%s.%s", MACHINE_FOLDER_PATH, currentPage->item[currentPage->itemCnt].name, MACHINE_FILE_EXT);
                currentPage->item[currentPage->itemCnt].contents = FileSystem_machineInfoXMLLoad(path);

                currentPage->itemCnt++;
                pageHeader->itemCnt++;
            }

            free(nameList[i]);
            nameList[i] = NULL;
        }
        free(nameList);
        nameList = NULL;
    }

    fileSystem->machineInfoPageHeader = pageHeader;

    return retStatus;
}

static SN_STATUS sMachineInfoPageDestroy(fileSystem_t* fileSystem)
{
    if(fileSystem->machineInfoPageHeader != NULL)
    {
        FileSystem_PageDestroy(fileSystem->machineInfoPageHeader);
    }

    return SN_STATUS_OK;
}


static SN_STATUS sOptionPageLoad(fileSystem_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;

    char path[MAX_PATH_LENGTH];
    char* nameBuffer = NULL;

    fsPageHeader_t* pageHeader = NULL;
    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "fileSysetm is NULL");
    }

    /* Page Init */
    pageHeader = FileSystem_PageInit();
    if(pageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "pageHeader init failed");
    }

    /* Add First Page */
    FileSystem_AddPage(pageHeader);
    if(pageHeader->firstPage == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "first page is can't open.");
    }
    currentPage = pageHeader->firstPage;

    numberOfnameList = scandir(OPTION_FOLDER_PATH, &nameList, 0, alphasort);
    if(numberOfnameList <= 1)
    {
        perror("scandir");
        retStatus = FileSystem_fctl_CreateDircetoryTree(OPTION_FOLDER_PATH);
        SN_SYS_ERROR_StatusCheck(retStatus, "Directory Create Failed.");

        sprintf(path,"%s/%s.%s", OPTION_FOLDER_PATH, OPTION_FILE_NAME, OPTION_FILE_EXT);
        printf("\n\n %s \n\n", path);

        FileSystem_fctl_CopyFile(DEFAULT_OPTION_FILE_PATH, path);

        return sOptionPageLoad(fileSystem);
    }
    else
    {
        for(i = 0; i < numberOfnameList; i++)
        {
            if(currentPage->itemCnt >= MAX_ITEM_SIZE)
            {
                FileSystem_AddPage(pageHeader);
                currentPage = currentPage->nextPage;
            }

            if(!strcmp(OPTION_FILE_EXT, FileSystem_fctl_ExtractFileExtention(nameList[i]->d_name)))
            {
                nameBuffer = FileSystem_fctl_Extarct_FileName(nameList[i]->d_name);
                strcpy(currentPage->item[currentPage->itemCnt].name, nameBuffer);
                free(nameBuffer);
                nameBuffer = NULL;

                sprintf(path,"%s/%s.%s", OPTION_FOLDER_PATH, currentPage->item[currentPage->itemCnt].name, OPTION_FILE_EXT);
                currentPage->item[currentPage->itemCnt].contents = (printOption_t *)FileSystem_optionXMLLoad(path);

                currentPage->itemCnt++;
                pageHeader->itemCnt++;
            }

            free(nameList[i]);
            nameList[i] = NULL;
        }
        free(nameList);
        nameList = NULL;
    }

    fileSystem->optionPageHeader = pageHeader;

    return retStatus;
}

static SN_STATUS sOptionPageDestroy(fileSystem_t* fileSystem)
{
    if(fileSystem->optionPageHeader != NULL)
    {
        FileSystem_PageDestroy(fileSystem->optionPageHeader);
    }

    return SN_STATUS_OK;
}

static SN_STATUS sFilePageLoad(fileSystem_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    struct dirent **nameList;
    int numberOfnameList = 0;
    int i = 0;
    int scandirErrorChecker = 0;

    fsPageHeader_t* pageHeader = NULL;
    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_INVALID_PARAM, "fileSysetm is NULL");
    }

    /* Page Init */
    pageHeader = FileSystem_PageInit();
    if(pageHeader == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "pageHeader init failed");
    }

    /* Add First Page */
    FileSystem_AddPage(pageHeader);
    if(pageHeader->firstPage == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "first page is can't open.");
    }
    currentPage = pageHeader->firstPage;

    numberOfnameList = scandir(USB_FOLDER_PATH, &nameList, 0, alphasort);
    if(numberOfnameList < 0)
    {
        while(scandirErrorChecker <= 5)
        {
            SN_SYS_TIMER_Delay(300);
            scandirErrorChecker++;
            numberOfnameList = scandir(USB_FOLDER_PATH, &nameList, 0, alphasort);

            if(numberOfnameList >= 0) break;
        }
    }

    if(numberOfnameList < 0)
    {
        perror("scandir");
    }
    else
    {
        for(i = 0; i < numberOfnameList; i++)
        {
            if(currentPage->itemCnt >= MAX_ITEM_SIZE)
            {
                FileSystem_AddPage(pageHeader);
                currentPage = currentPage->nextPage;
            }

            if((!strcmp(TARGET_CWS_FILE_EXT, FileSystem_fctl_ExtractFileExtention(nameList[i]->d_name)) || \
               !strcmp(TARGET_ZIP_FILE_EXT, FileSystem_fctl_ExtractFileExtention(nameList[i]->d_name))) && \
               !strstr(nameList[i]->d_name, SN3D_FW_STR) && \
               !strstr(nameList[i]->d_name, SN3D_OPTION_STR))
            {
                strcpy(currentPage->item[currentPage->itemCnt].name, nameList[i]->d_name);

                currentPage->itemCnt++;
                pageHeader->itemCnt++;
            }

            free(nameList[i]);
            nameList[i] = NULL;
        }
        free(nameList);
        nameList = NULL;
    }

    fileSystem->filePageHeader = pageHeader;

    return retStatus;
}

static SN_STATUS sFilePageDestroy(fileSystem_t* fileSystem)
{
    if(fileSystem->filePageHeader != NULL)
    {
        FileSystem_PageDestroy(fileSystem->filePageHeader);
    }

    return SN_STATUS_OK;
}

static SN_STATUS sFileSystemPrint(const fileSystem_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    uint32_t pageIndex = 0;
    uint32_t itemIndex = 0;

    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if(fileSystem->filePageHeader != NULL)
    {
        currentPage = fileSystem->filePageHeader->firstPage;

        printf("\nFile\n");
        for(pageIndex = 0; pageIndex < fileSystem->filePageHeader->pageCnt; pageIndex++)
        {
            printf("Page[%d]", pageIndex + 1);
            for(itemIndex = 0; itemIndex < currentPage->itemCnt; itemIndex++)
            {
                printf("  ---item [%d] %s\n", (itemIndex + 1),currentPage->item[itemIndex].name); fflush(stdout);
            }

            printf("max item[%d]\n", currentPage->itemCnt);

            currentPage = currentPage->nextPage;
        }

        printf("max page[%d]\n", fileSystem->filePageHeader->pageCnt);

    }
    if(fileSystem->optionPageHeader != NULL)
    {
        currentPage = fileSystem->optionPageHeader->firstPage;

        printf("\nOption\n");
        for(pageIndex = 0; pageIndex < fileSystem->optionPageHeader->pageCnt; pageIndex++)
        {
            for(itemIndex = 0; itemIndex < currentPage->itemCnt; itemIndex++)
            {
                printf("  ---Option [%d] %s\n", (itemIndex + 1),currentPage->item[itemIndex].name); fflush(stdout);
            }
            currentPage = currentPage->nextPage;
        }
    }

    if(fileSystem->machineInfoPageHeader != NULL)
    {
        currentPage = fileSystem->machineInfoPageHeader->firstPage;

        printf("\nMachine Config\n");
        for(pageIndex = 0; pageIndex < fileSystem->machineInfoPageHeader->pageCnt; pageIndex++)
        {
            for(itemIndex = 0; itemIndex < currentPage->itemCnt; itemIndex++)
            {
                printf("  ---Config [%d] %s\n", (itemIndex + 1),currentPage->item[itemIndex].name); fflush(stdout);
            }
            currentPage = currentPage->nextPage;
        }
    }

    return retStatus;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  OPTION
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sOptionLoad(uint32_t optionIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    const fsItem_t optionFile = FileSystem_GetItem(moduleFileSystem.fileSystem.optionPageHeader, optionIndex / MAX_ITEM_SIZE, optionIndex % MAX_ITEM_SIZE);

    moduleFileSystem.printOption = optionFile.contents;

    printf("\n\nOption Name : %s\n", moduleFileSystem.printOption->name);
    printf("Bottom Layer Exposure Time : %ld\n", moduleFileSystem.printOption->bottomLayerExposureTime);
    printf("Bottom Layer Number : %ld\n", moduleFileSystem.printOption->bottomLayerNumber);
    printf("Bottom Layer FeedRate : %f\n", moduleFileSystem.printOption->bottomLiftFeedRate);
    printf("Layer Exposure Time : %ld\n", moduleFileSystem.printOption->layerExposureTime);
    printf("Layer Thickness: %f \n", moduleFileSystem.printOption->layerThickness);
    printf("Lift Distance : %ld \n", moduleFileSystem.printOption->liftDistance);
    printf("Lift TIme : %ld \n", moduleFileSystem.printOption->liftTime);
    printf("Lift Feed Rate : %f \n", moduleFileSystem.printOption->liftFeedRate);
    printf("Bright : %ld \n\n", moduleFileSystem.printOption->bright);

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  MACHINE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sMachineInfoLoad(uint32_t machineInfoIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    const fsItem_t machineInfoFile = FileSystem_GetItem(moduleFileSystem.fileSystem.machineInfoPageHeader, machineInfoIndex / MAX_ITEM_SIZE, machineInfoIndex % MAX_ITEM_SIZE);

    moduleFileSystem.machineInfo = machineInfoFile.contents;

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  VERSION
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sVersionInfoLoad(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char path[MAX_PATH_LENGTH];

    sprintf(path, "%s/%s.%s", VERSION_FOLDER_PATH, SN3D_VERSION_STR, SN3D_VERSION_EXTENTION);

    moduleFileSystem.versionInfo = FileSystem_versionInfoXMLLoad(path);

    return retStatus;
}

static bool sVersionHashCheck(void)
{
    bool hashMatched = false;

    unsigned char* hash = NULL;
    unsigned char* hashStr = NULL;

    hash = FileSysetm_MD5_Hash_WithFile(BINARY_FOLDER_PATH, NULL);
    if(hash == NULL)
    {
        return false;
    }

    hashStr = FileSystem_MD5_HashToString(hash);
    if(hashStr == NULL)
    {
        return false;
    }

    printf(" CALCULATED   HASH :: %s \n", hashStr);
    printf(" VERSION INFO HASH :: %s \n", moduleFileSystem.versionInfo->hash);

    if(!strcmp((const char*)hashStr, (const char*)moduleFileSystem.versionInfo->hash) || \
       !strcmp((const char*)HASH_MAGIC_NUMBER, (const char*)moduleFileSystem.versionInfo->hash))
    {
        printf("\n\n Hash is matching!! \n\n");
        hashMatched = true;
    }
    else
    {
        printf("\n\n Hash is not matching!! \n\n");
        hashMatched = false;
    }

    free(hash);
    free(hashStr);

    return hashMatched;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  DEVICE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sDeviceInfoLoad(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char path[MAX_PATH_LENGTH];

    sprintf(path,"%s/%s.%s", DEVICE_FOLDER_PATH, DEVICE_FILE_NAME, DEVICE_FILE_EXT);
    moduleFileSystem.deviceInfo = FileSystem_deviceInfoXMLLoad(path);
    if(moduleFileSystem.deviceInfo == NULL)
    {
        retStatus = SN_STATUS_NOT_OK;
    }

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  TARGET
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
float GetTargetLayerThickness(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;


    FILE *fp;

    int line_num = 1;
    int find_result = 0;

    char path[MAX_PATH_LENGTH];
    char lineBuff[MAX_BUFFER_SIZE];
    char* linePointer = NULL;

    int valueIndex = 0;

    float layerThickness = 0.0;


    sprintf(path,"%s/%s.%s", TARGET_FOLDER_PATH, TARGET_CONFIG_FILE_NAME, TARGET_CONFIG_EXT);

    printf("\n%s\n", path);

    fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("\n%s\n", path);
        printf("\nCAN'T SEARCHING TARGET THINCKNESS CONFIG FILE, SO USE DEFAULT VALUE.\n");

        return 0.0;
    }

    while(fgets(lineBuff, MAX_BUFFER_SIZE, fp) != NULL)
    {
        linePointer = strstr(lineBuff, TARGET_CONFIG_STRING);

        while(linePointer != NULL)
        {
            printf("\n%s\n", linePointer);
            valueIndex++;

            if(valueIndex >= TARGET_THICKNESS)
            {
                sscanf((const char *)linePointer + strlen(TARGET_CONFIG_STRING), "%f", &layerThickness);

                /* mm to um */
                layerThickness /= 1000;
                printf("\nFOUND TARGET THICKNESS CONFIG FILE-! SO USE NEW VALUE IS [ %f ].\n", layerThickness);

                break;
            }
            else
            {
                linePointer = strstr(linePointer + 1, TARGET_CONFIG_STRING);
            }
        }
    }

    if(fp)
    {
        fclose(fp);
    }

    return layerThickness;
}

static SN_STATUS sTargetLoad(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    const fsItem_t TargetFile = FileSystem_GetItem(moduleFileSystem.fileSystem.filePageHeader, pageIndex, itemIndex);
    printTarget_t* printTarget = NULL;

    char* nameBuffer = NULL;

    printTarget = (printTarget_t *)malloc(sizeof(printTarget_t));
    if(printTarget == NULL)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "printTarget memroy allocate failed.");
    }

    /* Target Info Setting. */
    strcpy(printTarget->targetPath, TARGET_FOLDER_PATH);
    strcpy(printTarget->targetName, TargetFile.name);

    nameBuffer = FileSystem_fctl_Extarct_FileName(TargetFile.name);
    strcpy(printTarget->projectName, FileSystem_fctl_Extarct_FileName(TargetFile.name));
    free(nameBuffer);
    nameBuffer = NULL;

    retStatus = sTargetFileCreate(printTarget->projectName, FileSystem_fctl_ExtractFileExtention(TargetFile.name));
    SN_SYS_ERROR_StatusCheck(retStatus, "Faild Create Target Files.");

    if(FileSystem_CountFileWithStr(TARGET_FOLDER_PATH, CWS_CONDITION_STR))
    {
        printTarget->targetType = CWS;
    }
    else if(FileSystem_CountFileWithStr(TARGET_FOLDER_PATH, NETFABB_CONDITION_STR))
    {
        printTarget->targetType = NETFABB;
        strcpy(printTarget->projectName, printTarget->targetName);
    }
    /*
    else if(FileSystem_CountFileWithStr(TARGET_FOLDER_PATH, B9_CONDITION_STR))
    {
        //B9 Not Support yet.
    }
    */
    else
    {
        printTarget->targetType = SN3D;
        strcpy(printTarget->projectName, printTarget->targetName);

        printTarget->layerThickness = GetTargetLayerThickness();
    }

    printTarget->slice                      = FileSystem_CountFileWithStr(TARGET_FOLDER_PATH, TARGET_IMAGE_EXT);

    moduleFileSystem.printTarget = printTarget;

    return retStatus;
}

static SN_STATUS sTargetDestroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleFileSystem.printTarget != NULL)
    {
        free(moduleFileSystem.printTarget);
        moduleFileSystem.printTarget = NULL;
    }

    moduleFileSystem.printTarget = NULL;

    /* Clean Folder */
    retStatus = FileSystem_fctl_RemoveFiles(TARGET_FOLDER_PATH);

    return retStatus;
}

static SN_STATUS sTargetFileCreate(const char* fileName, const char* fileExtention)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char srcTargetPath[MAX_PATH_LENGTH], desTargetPath[MAX_PATH_LENGTH];

    /* Folder Check */
    retStatus = FileSystem_fctl_CreateDircetoryTree(TARGET_FOLDER_PATH);

    /* Clean Folder */
    retStatus = FileSystem_fctl_RemoveFiles(TARGET_FOLDER_PATH);

    /* Get Source Path */
    sprintf(srcTargetPath,"%s/%s.%s", USB_FOLDER_PATH, fileName, fileExtention);
    sprintf(desTargetPath,"%s/%s.%s", TARGET_FOLDER_PATH, fileName, fileExtention);

    /* Create Target Files */
    retStatus = FileSystem_fctl_CopyFile(srcTargetPath, desTargetPath);
    retStatus = FileSystem_fctl_ExtractFile(desTargetPath, TARGET_FOLDER_PATH);

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 * SYSTEM
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage)
{
    return SN_SYS_MESSAGE_Q_Put(msgQIdFileSystem, evtId, evtMessage);
}

