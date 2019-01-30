/*
 * SN_PATH.h
 *
 *  Created on: 2019. 1. 30.
 *      Author: BartKim
 */

#ifndef SN_PATH_H_
#define SN_PATH_H_

#ifdef __APPLE__
#define USB_PATH                      "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_PATH                      "/mnt/volume"
#endif

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256

#define FIRMWARE_PATH                    "/SN3D/sn3d-project/sn3d-bootloader/res/firmware"
#define TEMP_FIRMWARE_PATH               "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware"

#define FIRMWARE_VERSION_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/version.xml"
#define TEMP_FIRMWARE_VERSION_PATH       "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/version.xml"

#define FIRMWARE_MACHINE_INFO_PATH       "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/machineInfo.xml"
#define TEMP_FIRMWARE_MACHINE_INFO_PATH  "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/machineInfo.xml"

#define FIRMWARE_BINARY_PATH             "/SN3D/sn3d-project/sn3d-bootloader/res/firmware/exe"
#define TEMP_FIRMWARE_BINARY_PATH        "/SN3D/sn3d-project/sn3d-bootloader/res/tempFirmware/exe"

#define SN3D_BINARY_FOLDER_PATH          "/SN3D/sn3d-project/bin/build"
#define SN3D_BINARY_PATH                 "/SN3D/sn3d-project/bin/build/exe"

#define SN3D_OPTION_FOLDER_PATH          "/SN3D/sn3d-project/res/optionConfig"

#define SN3D_MACHINE_CONFIG_FOLDER_PATH  "/SN3D/sn3d-project/res/machineConfig"
#define SN3D_MACHINE_CONFIG_PATH         "/SN3D/sn3d-project/res/machineConfig/machineInfo.xml"

#define MACHINE_5_5_INCH_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/5_5.txt"
#define MACHINE_8_9_INCH_PATH            "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/8_9.txt"
#define MACHINE_15_6_INCH_PATH           "/SN3D/sn3d-project/sn3d-bootloader/res/bootConfig/4k.txt"
#define MACHINE_23_8_INCH_PATH           MACHINE_15_6_INCH_PATH
#define MACHINE_DEFAULT_INCH_PATH        MACHINE_5_5_INCH_PATH
#define MACHINE_BOOTCONFIG_PATH          "/boot/config.txt"

#define SN3D_FW_STR         "sn3d_fw"
#define SN3D_FW_EXTENTION   "zip"

#define SN3D_OPTION_STR         "sn3d_option"
#define SN3D_OPTION_EXTENTION   "zip"

#define HIDDEN_FILE_STR         "._"

#define SN3D_5_5_INCH                "5.5"
#define SN3D_8_9_INCH                "8.9"
#define SN3D_15_6_INCH               "15.6"
#define SN3D_23_8_INCH               "23.8"
#define SN3D_DEFAULT_INCH            SN3D_5_5_INCH

#endif /* PATH_H_ */
