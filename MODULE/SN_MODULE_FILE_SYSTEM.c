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
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

/* ******* STATIC DEFINE ******* */
/** @name USB Driver Config *////@{
#ifdef __APPLE__
#define USB_PATH              "/Volumes/USB_0/"
#endif
#ifdef linux
#define USB_PATH              "/mnt/volume/"
#endif

#ifdef __APPLE__
#define TEMP_FILE_PATH              "../"
#endif
#ifdef linux
#define TEMP_FILE_PATH              "/SN3D/sn3d-project/tempFile/"
#endif
///@}

/** @name File path & name config *////@{
#define FILENAME_EXT         "cws"
#define IMAGE_FILENAME_EXT   "png"

#define CONFIG_FILENAME      "manifest"
#define CONFIG_FILENAME_EXT  "xml"
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
    MSG_FILE_SYSTEM_IGNORE          = 0xFF01    /**< Came From Timer - Don't Care */

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
static SN_STATUS   sFileSystemRead(void);
static SN_STATUS   sFileSystemRemove(void);
static SN_STATUS   sFileSystemPrint(void);

/* *** TEMP FILE CONTROL *** */
static uint32_t    sCountSlice(const char* srcPath);
static SN_STATUS   sCopyFile(const char* srcPath, const char* desPath);
static SN_STATUS   sRemoveTempFile(const char* filePath);
static SN_STATUS   sExtractTempFile(const char* srcPath, const char* desPath);

/* *** UTIL *** */
static const char* sGetFilenameExt(const char *filename);
static const char* sGetFilename(const char *filename);

