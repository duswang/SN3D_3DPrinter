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
#include "SN_SYSTEM.h"
#include "APP_MESSAGES.h"

/*** bool ***/
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif

/* * * * * * * * * * * Modules * * * * * * * * * * * */
//#include "SN_MODULE_IMAGE_VIEWER.h"
// 'IMAGE_VIEWER MODULE' INCLUDE IN '3D PRINTER MODULE'
#include "SN_MODULE_3D_PRINTER.h"
#include "SN_MODULE_DISPLAY.h"
#include "SN_MODULE_FILE_SYSTEM.h"

#endif /* SN_API */
