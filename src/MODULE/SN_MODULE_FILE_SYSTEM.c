/**
 * @file SN_MODULE_FILE_SYSTEM.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo read XML File and Option File.
 * @todo read machine XML File.
 * @todo target folder check.
 * @todo optionConfig folder check.
 * @todo deviceConfigs folder check.
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#include "FILE_SYSTEM_FCTL.h"
#include "FILE_SYSTEM_PAGE.h"
#include "FILE_SYSTEM_XML.h"

/* ******* STATIC DEFINE ******* */
/** @name USB Driver Config *////@{
#ifdef __APPLE__
#define USB_PATH              "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_PATH              "/mnt/volume"
#endif

#ifdef __APPLE__
#define TARGET_PATH              "../res/tempFile"
#endif
#ifdef linux
#define TARGET_PATH              "/SN3D/sn3d-project/res/target"
#endif

#ifdef __APPLE__
#define OPTION_FILE_PATH              "../res/profileConfig"
#endif
#ifdef linux
#define OPTION_FILE_PATH              "/SN3D/sn3d-project/res/profileConfig"
#endif

#ifdef __APPLE__
#define DEVICE_FILE_PATH              "../res/deviceConfig"
#endif
#ifdef linux
#define DEVICE_FILE_PATH              "/SN3D/sn3d-project/res/deviceConfig"
#endif
///@}

/** @name File path & name config *////@{
#define TARGET_CWS_FILE_EXT     "cws"
#define TARGET_ZIP_FILE_EXT     "zip"
#define TARGET_IMAGE_EXT        "png"

#define MANIFEST_FILE_NAME      "manifest"
#define MANIFEST_FILE_EXT       "xml"

#define DEVICE_FILE_EXT         "xml"

#define OPTION_FILE_EXT         "xml"
///@}

/** @name Z config *////@{
#define Z_DELAY_OFFSET (1600)

/** @def SPEED_MM_MIN_TO_MM_MSEC(speed_mm_min)
 *  @brief mm/m to mm/s
 *
 *  @param speed_mm_min
 *
 *  @return mm/s
 */
#define SPEED_MM_MIN_TO_MM_MSEC(speed_mm_min) \
        ((speed_mm_min) / (60 * 1000))

/** @def Z_DELAY_MSEC_CAL(distnace, speed)
 *  @brief calculate z delay
 *
 *  @param distance
 *  @param speed
 *
 *  @return z_delay (msec)
 */
#define Z_DELAY_MSEC_CAL(distnace, speed) \
    ((((distnace) * 2) / (SPEED_MM_MIN_TO_MM_MSEC(speed))) + Z_DELAY_OFFSET)
///@}

/** @name Other Define *////@{
#define DEFAULT_DEVICE_NAME "POLARIS 500"
///@}

/* *** MODULE *** */
typedef struct moduel_file_system {
    fs_t                     fs;
    printInfo_t       printInfo;
    machineInfo_t   machineInfo;
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
static void*       USBEvent_Callback(int evt);

/* *** SYSTEM *** */
static void*       sFileSystemThread();
static SN_STATUS   sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage);

/* *** FILE SYSTEM CONTROL *** */
static SN_STATUS   sFileSystemRead(fs_t* fileSystem);
static SN_STATUS   sFileSystemRemove(fs_t* fileSystem);
static SN_STATUS   sFileSystemPrint(const fs_t* fileSystem);

/* *** DEMO *** */
static void        sDemoPrintSetting(void);
static void        sDemoMachineSetting(void);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

bool SN_MODULE_FILE_SYSTEM_isItemExist(void)
{
    return moduleFileSystem.fs.pageHeader->isItemExist;
}

int SN_MODULE_FILE_SYSTEM_GetPageCnt(void)
{
    return moduleFileSystem.fs.pageHeader->pageCnt;
}

const fsPage_t* SN_MODULE_FILE_SYSTEM_GetPage(int pageIndex)
{
    return FileSystem_GetPage(moduleFileSystem.fs.pageHeader, pageIndex);
}

