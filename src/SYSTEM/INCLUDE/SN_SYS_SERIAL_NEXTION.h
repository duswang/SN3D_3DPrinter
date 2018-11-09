/**
 * @fileSN_SYS_SERIAL_NEXTION.h
 * @author Bato
 * @date 27 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_SERIAL_NEXTION Nextion Display Command
 * @ingroup SYSTEM_SERIAL_COMM
 * @brief Nextion Display command
 * @{
 */

#ifndef SN_SYS_SERIAL_NEXTION_H_
#define SN_SYS_SERIAL_NEXTION_H_

typedef union Nextion_message {

   struct {
           uint8_t page;
           uint8_t type;
           uint8_t id;
           uint8_t value;
           uint8_t command;
           uint8_t endcode[3];
   };
   uint8_t  NXmessage8bit[8];
   uint32_t NXmessage[2];
} msgNX_t;

/* * Nextion Display UART Message * */
/* * * * * * * * * * * * * * * * * * * * * * * *

    Nextion Display TX Command.

    [page] [TYPE] [ID] [value]       32bit

    [reserve] [endCode[][][]]        32bit

 * * * * * * * * * * * * * * * * * * * * * * * */
/**
 *
 */
#define NX_PAGE_BOOT_COMMAND        "page Boot"
#define NX_PAGE_WAITING_COMMAND     "page Waiting"
#define NX_PAGE_CONTROL_COMMAND     "page Control"
#define NX_PAGE_FILE_SYSTEM_COMMAND "page FileSelect"
#define NX_PAGE_PRINTING_COMMAND    "page Print"
#define NX_PAGE_LOADING_COMMAND     "page Loading"

#define NX_COMMAND_RESET            "rest"

/* * * * * * * * * * * * * * * * * * * * * * * *

    Nextion Display RX EVENT.

    [Command]                        8bit

    [page] [TYPE] [ID] [value]       32bit

    [endCode[][][]]                  24bit

 * * * * * * * * * * * * * * * * * * * * * * * */
/** RX DATA HEAD COMMAND **/
typedef enum
{
    /* RESPONSE */
    NX_COMMAND_FINISHED                 = 0x01,//!< NX_COMMAND_FINISHED
    /* EVT */
    NX_COMMAND_EVT_LAUNCHED             = 0x88,//!< NX_COMMAND_EVT_LAUNCHED
    NX_COMMAND_EVT_UPGRADED             = 0x89,//!< NX_COMMAND_EVT_UPGRADED
    NX_COMMAND_EVT_TOUCH_HEAD           = 0x65,//!< NX_COMMAND_EVT_TOUCH_HEAD
    NX_COMMAND_EVT_POSITIOn_HEAD        = 0x67,//!< NX_COMMAND_EVT_POSITIOn_HEAD
    NX_COMMAND_EVT_SLEEP_POSITION_HEAD  = 0x68,//!< NX_COMMAND_EVT_SLEEP_POSITION_HEAD

    /* SN MESSAGE HEAD */
    NX_COMMAND_SN_MESSAGE_HEAD          = 0xF0,//!< NX_COMMAND_SN_MESSAGE_HEAD

    /* REQUEST VALUE HEAD */
    NX_COMMAND_CURRENT_PAGE_ID_HEAD     = 0x66,//!< NX_COMMAND_CURRENT_PAGE_ID_HEAD
    NX_COMMAND_STRING_HEAD              = 0x70,//!< NX_COMMAND_STRING_HEAD
    NX_COMMAND_NUMBER_HEAD              = 0x71,//!< NX_COMMAND_NUMBER_HEAD
    /* ERROR */
    NX_COMMAND_INVALID_CMD              = 0x00,//!< NX_COMMAND_INVALID_CMD
    NX_COMMAND_INVALID_COMPONET_ID      = 0x02,//!< NX_COMMAND_INVALID_COMPONET_ID
    NX_COMMAND_INVALID_PAGE_ID          = 0x03,//!< NX_COMMAND_INVALID_PAGE_ID
    NX_COMMAND_INVALID_PICTURE_ID       = 0x04,//!< NX_COMMAND_INVALID_PICTURE_ID
    NX_COMMAND_INVALID_FONT_ID          = 0x05,//!< NX_COMMAND_INVALID_FONT_ID
    NX_COMMAND_INVALID_BAUD             = 0x11,//!< NX_COMMAND_INVALID_BAUD
    NX_COMMAND_INVALID_VARIABLE         = 0x1A,//!< NX_COMMAND_INVALID_VARIABLE
    NX_COMMAND_INVALID_OPERATION        = 0x1B //!< NX_COMMAND_INVALID_OPERATION
}nx_command_t;

