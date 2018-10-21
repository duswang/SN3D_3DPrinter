/*
 * APP_MESSAGES.h
 *
 *  Created on: 2018. 10. 2.
 *      Author: BartKim
 */

#ifndef INCLUDE_SN_SYSTEM_MESSAGES_H_
#define INCLUDE_SN_SYSTEM_MESSAGES_H_

#define SN_MSG_TYPE_BASE 0

#define SN_APP_MSG_TYPE_3D_PRINTER              (SN_MSG_TYPE_BASE + 0)
#define SN_APP_MSG_TYPE_DISPLAY                 (SN_MSG_TYPE_BASE + 1)
#define SN_APP_MSG_TYPE_FILE_SYSTEM             (SN_MSG_TYPE_BASE + 2)
#define SN_APP_MSG_TYPE_IMAGE_VIEWER            (SN_MSG_TYPE_BASE + 3)

/* * * * * * * * * *  * * MESSAGE STRUCTUE * * * * * * * * * * * *

    Message Strucutre

    long     mtype - can't edit this-!
    uint32_t
    event_id            evt_id;
    uint32_t            value;

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/** EVENT MESSAGES DEFINE **/
#define APP_EVENT_MESSAGE_TYPE_MASK                     0xFFFF0000
#define APP_EVENT_MESSAGE_VALUE_MASK                    0x0000FFFF

#define APP_EVENT_MESSAGE(TYPE, VALUE)  \
    (((TYPE<<16) & APP_EVENT_MESSAGE_TYPE_MASK) | ((VALUE) & APP_EVENT_MESSAGE_VALUE_MASK))

#define APP_EVENT_MESSAGE_TYPE(EVENTCODE)   \
    (((EVENTCODE) & APP_EVENT_MESSAGE_TYPE_MASK) >> 16)

#define APP_EVENT_MESSAGE_VALUE(EVENTCODE)  \
    (((EVENTCODE) & APP_EVENT_MESSAGE_VALUE_MASK))

typedef enum {
    APP_EVT_ID_3D_PRINTER     = 0x0001,
    APP_EVT_ID_DISPLAY        = 0x0002,
    APP_EVT_ID_FILE_SYSTEM    = 0x0003,
    APP_EVT_ID_IMAGE_VIEWER   = 0x0004,
    APP_EVT_ID_IGNORE         = 0xFF01
} evtAppType_t;

/** Module App Message **/
typedef enum {
    APP_EVT_MSG_3D_PRINTER_HOMING_DONE           = 0x0001,
    APP_EVT_MSG_3D_PRINTER_Z_MOVE_DONE           = 0x0002,
    APP_EVT_MSG_3D_PRINTER_PAUSE                 = 0x0003,
    APP_EVT_MSG_3D_PRINTER_STOP                  = 0x0004,
    APP_EVT_MSG_3D_PRINTER_FINISH                = 0x0005,
    APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE = 0x0006,
    APP_EVT_MSG_3D_PRINTER_NONE                  = 0xFF01
} evtAppMsg_3DPrinter_t;

typedef enum {
    APP_EVT_MSG_FILE_SYSTEM_USB_MOUNT            = 0x0001,
    APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT          = 0x0002,
    APP_EVT_MSG_FILE_SYSTEM_READ_DONE            = 0x0003,
    APP_EVT_MSG_FILE_SYSTEM_UPDATE               = 0x0004,
    APP_EVT_FILE_SYSTEM_NONE                     = 0xFF01
} evtAppMsg_FileSystem_t;

typedef enum {
    APP_EVT_DISPLAY_NONE        = 0xFF01
} evtAppMsg_Display_t;

typedef enum {
    APP_EVT_IMAGE_VIEWER_NONE        = 0xFF01
} evtAppMsg_ImageViewr_t;

extern SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage);

#endif /* INCLUDE_SN_SYSTEM_MESSAGES_H_ */
