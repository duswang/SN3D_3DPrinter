/*
 * MODUEL_FILE_SYSTEM.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_MODULE_FILE_SYSTEM.h"

/** System **/
/* Thread */
pthread_mutex_t ptmFileSystem = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptFileSystem;

/* Message */
sysMessageQId msgQIdFileSystem;

/** Moduel Controller **/
static moduleFileSystem_t moduleFileSystem;

/** Static Functions **/
/* callback */
static void*       USBEvent_callback(int evt);

/* System */
static void        sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage);
static void*       sFileSystemThread();

/* Local */
static SN_STATUS   sFileSystemRead(void);
static void        sFileSystemPrint(void);

static SN_STATUS   sReadGCodeFile(const char* srcPath);

/* Util*/
static SN_STATUS   sCopyFile(const char* srcPath, const char* desPath);
static SN_STATUS   sRemoveTempFile(const char* filePath);
static SN_STATUS   sExtractTempFile(const char* srcPath, const char* desPath);
static uint32_t    sCountSlice(const char* srcPath);

/* Util */
static const char* sGetFilenameExt(const char *filename);
static const char* sGetFilename(const char *filename);

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
    sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);

    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //sMachineInfoRead();
    //@DEBUG

    moduleFileSystem.machineInfo.isInit = true;
    moduleFileSystem.machineInfo.deviceParameter.weight   =         1920;
    moduleFileSystem.machineInfo.deviceParameter.height   =         1080;

    moduleFileSystem.machineInfo.deviceTarget.machineName =  DEVICE_NAME;

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char srcPath[50];
    char desPath[50];

    //sPrintInfoRead();

    sprintf(srcPath,"%s%s.%s",  USB_PATH, \
                                moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, \
                                FILENAME_EXT); fflush(stdout);

    sprintf(desPath,"%s%s.%s",TEMP_FILE_PATH, \
                                moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, \
                                FILENAME_EXT); fflush(stdout);

    sRemoveTempFile(TEMP_FILE_PATH);
    sCopyFile(srcPath, desPath);
    sExtractTempFile(desPath, TEMP_FILE_PATH);

    sprintf(srcPath,"%s%s.%s", TEMP_FILE_PATH, \
                               moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, \
                               CONFIG_FILENAME_EXT); fflush(stdout);
    //@DEBUG
    //sReadGCodeFile(TEMP_FILE_PATH);

    moduleFileSystem.printInfo.printTarget.slice                      = sCountSlice(TEMP_FILE_PATH);

    /** Parameter **/
    /* Base Paramter */
    moduleFileSystem.printInfo.printParameter.layerThickness          = 0.05000;   //mm

    /* Bottom Layer */
    moduleFileSystem.printInfo.printParameter.bottomLayerExposureTime =    8000;   //ms
    moduleFileSystem.printInfo.printParameter.bottomLayerNumber       =       7;   //layer
    moduleFileSystem.printInfo.printParameter.bottomLiftFeedRate      =  150.00;   //mm/s

    /* Normal Layer */
    moduleFileSystem.printInfo.printParameter.layerExposureTime       =    3000;   //ms
    moduleFileSystem.printInfo.printParameter.liftTime                =    7200;   //ms
    moduleFileSystem.printInfo.printParameter.liftDistance            =       7;   //mm
    moduleFileSystem.printInfo.printParameter.liftFeedRate            =   150.00;//mm/s

    /** Target **/
    moduleFileSystem.printInfo.printTarget.sourceFilePath             = USB_PATH;
    moduleFileSystem.printInfo.printTarget.tempFilePath               = TEMP_FILE_PATH;
    moduleFileSystem.printInfo.printTarget.tempFileName               = moduleFileSystem.fs.page[pageIndex].item[itemIndex].name;



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

SN_STATUS SN_MODULE_FILE_SYSTEM_MachineUninit(void)
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
    SN_STATUS retValue = SN_STATUS_OK;

    /** MESSAGE Q INIT **/
    SN_SYS_MessageQInit(&msgQIdFileSystem);

    /** MUTEX INIT **/
    if (pthread_mutex_init(&ptmFileSystem, NULL) != 0)
    {
        printf("\n mutex init failed\n"); fflush(stdout);
    }

    /** THREAD INIT **/
    if((retValue = pthread_create(&ptFileSystem, NULL, sFileSystemThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retValue); fflush(stdout);
    }

    /** USB DRIVER INIT **/
    SN_SYS_USBDriverInit(USBEvent_callback);

    /** MACHINE INFO INIT **/
    SN_MODULE_FILE_SYSTEM_MachineInfoInit();

    /** FILE SYSTEM INIT **/
    sFileSystemRead();

    return retValue;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void)
{
    SN_SYS_MessageQRemove(&msgQIdFileSystem);

    return SN_STATUS_OK;
}

