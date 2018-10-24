/**
@mainpage SN3D Main Page
  @section INTRO
  -
  @section   PROJECT INFO
  - Name  :  SN3D 3D Printer.
  - brief :
  @section  INOUTPUT
  - INPUT       :
  - OUTPUT      :
  @section  CREATEINFO
  -
  @section  MODIFY INFO
  -
  -
 */
/**
 * @file SN_API.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup API API
 * @brief SN_API 0.0.1v
 * @{
 */

#ifndef SN_API
#define SN_API

/* Standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* Operation System */
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

/* Operation System */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/time.h>

#include <errno.h>

/* File System */
#include <dirent.h>

/* Serial */
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/* Library */
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

/** @def bool */
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif


/* * * * * * * * * * * System * * * * * * * * * * * */
#include "SN_SYSTEM.h"

/* * * * * * * * * * * Modules * * * * * * * * * * * */
#include "SN_MODULE.h"

/* * * * * * * * * * * Applications * * * * * * * * * * * */
#include "APP_MESSAGES.h"

#endif /* SN_API */
/**@}*/
