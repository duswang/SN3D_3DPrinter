/*
 * SN_API.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SN_API
#define SN_API

/** Standard **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/** Operation System */
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

/* system */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/time.h>

#include <errno.h>

/** File System **/
#include <dirent.h>

/** Serial **/
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

/**** Library ****/
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <libusb-1.0/libusb.h>
#endif

#ifdef linux
#include <SDL.h>
#include <SDL_image.h>
#include <libusb-1.0/libusb.h>
#endif

#include <zip.h>

/**** Local Library ****/
#include "SN_ERROR.h"

#include "SN_SYSTEM.h"
#include "SN_SYSTEM_MESSAGES.h"

/*** bool ***/
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

/* * * * * * * * * * * Global Structure * * * * * * * * * */
#define MAX_PAGE_SIZE   10
#define MAX_ITEM_SIZE   5
#define MAX_OPTION_SIZE 5

#define FILENAME_SIZE        256

typedef struct file_system_option {
    char name[FILENAME_SIZE];
} fsOption_t;

typedef struct file_system_item {
    char name[FILENAME_SIZE];
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

typedef struct print_prameter {
    float    layerThickness;            //0.05000 mm

    /** Bottom Layer Parameter **/
    long     bottomLayerExposureTime;   //5000 ms
    long     bottomLayerNumber;         //7 Layer
    float    bottomLiftFeedRate;        //150.00 mm/s

    /** Normal Layer Paramter **/
    long     layerExposureTime;         //8000 ms
    float    liftFeedRate;              //150.00 mm/s
    long     liftTime;                  //7200 ms - Need Auto Calculate
    long     liftDistance;              //7 mm

    float    retractFeedRate;           //150.00 mm/s

    /** Reserve **/
} printParm_t;

typedef struct print_target {
    char* sourceFilePath;
    char* tempFilePath;
    char* tempFileName;
    uint32_t slice;
} printTarget_t;

typedef struct machine_prameter {
    uint32_t weight;             //MACHINE Weight
    uint32_t height;             //MACHINE Height
} machineParm_t;


typedef struct machine_info {
    char* machineName;      //PARARIS 500
} machineTarget_t;

typedef struct print_information {
    printParm_t   printParameter;
    printTarget_t    printTarget;
    bool                  isInit;
} printInfo_t;

typedef struct machine {
    machineParm_t deviceParameter;
    machineTarget_t  deviceTarget;
    bool                   isInit;
} machineInfo_t;

/* * * * * * * * * * * Functions * * * * * * * * * * * */
/**** Application ****/
extern SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage);

/**** DISPLAY MODULE ****/
extern SN_STATUS SN_MODULE_DISPLAY_Init(void);
extern SN_STATUS SN_MODULE_DISPLAY_Uninit(void);

extern SN_STATUS SN_MODULE_DISPLAY_EnterState(nx_page_t state);

extern SN_STATUS SN_MODULE_DISPLAY_PrintingUpdate(void);
extern SN_STATUS SN_MODULE_DISPLAY_FileSelectUpdate(uint32_t page);

/**** 3D PRINTER MODULE ****/
extern SN_STATUS SN_MODULE_3D_PRINTER_Init(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Uninit(void);

/*** Printing Functions ***/
extern SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODULE_3D_PRINTER_Stop(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Pause(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Resume(void);

/*** Control Functions ***/
extern SN_STATUS SN_MODULE_3D_PRINTER_MOTOR_INIT(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_HOMING(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_UP(float mm);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_DOWN(float mm);

/**** FILE SYSTEM MODULE ****/
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Init(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Uninit(void);

extern SN_STATUS SN_MODULE_FILE_SYSTEM_Get(fs_t* pFs);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_Update(void);

extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoUninit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoUninit(void);

extern machineInfo_t SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);
extern printInfo_t   SN_MODULE_FILE_SYSTEM_PrintInfoGet(void);

/**** IMAGE VIEWR MODULE ****/
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void);
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void);
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex);
extern SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void);



#endif /* SN_API */
