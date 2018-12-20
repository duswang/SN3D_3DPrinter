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
#include <sys/reboot.h>

/* File System */
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

/* Display */


#include <errno.h>

#ifdef __APPLE__
#define USB_PATH                      "/Volumes/USB_0"
#endif
#ifdef linux
#define USB_PATH                      "/mnt/volume"
#endif

#define MAX_FILENAME_LENGTH        256
#define MAX_PATH_LENGTH            256

#include "SN_SYS_ERROR.h"

#include "FILE_SYSTEM_FCTL.h"
#include "FILE_SYSTEM_XML.h"

#include "SN_MACHINE_CONTROL.h"
#include "SN_VERSION_CONTROL.h"

#endif /* SN3D_BOOTLOADER_SRC_INCLUDE_SN_BOOTLOADER_MAIN_H_ */