static void* sFileSystemThread()
{
    general_evt_t evt;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdFileSystem);

        switch(evt.evt_id)
        {
            case MSG_FILE_SYSTEM_USB_MOUNT:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, 0);
                //printf("File System USB Mount. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;
            case MSG_FILE_SYSTEM_USB_UNMOUNT:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, 0);
                //printf("File System USB Unmount. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_READ:
                sFileSystemRead();
                sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, APP_EVT_MSG_FILE_SYSTEM_READ_DONE);
                //printf("File System Read. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_UPDATE:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, 0);
                SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, APP_EVT_MSG_FILE_SYSTEM_UPDATE_DONE);
                //printf("File System Update. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_WAITING:
                //printf("File System Waiting. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            default:
                break;
        }
    }

    return 0;
}

static void* USBEvent_callback(int evt)
{
    switch(evt)
    {
        case USB_EVT_MOUNT:
            sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_MOUNT, 0);
            break;
        case USB_EVT_UNMOUNT:
            sFileSystemMessagePut(MSG_FILE_SYSTEM_USB_UNMOUNT, 0);
            break;
        case USB_EVT_WAITING:
            sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, 0);
            break;
        default:
            break;
    }
    return 0;
}

static void sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage)
{
    SN_SYS_MessagePut(&msgQIdFileSystem, evtId, evtMessage);
}

//@DEBUG
static void safe_create_dir(const char *dir)
{
    if (mkdir(dir, 0755) < 0) {
        if (errno != EEXIST) {
            perror(dir);
            exit(1);
        }
    }
}

static SN_STATUS sReadGCodeFile(const char* srcPath)
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

static SN_STATUS sFileSystemRead(void)
{
    DIR *dp;
    struct dirent *ep;
    char* nameBuffer = NULL;
    int pageIndex = 0;
    int itemIndex = 0;

    dp = opendir(USB_PATH);

    /* Init FS */
    for(pageIndex = 0; pageIndex <= moduleFileSystem.fs.pageCnt; pageIndex++)
    {
        moduleFileSystem.fs.page[pageIndex].itemCnt = 0;
    }
    moduleFileSystem.fs.pageCnt = 0;

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

    /* @DEBUG */
    sFileSystemPrint();

    return SN_STATUS_OK;
}

/* @DEBUG */
static void sFileSystemPrint(void)
{
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
}

/** Util Functions **/
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
        printf("COUNT SLICE --FAILD...\n"); fflush(stdout);
    }

    if(!r)
    {
        //r = rmdir(filePath);
        printf("SLICE : %04d\n", slice); fflush(stdout);
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
        printf("CAN'T ONEN SOURCE FILE.\n");
    }

    if(des ==NULL)
    {
        printf("CAN'T OPEN DES FILE.\n");
    }

    while(true)
    {
        readCnt = fread((void*)buf, 1, sizeof(buf), src);

        if(readCnt < sizeof(buf))
        {
            if(feof(src)!=0)
            {
                fwrite((void*)buf, 1, readCnt, des);
                printf("MAKE TEMP FILE.\n"); fflush(stdout);
                break;
            }
            else
            {
                printf("MAKE TEMP FILE. --FAILD...\n");
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
        printf("REMOVE TEMP FILE.\n"); fflush(stdout);
        closedir(d);
    }
    else
    {
        printf("REMOVE TEMP FILE. --FAILD...\n"); fflush(stdout);
    }

    if(!r)
    {

    }

    return SN_STATUS_OK;
}

SN_STATUS sExtractTempFile(const char* srcPath, const char* desPath)
{

    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;

    char buf[1000];

    int err;
    int i, len;
    int fd;
    long long sum;
    char* fullFilePath;

    if ((za = zip_open(srcPath, 0, &err)) == NULL)
    {
            zip_error_to_str(buf, sizeof(buf), err, errno);
            fprintf(stderr, "can't open zip archive `%s': %s/n",srcPath, buf); fflush(stdout);
            return 1;
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
                    fprintf(stderr, "boese, boese/n");
                    exit(100);
                }

                len = strlen(desPath) + strlen(sb.name);

                fullFilePath = malloc((len + 2)* sizeof(char));

                snprintf(fullFilePath, len + 2, "%s/%s", desPath, sb.name);

                fd = open(fullFilePath, O_RDWR | O_TRUNC | O_CREAT, 0644);

                if (fullFilePath != NULL)
                {
                    free(fullFilePath);
                }

                if (fd < 0) {
                    fprintf(stderr, "boese, boese/n");
                    exit(101);
                }

                sum = 0;

                while (sum != sb.size)
                {
                    len = zip_fread(zf, buf, 1000);
                    if (len < 0)
                    {
                      fprintf(stderr, "boese, boese/n");
                      exit(102);
                    }
                    write(fd, buf, len);
                    sum += len;
                }

                close(fd);
                zip_fclose(zf);
            }
        }
        else
        {
              //printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1)
    {
        fprintf(stderr, "can't close zip archive `%s'/n", srcPath);
        return 1;
    }

    printf("EXTRACT TEMP FILE.\n"); fflush(stdout);

    return SN_STATUS_OK;
}