/* *** DEMO *** */
static void        sDemoPrintSetting(void);
static void        sDemoMachineSetting(void);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS SN_MODULE_FILE_SYSTEM_Get(fs_t* pFs)
{
    if(pFs == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    *pFs = moduleFileSystem.fs;

    return SN_STATUS_OK;
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
    char srcPath[MAX_PATH_LENGTH], desPath[MAX_PATH_LENGTH];

    sprintf(srcPath,"%s%s.%s",  USB_PATH, \
                                moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, \
                                FILENAME_EXT); fflush(stdout);

    sprintf(desPath,"%s%s.%s",TEMP_FILE_PATH, \
                                moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, \
                                FILENAME_EXT); fflush(stdout);

    retStatus = sRemoveTempFile(TEMP_FILE_PATH);
    retStatus = sCopyFile(srcPath, desPath);
    retStatus = sExtractTempFile(desPath, TEMP_FILE_PATH);

    //@DEMO SETTING
    sDemoPrintSetting(); // Option Demo

    /* Target */
    moduleFileSystem.printInfo.printTarget.sourceFilePath             = USB_PATH;
    moduleFileSystem.printInfo.printTarget.tempFilePath               = TEMP_FILE_PATH;
    moduleFileSystem.printInfo.printTarget.tempFileName               = moduleFileSystem.fs.page[pageIndex].item[itemIndex].name;
    moduleFileSystem.printInfo.printTarget.slice                      = sCountSlice(TEMP_FILE_PATH);


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
        sRemoveTempFile(TEMP_FILE_PATH);
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
                retStatus = sFileSystemRemove();
                SN_SYS_ERROR_CHECK(retStatus, "File System Remove Failed.");

                retStatus = SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT);
                SN_SYS_ERROR_CHECK(retStatus, "App Message Send Failed.");
                break;
            case MSG_FILE_SYSTEM_READ:
                retStatus = sFileSystemRead();
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

            case MSG_FILE_SYSTEM_IGNORE:
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

static SN_STATUS sFileSystemRead(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    DIR *dp;
    struct dirent *ep;
    char* nameBuffer = NULL;
    int pageIndex = 0, itemIndex = 0;

    dp = opendir(USB_PATH);

    sFileSystemRemove();

    pageIndex = 0;

    if (dp != NULL)
    {
        while((ep = readdir (dp)))
        {
            nameBuffer = ep->d_name;

            if(!strcmp(FILENAME_EXT, sGetFilenameExt(nameBuffer)))
            {
                if(MAX_ITEM_SIZE <= itemIndex)
                {
                    if(MAX_PAGE_SIZE <= pageIndex)
                    {
                        break;
                    }
                    else
                    {
                        moduleFileSystem.fs.pageCnt++;
                        pageIndex++;
                        itemIndex = 0;
                    }
                }
                moduleFileSystem.fs.page[pageIndex].itemCnt++;
                strcpy(moduleFileSystem.fs.page[pageIndex].item[itemIndex++].name, sGetFilename(nameBuffer));
            }
        }
        (void) closedir (dp);
    }
    else
    {
        perror ("Couldn't open the directory");
    }

    if(moduleFileSystem.fs.page[0].itemCnt != 0)
    {
        moduleFileSystem.fs.isItemExist = true;
    }
    else
    {
        moduleFileSystem.fs.isItemExist = false;
    }

    sFileSystemPrint();

    return SN_STATUS_OK;
}

static SN_STATUS sFileSystemRemove(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int pageIndex = 0, itemIndex = 0;

    /* Init FS */
    for(pageIndex = 0; pageIndex <= moduleFileSystem.fs.pageCnt; pageIndex++)
    {
        moduleFileSystem.fs.page[pageIndex].itemCnt = 0;
    }
    moduleFileSystem.fs.pageCnt = 0;

    moduleFileSystem.fs.isItemExist = false;

    return retStatus;
}


static SN_STATUS sFileSystemPrint(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int pageIndex = 0;
    int itemIndex = 0;

    for(pageIndex = 0; pageIndex <= moduleFileSystem.fs.pageCnt; pageIndex++)
    {
        printf("\nPage    [%d]\n", pageIndex);
        for(itemIndex = 0; itemIndex < moduleFileSystem.fs.page[pageIndex].itemCnt; itemIndex++)
        {
            printf("  ---item [%d] %s\n", itemIndex, moduleFileSystem.fs.page[pageIndex].item[itemIndex].name); fflush(stdout);
        }
        printf("max item[%d]\n", moduleFileSystem.fs.page[pageIndex].itemCnt);
    }

    printf("max page[%d]\n", moduleFileSystem.fs.pageCnt);

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  TEMP FILE CONTROL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static uint32_t sCountSlice(const char* srcPath)
{
    DIR *d = opendir(srcPath);
    size_t path_len = strlen(srcPath);
    uint32_t slice = 0;

    int r = -1;

    if(d)
    {
        struct dirent *p;

        r = 0;

        while(!r &&( p=readdir(d)))
        {
            if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            if(strstr(p->d_name, IMAGE_FILENAME_EXT) != NULL)
            {
                slice++;
            }
        }
        closedir(d);
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
    }

    if(!r)
    {
        printf("Module => File System => Slice File Number : [ %04d ]\n", slice); fflush(stdout);
    }

    return slice;
}

SN_STATUS sCopyFile(const char* srcPath, const char* desPath)
{
    FILE* src = fopen(srcPath, "rb");
    FILE* des = fopen(desPath, "wb");
    char buf[1000];
    int readCnt = 0;

    if(src == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Source File Path Invalid.");
    }

    if(des == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
    }

    while(true)
    {
        readCnt = fread((void*)buf, 1, sizeof(buf), src);

        if(readCnt < sizeof(buf))
        {
            if(feof(src)!=0)
            {
                fwrite((void*)buf, 1, readCnt, des);
                SN_SYS_Log("Module => File System => MAKE TEMP FILE.");
                break;
            }
            else
            {
                break;
            }
        }

        fwrite((void*)buf,1, sizeof(buf), des);
    }

    fclose(src);
    fclose(des);

    return SN_STATUS_OK;
}

SN_STATUS sRemoveTempFile(const char* filePath)
{
    DIR *d = opendir(filePath);
    size_t path_len = strlen(filePath);
    int r = -1;

    if(d)
    {
        struct dirent *p;

        r = 0;

        while(!r &&(p=readdir(d)))
        {
            int r2 = -1;
            char *buf;
            size_t len;

            if(!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = malloc(len);

            if(buf)
            {
                struct stat statbuf;

                snprintf(buf, len,"%s%s", filePath, p->d_name);

                if(!stat(buf, &statbuf))
                {
                    if(S_ISDIR(statbuf.st_mode))
                    {
                        r2 = sRemoveTempFile(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }
    else
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Folder Open Failed.");
    }

    if(!r)
    {
        SN_SYS_Log("Module => File System => REMOVE TEMP FILE.");
    }

    return SN_STATUS_OK;
}

/*
static SN_STATUS sReadXMLFile(const char* srcPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    FILE* GCodeFile;
    char* line = NULL;
    size_t  len;
    ssize_t read;
    int i = 0;

    GCodeFile = fopen(srcPath, "r");

    if(GCodeFile != NULL)
    {
        printf("open gcode file faild.\n");
        return SN_STATUS_INVALID_PARAM;
    }

    while((read = getline(&line, &len, GCodeFile)) != -1)
    {
        printf("%d :: %s\n", i, line); fflush(stdout);
        i++;
    }

    fclose(GCodeFile);

    printf("%d :: %s\n", i, line); fflush(stdout);

    if(line)
    {
        free(line);
    }

    return retStatus;
}
*/

static void safe_create_dir(const char *dir)
{
    if(mkdir(dir, 0755) < 0) {
        if (errno != EEXIST) {
            SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
        }
    }
}

SN_STATUS sExtractTempFile(const char* srcPath, const char* desPath)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;

    char buf[1000];

    int err;
    int i, len;
    int fd;
    long long sum;
    char fullFilePath[MAX_PATH_LENGTH];

    if ((za = zip_open(srcPath, 0, &err)) == NULL)
    {
            SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Folder Open Failed.");
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++)
    {
        if (zip_stat_index(za, i, 0, &sb) == 0)
        {
            len = strlen(sb.name);

            if (sb.name[len - 1] == '/')
            {
                safe_create_dir(sb.name);
            }
            else
            {
                zf = zip_fopen_index(za, i, 0);

                if (!zf) {
                    SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
                }

                len = strlen(desPath) + strlen(sb.name);

                snprintf(fullFilePath, len + 2, "%s/%s", desPath, sb.name);

                fd = open(fullFilePath, O_RDWR | O_TRUNC | O_CREAT, 0644);

                if (fd < 0) {
                    SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "Temp File Path Invalid.");
                }

                sum = 0;

                while (sum != sb.size)
                {
                    len = zip_fread(zf, buf, 1000);
                    if (len < 0)
                    {
                        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
                    }
                    write(fd, buf, len);
                    sum += len;
                }

                close(fd);
                zip_fclose(zf);
            }
        }
    }

    if (zip_close(za) == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Temp File Extract Failed.");
    }

    SN_SYS_Log("Module => File System => EXTRACT TEMP FILE.");

    return retStatus;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  UTIL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static const char* sGetFilenameExt(const char *filename)
{
    const char *dot = strrchr(filename, '.');

    if(filename[0] == '.') return "";
    if(!dot || dot == filename) return "";
    return dot + 1;
}

static const char* sGetFilename(const char *filename)
{
    char *retstr;
    char *lastdot;

    if (filename == NULL)
         return NULL;

    if ((retstr = malloc (strlen (filename) + 1)) == NULL)
        return NULL;

    strcpy (retstr, filename);

    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';

    return retstr;
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
    moduleFileSystem.printInfo.printParameter.bottomLayerExposureTime =   35000;//ms
    moduleFileSystem.printInfo.printParameter.bottomLayerNumber       =       7;//layer
    moduleFileSystem.printInfo.printParameter.bottomLiftFeedRate      =  150.00;//mm/s

    /* Normal Layer */
    moduleFileSystem.printInfo.printParameter.layerExposureTime       =    3000;//ms
    moduleFileSystem.printInfo.printParameter.liftDistance            =       7;//mm
    moduleFileSystem.printInfo.printParameter.liftFeedRate            =  150.00;//mm/s

    moduleFileSystem.printInfo.printParameter.liftTime                =  Z_DELAY_MSEC_CAL( \
                             moduleFileSystem.printInfo.printParameter.liftDistance, \
                             moduleFileSystem.printInfo.printParameter.liftFeedRate);

}

static void sDemoMachineSetting(void)
{
    strcpy(moduleFileSystem.machineInfo.name, DEFAULT_DEVICE_NAME);
    moduleFileSystem.machineInfo.height           =          200;//mm
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

