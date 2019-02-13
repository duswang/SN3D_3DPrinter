/**
 * @file SN_BOOTLOADER_MAIN.h
 * @author Bato
 * @date 18 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo Hashing the binary file and file checking solution with hash.
 * @todo auto partition allocate on raspberry pi file system.
 *
 * @defgroup SN_BOOTLOADER Bootloader
 * @brief Bootloader Version Beta 0.0.5v
 * @{
 */
#ifndef SN_BOOTLOADER_MAIN_H
#define SN_BOOTLOADER_MAIN_H

/* Standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* System */
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/reboot.h>

#include <openssl/md5.h>

/* File System */
#include <dirent.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <errno.h>

/**
 * @defgroup BOOTLOADER_SYSTEM System
 * @ingroup SN_BOOTLOADER
 * @brief Bootloader System
 * @{
 */
#include "SN_SYS_ERROR.h"

#include "SN_PATH.h"
/**@}*/

/**
 * @defgroup BOOTLOADER_FILE_SYSTEM File System
 * @ingroup SN_BOOTLOADER
 * @brief Bootloader File System
 * @{
 */
#include "FILE_SYSTEM_FCTL.h"
#include "FILE_SYSTEM_XML.h"
/**@}*/

/**
 * @defgroup BOOTLOADER_MODULE Module
 * @ingroup SN_BOOTLOADER
 * @brief Bootloader Module
 * @{
 */
#include "SN_MACHINE_CONTROL.h"
#include "SN_VERSION_CONTROL.h"
/**@}*/

#endif /* SN_BOOTLOADER_MAIN_H */
/**@}*/
