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

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <errno.h>


/** Serial **/
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
    /** Commands */
    #include "GCode_Commands.h"
    #include "Nextion_Commands.h"

/** File System**/
#include <libusb-1.0/libusb.h>





#include "SN_SYSTEM.h"

/** Error Codes **/
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


#define SN_MSG_TYPE_BASE 0

#define SN_APP_MSG_TYPE_3D_PRINTER              (SN_MSG_TYPE_BASE + 0)
#define SN_APP_MSG_TYPE_DISPLAY                 (SN_MSG_TYPE_BASE + 1)
#define SN_APP_MSG_TYPE_FILE_SYSTEM             (SN_MSG_TYPE_BASE + 2)
#define SN_APP_MSG_TYPE_IMAGE_VIEWER            (SN_MSG_TYPE_BASE + 3)


#define APP_EVENT_TYPE_MASK                    0xFFFF0000
#define APP_EVENT_VALUE_MASK                   0x0000FFFF

#define APP_EVENT(TYPE, VALUE)  \
    (((TYPE<<16) & APP_EVENT_TYPE_MASK) | ((VALUE) & APP_EVENT_VALUE_MASK))

#define APP_EVENT_TYPE(EVENTCODE)   \
    (((EVENTCODE) & APP_EVENT_TYPE_MASK) >> 16)

#define APP_EVENT_VALUE(EVENTCODE)  \
    (((EVENTCODE) & APP_EVENT_VALUE_MASK))

#define APP_EVT_TYPE_3D_PRINTER                 0x0001
#define APP_EVT_TYPE_DISPLAY                    0x0002
#define APP_EVT_TYPE_FILE_SYSTE                 0x0003
#define APP_EVT_TYPE_IMAGE_VIEWER               0x0004

#define MODUEL_EVT_TYPE_3D_PRINTER              0x00F1
#define MODUEL_EVT_TYPE_DISPLAY                 0x00F2
#define MODUEL_EVT_TYPE_FILE_SYSTEM             0x00F3
#define MODUEL_EVT_TYPE_IMAGE_VIEWER            0x00F4








extern void SN_MODUEL_DISPLAY_Init(void);
extern void SN_MODUEL_DISPLAY_Uninit(void);

extern void SN_MODUEL_3D_PRINTER_Init(void);
extern void SN_MODUEL_3D_PRINTER_Uninit(void);

extern int SN_MODUEL_FILE_SYSTEM_Init(void);
extern void SN_MODUEL_FILE_SYSTEM_Uninit(void);

//@DEBUG
extern void SN_MODUEL_3D_PRINTER_FOR_DEMO(void);

#endif /* SN_API */
