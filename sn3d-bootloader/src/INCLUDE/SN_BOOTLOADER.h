/*
 * SN_BOOTLOADER_MAIN.h
 *
 *  Created on: 2018. 12. 7.
 *      Author: BartKim
 */

#ifndef SN_BOOTLOADER_MAIN_H_
#define SN_BOOTLOADER_MAIN_H_

/* Standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* System */
#include <sys/stat.h>

/* File System */
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <errno.h>


#ifdef __APPLE__
#define USB_PATH                      "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_PATH                      "/mnt/volume"
#endif

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256

#define SN3D_FW_STR         "sn3d_fw"
#define SN3D_FW_EXTENTION   "zip"

#include "SN_SYS_ERROR.h"

#include "FILE_SYSTEM_FCTL.h"
#include "FILE_SYSTEM_XML.h"

#endif /* SN3D_BOOTLOADER_SRC_INCLUDE_SN_BOOTLOADER_MAIN_H_ */
