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
#if(APPLE)
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <libusb-1.0/libusb.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#include <libusb-1.0/libusb.h>
#endif
#include <zip.h>

/**** Local Library ****/
#include "SN_SYSTEM.h"
#include "SN_SYSTEM_MESSAGES.h"

/*** bool ***/
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

/* * * * * * * * * Error Codes * * * * * * * * * **/
#define SN_STATUS           uint16_t

#define SN_STATUS_BASE      0

#define SN_STATUS_OK                        (SN_STATUS_BASE + 0)
#define SN_STATUS_TIMEOUT                   (SN_STATUS_BASE + 1)
#define SN_STATUS_INVALID_PARAM             (SN_STATUS_BASE + 2)
#define SN_STATUS_NOT_SUPPORTED             (SN_STATUS_BASE + 3)
#define SN_STATUS_OUT_OF_MEM                (SN_STATUS_BASE + 4)
#define SN_STATUS_NOT_INITIALIZED           (SN_STATUS_BASE + 5)
#define SN_STATUS_ALREADY_INITIALIZED       (SN_STATUS_BASE + 6)
#define SN_STATUS_RESOURCE_NOT_AVAILABLE    (SN_STATUS_BASE + 7)
#define SN_STATUS_NOT_OK                    (SN_STATUS_BASE + 8)       /* Unknown or undefined reason */

/* * * * * * * * * * * Global Struct * * * * * * * * * */
typedef struct print_prameter {
    float    layerThickness;            //0.05000 mm

    /** Bottom Layer Parameter **/
    uint32_t bottomLayerExposureTime;   //5000 ms
    uint32_t bottomLayerNumber;         //7 Layer
    float    bottomLiftFeedRate;        //150.00 mm/s

    /** Normal Layer Paramter **/
    uint32_t layerExposureTime;         //8000 ms
    float    liftFeedRate;              //150.00 mm/s
    uint32_t liftTime;                  //7200 ms - Need Auto Calculate
    uint32_t liftDistance;              //7 mm

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
extern SN_STATUS SN_MODUEL_DISPLAY_Init(void);
extern SN_STATUS SN_MODUEL_DISPLAY_Uninit(void);

extern SN_STATUS SN_MODUEL_DISPLAY_PrintingUpdate(void);
extern SN_STATUS SN_MODUEL_DISPLAY_FileSelectUpdate(void);

/**** 3D PRINTER MODULE ****/
extern SN_STATUS SN_MODUEL_3D_PRINTER_Init(void);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Uninit(void);

/*** Printing Functions ***/
extern SN_STATUS SN_MODUEL_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Stop(void);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Pause(void);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Resume(void);

/*** Control Functions ***/
extern SN_STATUS SN_MODUEL_3D_PRINTER_Z_HOMING(void);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Z_UP(float mm);
extern SN_STATUS SN_MODUEL_3D_PRINTER_Z_DOWN(float mm);

/**** FILE SYSTEM MODULE ****/
extern SN_STATUS SN_MODUEL_FILE_SYSTEM_Init(void);
extern SN_STATUS SN_MODUEL_FILE_SYSTEM_Uninit(void);

extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoInit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoInit(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_MachineInfoUninit(void);
extern SN_STATUS SN_MODULE_FILE_SYSTEM_PrintInfoUninit(void);

extern machineInfo_t SN_MODULE_FILE_SYSTEM_MachineInfoGet(void);
extern printInfo_t   SN_MODULE_FILE_SYSTEM_PrintInfoGet(void);

/**** IMAGE VIEWR MODULE ****/
extern SN_STATUS SN_MODUEL_IMAGE_VIEWER_Init(void);
extern SN_STATUS SN_MODUEL_IMAGE_VIEWER_Destroy(void);
extern SN_STATUS SN_MODUEL_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex);
extern SN_STATUS SN_MODUEL_IMAGE_VIEWER_CLEAR(void);



#endif /* SN_API */