const fs_t SN_MODULE_FILE_SYSTEM_GetFileSystem(void)
{
    return moduleFileSystem.fs;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_Update(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
    SN_SYS_ERROR_CHECK(retStatus, "File System Send Message Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //@DEMO SETTING
    sDemoMachineSetting();

    moduleFileSystem.machineInfo.isInit = true;

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex/*,  uint32_t optionIndex */)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char srcTargetPath[MAX_PATH_LENGTH], desTargetPath[MAX_PATH_LENGTH];
    //char optionFilePath[MAX_PATH_LENGTH];
    char manifestPath[MAX_PATH_LENGTH];

    const fsItem_t target = FileSystem_GetItem(moduleFileSystem.fs.pageHeader, pageIndex, itemIndex);

    retStatus = FileSystem_fctl_RemoveFiles(TARGET_PATH);

    sprintf(srcTargetPath,"%s/%s.%s", USB_PATH, target.name, TARGET_CWS_FILE_EXT);

    sprintf(desTargetPath,"%s/%s.%s", TARGET_PATH, target.name, TARGET_CWS_FILE_EXT);

    retStatus = FileSystem_fctl_CopyFile(srcTargetPath, desTargetPath);
    retStatus = FileSystem_fctl_ExtractFile(desTargetPath, TARGET_PATH);

    /** @todo target file checker (mango netfabb cws 9b) */
    sprintf(manifestPath,"%s/%s.%s", TARGET_PATH, MANIFEST_FILE_NAME, MANIFEST_FILE_EXT);

    //@DEMO SETTING
    sDemoPrintSetting(); // Option Demo

    /* Target */
    moduleFileSystem.printInfo.printTarget.targetPath                 = TARGET_PATH;
    //moduleFileSystem.printInfo.printTarget.targetName                 = sParseXML_TargetName(manifestPath);
    moduleFileSystem.printInfo.printTarget.targetName                 = target.name;
    moduleFileSystem.printInfo.printTarget.slice                      = FileSystem_CountFileNumWithExtetion(TARGET_PATH, TARGET_IMAGE_EXT);


    moduleFileSystem.printInfo.isInit = true;

    return retStatus;
}

machineInfo_t SN_MODULE_FILE_SYSTEM_MachineInfoGet(void)
{
    return moduleFileSystem.machineInfo;
}

printInfo_t SN_MODULE_FILE_SYSTEM_PrintInfoGet(void)
{
    return moduleFileSystem.printInfo;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleFileSystem.machineInfo.isInit)
    {
        moduleFileSystem.machineInfo.isInit = false;
    }

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoUninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleFileSystem.printInfo.isInit)
    {
        moduleFileSystem.printInfo.isInit = false;
        FileSystem_fctl_RemoveFiles(TARGET_PATH);
    }

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SN_SYS_Log("MODULE INIT => FILE SYSTEM.");


    /* USB DRIVER INIT */
    SN_SYS_USBDriverInit(USBEvent_Callback);

    /* MESSAGE Q INIT */
    retStatus = SN_SYS_MessageQInit(&msgQIdFileSystem);
    SN_SYS_ERROR_CHECK(retStatus, "File System Module Message Q Init Failed.");

    /* MUTEX INIT */
    if (pthread_mutex_init(&ptmFileSystem, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "File System Mutex Init Failed.");
    }

    /* THREAD INIT */
    if((retStatus = pthread_create(&ptFileSystem, NULL, sFileSystemThread, NULL)))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "File System Thread Init Failed.");
    }

    /* MACHINE INFO INIT */
    SN_MODULE_FILE_SYSTEM_MachineInfoInit();

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void)
{
    SN_SYS_MessageQRemove(&msgQIdFileSystem);

    return SN_STATUS_OK;
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
        evt = SN_SYS_MessageGet(&msgQIdFileSystem);

        switch(evt.evt_id)
        {
            case MSG_FILE_SYSTEM_USB_MOUNT:
                SN_SYS_Log("File System => Module => USB Mount.");

                SN_SYS_Delay(2000);

                retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
                SN_SYS_ERROR_CHECK(retStatus, "File System Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_USB_UNMOUNT:

                SN_SYS_Log("File System => Module => USB Unmount.");
                retStatus = sFileSystemRemove(&moduleFileSystem.fs);
                SN_SYS_ERROR_CHECK(retStatus, "File System Remove Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT);
                SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_READ:
                retStatus = sFileSystemRead(&moduleFileSystem.fs);
                SN_SYS_ERROR_CHECK(retStatus, "File System Read Failed.");

                retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, 0);
                SN_SYS_ERROR_CHECK(retStatus, "File System Message Send Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_READ_DONE);
                SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_UPDATE:
                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_UPDATE);
                SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_WAITING:
                break;
            default:
                SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "File System Get Unknown Message.");
                break;
        }

        SN_SYS_ERROR_CHECK(retStatus, "File System Module Get Error.");
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

    switch(evt)
    {
        case MSG_USB_EVT_MOUNT:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_MOUNT, 0);
            SN_SYS_ERROR_CHECK(retStatus, "File System Send Message Fiailed.");
            break;
        case MSG_USB_EVT_UNMOUNT:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_UNMOUNT, 0);
            SN_SYS_ERROR_CHECK(retStatus, "File System Send Message Fiailed.");
            break;
        case MSG_USB_EVT_WAITING:
            retStatus = sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, 0);
            SN_SYS_ERROR_CHECK(retStatus, "File System Send Message Fiailed.");
            break;
        default:
            SN_SYS_ERROR_CHECK(SN_STATUS_UNKNOWN_MESSAGE, "Unknown USB Event.");
            break;
    }
    return NULL;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  FILE SYSTEM CONTROL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sFileSystemRead(fs_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    DIR *dp;
    struct dirent *ep;
    char* nameBuffer = NULL;

    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    dp = opendir(USB_PATH);

    sFileSystemRemove(fileSystem);

    /* Page Init */
    fileSystem->pageHeader = FileSystem_PageInit();
    if(fileSystem->pageHeader == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "pageHeader init failed");
    }



    if (dp != NULL)
    {
        /* Add First Page */
        FileSystem_AddPage(fileSystem->pageHeader);
        if(fileSystem->pageHeader->firstPage == NULL)
        {
            SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "first page is can't open.");
        }
        currentPage = fileSystem->pageHeader->firstPage;

        while((ep = readdir (dp)))
        {
            nameBuffer = ep->d_name;

            if(!strcmp(TARGET_CWS_FILE_EXT, FileSystem_fctl_ExtractFileExtention(nameBuffer)))
            {
                strcpy(currentPage->item[currentPage->itemCnt].name, FileSystem_fctl_Extarct_FileName(nameBuffer));

                if((currentPage->itemCnt + 1) >= MAX_ITEM_SIZE)
                {
                    FileSystem_AddPage(fileSystem->pageHeader);
                    currentPage = currentPage->nextPage;
                }
                else
                {
                    currentPage->itemCnt++;
                }
            }
        }
        (void) closedir (dp);
    }
    else
    {
        perror ("Couldn't open the directory");
    }


    fileSystem->pageHeader->isItemExist = (fileSystem->pageHeader->firstPage->itemCnt != 0);

    sFileSystemPrint(fileSystem);

    return retStatus;
}