/** PAGE **/
typedef enum
{
    NX_PAGE_WAITING             = 0x01,
    NX_PAGE_CONTROl             = 0x02,
    NX_PAGE_FILE_SELECT         = 0x03,
    NX_PAGE_PRINTING            = 0x04,
    NX_PAGE_PAUSE               = 0x05,
    NX_PAGE_SETUP               = 0x06,
    NX_PAGE_INIT                = 0x07,
    NX_PAGE_LOADING             = 0xF1, //ONLY NEXTION DISPLAY. FOR LOADING or WAITING MOTOR.
    NX_PAGE_NONE
} nx_page_t; /* It Need to sync ref : 'app_state_t' */

/** TYPE **/
typedef enum
{
    NX_TYPE_BUTTON              = 0x01,
    NX_TYPE_TOGGLE              = 0x02,
    NX_TYPE_SCREEN_TOUCH        = 0x03,
    NX_TYPE_NOTIFY              = 0x04,
    NX_TYPE_NONE
} nx_type_t;

/** ID  **/
typedef enum
{
    NX_ID_BUTTON_HOME           = 0x1F,
    NX_ID_BUTTON_PRINT          = 0x2F,
    NX_ID_BUTTON_YES            = 0x3F,
    NX_ID_BUTTON_NO             = 0x4F,
    NX_ID_NONE
} nx_id_t;

typedef enum
{
    NX_VALUE_NOT_USED           = 0x00
} nx_value_t;

/*** ID :: PAGE ***/

typedef enum
{
    NX_ID_WAITING_BUTTON_PRINT   = 0x01,
    NX_ID_WAITING_BUTTON_CONTROL,
} nx_waiting_id_t;

typedef enum
{
    NX_ID_FILE_SELECT_BUTTON_OPTION_UP          = 0x01,
    NX_ID_FILE_SELECT_BUTTON_OPTION_DOWN        = 0x02,
    NX_ID_FILE_SELECT_BUTTON_PAGE_DOWN          = 0x03,
    NX_ID_FILE_SELECT_BUTTON_PAGE_UP            = 0x04,
    NX_ID_FILE_SELECT_BUTTON_FILE_SELECT        = 0x05,
    NX_ID_FILE_SELECT_BUTTON_PRINT_START        = 0x06
} nx_file_select_id_t;

typedef enum
{
    NX_ID_PRINTING_BUTTON_PAUSE   = 0x01,
    NX_ID_PRINTING_BUTTON_STOP,
} nx_printing_id_t;

typedef enum
{
    NX_ID_PAUSE_BUTTON_RESUME   = 0x01,
    NX_ID_PAUSE_BUTTON_STOP,
} nx_pause_id_t;


typedef enum
{
    NX_ID_CONTROL_BUTTON_Z_UP   = 0x01,
    NX_ID_CONTROL_BUTTON_Z_DOWN,
    NX_ID_CONTROL_BUTTON_Z_HOMMING
} nx_control_id_t;

typedef enum
{
    NX_ID_INIT_BUTTON_RESERVE   = 0x01,
} nx_init_id_t;

/** VALUE :: PAGE **/

typedef enum
{
    NX_VALUE_CONTROL_Z_000_1_MM  = 0x01,
    NX_VALUE_CONTROL_Z_001_0_MM,
    NX_VALUE_CONTROL_Z_005_0_MM,
    NX_VALUE_CONTROL_Z_010_0_MM,
    NX_VALUE_CONTROL_Z_050_0_MM,
    NX_VALUE_CONTROL_Z_100_0_MM
} nx_control_value_t;


#endif /* SN_SYS_SERIAL_NEXTION_ */
/**@}*/
