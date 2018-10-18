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
#define FILENAME_EXT         "cws"
#define IMAGE_FILENAME_EXT   "png"
#define CONFIG_FILENAME_EXT  "gcode"

#ifdef __APPLE__
#define USB_PATH              "/Volumes/USB_0/"
#endif

#ifdef linux
#define USB_PATH              "/mnt/volume/"
#endif

#ifdef __APPLE__

#endif

#ifdef linux
#define EXTRACTED_TEMP_FILE_PATH    "/SN3D/sn3d-project/build/"
#define TEMP_FILE_PATH              "/SN3D/sn3d-project/tempFile/"
#endif


#define DEVICE_NAME                 "POLARIS 500"

/* Module Message */
typedef enum {
    MSG_FILE_SYSTEM_USB_MOUNT = 0,
    MSG_FILE_SYSTEM_USB_UNMOUNT,
    MSG_FILE_SYSTEM_READ,
    MSG_FILE_SYSTEM_UPDATE,
    MSG_FILE_SYSTEM_WAITING,
    MSG_FILE_SYSTEM_RESERVE
} evtFileSystem_t;

typedef struct moduel_file_system {
    fs_t                     fs;
    printInfo_t       printInfo;
    machineInfo_t   machineInfo;
} moduleFileSystem_t;

#endif /* MODUEL_INCLUDE_SN_MODUEL_FILE_SYSTEM_H_ */