static SN_STATUS sFileSystemRemove(fs_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    FileSystem_PageDestroy(fileSystem->pageHeader);

    return retStatus;
}


static SN_STATUS sFileSystemPrint(const fs_t* fileSystem)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int pageIndex = 0;
    int itemIndex = 0;

    fsPage_t* currentPage = NULL;

    if(fileSystem == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    currentPage = fileSystem->pageHeader->firstPage;

    for(pageIndex = 0; pageIndex < fileSystem->pageHeader->pageCnt; pageIndex++)
    {
        printf("\nPage    [%d]\n", (pageIndex + 1));

        for(itemIndex = 0; itemIndex < currentPage->itemCnt; itemIndex++)
        {
            printf("  ---item [%d] %s\n", (itemIndex + 1),currentPage->item[itemIndex].name); fflush(stdout);
        }

        printf("max item[%d]\n", currentPage->itemCnt);

        currentPage = currentPage->nextPage;
    }

    printf("max page[%d]\n", fileSystem->pageHeader->pageCnt);

    return retStatus;
}


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  DEMO
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sDemoPrintSetting(void)
{
    /* Base Paramter */
    moduleFileSystem.printInfo.printParameter.layerThickness          = 0.05000;//mm

    /* Bottom Layer */
    //moduleFileSystem.printInfo.printParameter.bottomLayerExposureTime =     50;//ms
    moduleFileSystem.printInfo.printParameter.bottomLayerExposureTime =   35000;//ms
    moduleFileSystem.printInfo.printParameter.bottomLayerNumber       =       7;//layer
    moduleFileSystem.printInfo.printParameter.bottomLiftFeedRate      =  150.00;//mm/s

    /* Normal Layer */
    moduleFileSystem.printInfo.printParameter.layerExposureTime       =    3000;//ms
    //moduleFileSystem.printInfo.printParameter.layerExposureTime       =      50;//ms
    moduleFileSystem.printInfo.printParameter.liftDistance            =       7;//mm
    moduleFileSystem.printInfo.printParameter.liftFeedRate            =  150.00;//mm/s

    moduleFileSystem.printInfo.printParameter.liftTime                =  Z_DELAY_MSEC_CAL( \
                             moduleFileSystem.printInfo.printParameter.liftDistance, \
                             moduleFileSystem.printInfo.printParameter.liftFeedRate);

}

static void sDemoMachineSetting(void)
{
    strcpy(moduleFileSystem.machineInfo.name, DEFAULT_DEVICE_NAME);
    moduleFileSystem.machineInfo.height                                =     200;//mm
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 * SYSTEM
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static SN_STATUS sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage)
{
    return SN_SYS_MessagePut(&msgQIdFileSystem, evtId, evtMessage);
}

