/*
 * SN_MODUEL_FILE_SYSTEM.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_
#define MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_

/** Static Define **/
//@TODO: TO LIST DATA STRUCTURE
#define MAX_PAGE_SIZE   6
#define MAX_ITEM_SIZE   6
#define MAX_OPTION_SIZE 6

#define FILENAME_SIZE 256
#define FILENAME_EXT  "cws"

#define CONFIG_FILENAME_EXT "gcode"

#ifdef __APPLE__
#define USB_PATH              "/Volumes/USB_0/"
#endif

#ifdef linux
#define USB_PATH             "/media/pi/USB_0/"
#endif

#define EXTRACTED_TEMP_FILE_PATH    "../build/"
#define TEMP_FILE_PATH           "../tempFile/"

#define DEVICE_NAME               "POLARIS 500"

/* Module Message */
typedef enum {
    MSG_FILE_SYSTEM_USB_MOUNT = 0,
    MSG_FILE_SYSTEM_USB_UNMOUNT,
    MSG_FILE_SYSTEM_READ,
    MSG_FILE_SYSTEM_UPDATE,
    MSG_FILE_SYSTEM_WAITING,
    MSG_FILE_SYSTEM_RESERVE
} evtFileSystem_t;


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
} fs_t;

typedef struct moduel_file_system {
    fs_t                     fs;
    printInfo_t       printInfo;
    machineInfo_t   machineInfo;
} moduleFileSystem_t;

#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
