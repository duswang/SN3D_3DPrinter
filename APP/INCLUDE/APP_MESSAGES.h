/**
 * @file APP_MESSAGES.h
 * @author Bato
 * @date 2 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup APP_MSG Application Message
 * @ingroup APP
 * @brief Application Message Type and messages
 * @{
 */

#ifndef INCLUDE_SN_SYSTEM_MESSAGES_H_
#define INCLUDE_SN_SYSTEM_MESSAGES_H_

/** @brief
 *
 *  @param evtId - Module type
 *  @param evtMessage - Module event or message
 *
 *  @return
 */
extern SN_STATUS SN_SYSTEM_SendAppMessage(event_id_t evtId, event_msg_t evtMessage);
/**@}*/

/** @defgroup APP_MSG_TYPE Message Type
 *  @ingroup APP_MSG
 *  @{
 *  */
typedef enum {
    APP_EVT_ID_3D_PRINTER     = 0x0001,     /**< 0x0001*/
    APP_EVT_ID_DISPLAY        = 0x0002,     /**< 0x0002*/
    APP_EVT_ID_FILE_SYSTEM    = 0x0003,     /**< 0x0003*/
    APP_EVT_ID_IMAGE_VIEWER   = 0x0004,     /**< 0x0004*/
    APP_EVT_ID_IGNORE         = 0xFF01      /**< 0xFF01*/
} evtAppType_t;
/**@}*/

/** @defgroup APP_MSG_MODULE_3D_PRINTER 3D Printer Message
 *  @ingroup APP_MSG_TYPE
 *  @{
 * */
typedef enum {
    APP_EVT_MSG_3D_PRINTER_HOMING_DONE           = 0x0001,  /**< 0x0001*/
    APP_EVT_MSG_3D_PRINTER_Z_MOVE_DONE           = 0x0002,  /**< 0x0002*/
    APP_EVT_MSG_3D_PRINTER_PAUSE                 = 0x0003,  /**< 0x0003*/
    APP_EVT_MSG_3D_PRINTER_STOP                  = 0x0004,  /**< 0x0004*/
    APP_EVT_MSG_3D_PRINTER_FINISH                = 0x0005,  /**< 0x0005*/
    APP_EVT_MSG_3D_PRINTER_RAMPS_BOARD_INIT_DONE = 0x0006,  /**< 0x0006*/
    APP_EVT_MSG_3D_PRINTER_NONE                  = 0xFF01   /**< 0xFF01*/
} evtAppMsg_3DPrinter_t;
/**@}*/

/** @defgroup APP_MSG_MODULE_FILE_SYSTEM File System Message
 *  @ingroup APP_MSG_TYPE
 *  @{
 */
typedef enum {
    APP_EVT_MSG_FILE_SYSTEM_USB_MOUNT            = 0x0001,  /**< 0x0001*/
    APP_EVT_MSG_FILE_SYSTEM_USB_UNMOUNT          = 0x0002,  /**< 0x0002*/
    APP_EVT_MSG_FILE_SYSTEM_READ_DONE            = 0x0003,  /**< 0x0003*/
    APP_EVT_MSG_FILE_SYSTEM_UPDATE               = 0x0004,  /**< 0x0004*/
    APP_EVT_FILE_SYSTEM_NONE                     = 0xFF01   /**< 0xFF01*/
} evtAppMsg_FileSystem_t;
/**@}*/

/** @defgroup APP_MSG_MODULE_DISPLAY Display Message
 *  @ingroup APP_MSG_TYPE
 *  @{
 */
typedef enum {
    APP_EVT_DISPLAY_NONE                         = 0xFF01   /**< 0xFF01*/
} evtAppMsg_Display_t;
/**@}*/

/** @defgroup APP_MSG_MODULE_IMAGE_VIEWER Image Viewer Message
 *  @ingroup APP_MSG_TYPE
 *  @{
 */
typedef enum {
    APP_EVT_IMAGE_VIEWER_NONE                    = 0xFF01   /**< 0xFF01*/
} evtAppMsg_ImageViewr_t;
/**@}*/

#endif /* INCLUDE_SN_SYSTEM_MESSAGES_H_ */