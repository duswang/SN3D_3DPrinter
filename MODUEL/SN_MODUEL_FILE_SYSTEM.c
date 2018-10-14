/*
 * MODUEL_FILE_SYSTEM.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_MODUEL_FILE_SYSTEM.h"

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
static void* USBEvent_callback(int evt);

/* System */
static void        sFileSystemMessagePut(evtFileSystem_t evtId, event_msg_t evtMessage);
static void*       sFileSystemThread();

/* Local */
static SN_STATUS   sFileSystemRead(void);
static void        sFileSystemPrint(void);

static SN_STATUS   sCreateTempFile(uint32_t pageIndex, uint32_t itemIndex);
static SN_STATUS   sRemoveTempFile(void);

/* Util */
static const char* sGetFilenameExt(const char *filename);
static const char* sGetFilename(const char *filename);

SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //sMachineInfoRead();
    //@DEBUG

    moduleFileSystem.machineInfo.isInit = true;
    moduleFileSystem.machineInfo.deviceParameter.weight   =         1600;
    moduleFileSystem.machineInfo.deviceParameter.height   =          900;

    moduleFileSystem.machineInfo.deviceTarget.machineName =  DEVICE_NAME;

    return retStatus;
}

SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    //sPrintInfoRead();
    //@DEBUG
    //sCreateTempFile(pageIndex, itemIndex);


    moduleFileSystem.printInfo.isInit = true;

    /** Parameter **/
    /* Base Paramter */
    moduleFileSystem.printInfo.printParameter.layerThickness          = 0.05000;//mm

    /* Bottom Layer */
    moduleFileSystem.printInfo.printParameter.bottomLayerExposureTime = 5000;    //ms
    moduleFileSystem.printInfo.printParameter.bottomLayerNumber       =    7;    //layer
    moduleFileSystem.printInfo.printParameter.bottomLiftFeedRate      =  150.00; //mm/s

    /* Normal Layer */
    moduleFileSystem.printInfo.printParameter.layerExposureTime       =  8000;   //ms
    moduleFileSystem.printInfo.printParameter.liftTime                =  7200;   //ms
    moduleFileSystem.printInfo.printParameter.liftDistance            =     7;   //mm
    moduleFileSystem.printInfo.printParameter.liftFeedRate            =   150.00;//mm/s

    /** Target **/
    moduleFileSystem.printInfo.printTarget.sourceFilePath             = USB_PATH;
    moduleFileSystem.printInfo.printTarget.tempFilePath               = TEMP_FILE_PATH;
    moduleFileSystem.printInfo.printTarget.tempFileName               = TEMP_FILE_NAME;

    moduleFileSystem.printInfo.printTarget.slice                      = 637;

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
        sRemoveTempFile();
    }

    return retStatus;
}

SN_STATUS SN_MODUEL_FILE_SYSTEM_Init(void)
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
    sFileSystemPrint();



    return retValue;
}

SN_STATUS SN_MODUEL_FILE_SYSTEM_Uninit(void)
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
                printf("File System USB Mount. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;
            case MSG_FILE_SYSTEM_USB_UNMOUNT:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, 0);
                printf("File System USB Unmount. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_READ:
                sFileSystemRead();
                sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, 0);
                printf("File System Read. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_UPDATE:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, 0);
                SN_SYSTEM_SendAppMessage(APP_EVT_ID_FILE_SYSTEM, 0);
                printf("File System Update. :: %d %d\n", evt.evt_id, evt.evt_msg);
                fflush(stdout);
                break;

            case MSG_FILE_SYSTEM_WAITING:
                printf("File System Waiting. :: %d %d\n", evt.evt_id, evt.evt_msg);
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

SN_STATUS sCreateTempFile(uint32_t pageIndex, uint32_t itemIndex)
{

    struct zip *za;
    struct zip_file *zf;
    struct zip_stat sb;

    char* fromPath;
    char* toPath;
    char buf[100];

    int err;
    int i, len;
    int fd;
    long long sum;

    sprintf(fromPath,"%s%s.%s/",moduleFileSystem.printInfo.printTarget.sourceFilePath, moduleFileSystem.fs.page[pageIndex].item[itemIndex].name, FILENAME_EXT);
    //sprintf(toPath  ,"%s%s.%s",PRINT_FILE_PATH, PRINT_FILE_NAME, FILENAME_EXT);

    puts(fromPath); fflush(stdout);

    if ((za = zip_open(fromPath, 0, &err)) == NULL)
    {
            zip_error_to_str(buf, sizeof(buf), err, errno);
            fprintf(stderr, "can't open zip archive `%s': %s/n",fromPath, buf);
            return 1;
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++)
    {
            if (zip_stat_index(za, i, 0, &sb) == 0) {
                printf("==================/n");
                len = strlen(sb.name); fflush(stdout);
                printf("Name: [%s], ", sb.name);
                printf("Size: [%llu], ", sb.size);
                printf("mtime: [%u]/n", (unsigned int)sb.mtime);
                if (sb.name[len - 1] == '/') {
                    safe_create_dir(sb.name);
                } else {
                    zf = zip_fopen_index(za, i, 0);
                    if (!zf) {
                        fprintf(stderr, "boese, boese/n");
                        exit(100);
                    }

                    fd = open(sb.name, O_RDWR | O_TRUNC | O_CREAT, 0644);
                    if (fd < 0) {
                        fprintf(stderr, "boese, boese/n");
                        exit(101);
                    }

                    sum = 0;
                    while (sum != sb.size) {
                        len = zip_fread(zf, buf, 100);
                        if (len < 0) {
                            fprintf(stderr, "boese, boese/n");
                            exit(102);
                        }
                        write(fd, buf, len);
                        sum += len;
                    }
                    close(fd);
                    zip_fclose(zf);
                }
            } else {
                printf("File[%s] Line[%d]/n", __FILE__, __LINE__);
            }
        }

    if (zip_close(za) == -1) {
        fprintf(stderr, "can't close zip archive `%s'/n", toPath);
        return 1;
    }

    return 0;
}

static SN_STATUS sRemoveTempFile(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

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

    if (dp != NULL)
    {
        while((ep = readdir (dp)))
        {
            nameBuffer = ep->d_name;

            if(strcmp(FILENAME_EXT, sGetFilenameExt(nameBuffer)) == 0)
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
                moduleFileSystem.fs.page[pageIndex].itemCnt = itemIndex;
                strcpy(moduleFileSystem.fs.page[pageIndex].item[itemIndex++].name, sGetFilename(nameBuffer));
            }
        }
        (void) closedir (dp);
    }
    else
    {
        perror ("Couldn't open the directory");
    }

    return SN_STATUS_OK;
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

/* @DEBUG */
static void sFileSystemPrint(void)
{
    int pageIndex = 0;
    int itemIndex = 0;

    for(pageIndex = 0; pageIndex <= moduleFileSystem.fs.pageCnt; pageIndex++)
    {
        for(itemIndex = 0; itemIndex <= moduleFileSystem.fs.page[pageIndex].itemCnt; itemIndex++)
        {
            printf("%d: %d :: %s\n", pageIndex, itemIndex, moduleFileSystem.fs.page[pageIndex].item[itemIndex].name); fflush(stdout);
        }

    }
}
