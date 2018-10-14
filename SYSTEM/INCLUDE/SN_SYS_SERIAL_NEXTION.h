/*
 * SN_SYS_SERIAL_NEXTION.h
 *
 *  Created on: Sep 27, 2018
 *      Author: pi
 */

#ifndef SN_SYS_SERIAL_NEXTION_H_
#define SN_SYS_SERIAL_NEXTION_H_

typedef union Nextion_message {

   struct {
           uint8_t page;
           uint8_t type;
           uint8_t id;
           uint8_t value;
           uint8_t endCode[4];
   };
   uint8_t  NXmessage8bit[8];
   uint32_t NXmessage[2];
} msgNX_t;

/** Nextion Display UART Message **/
/* * * * * * * * * * * * * * * * * * * * * * * *

    Nextion Display Command.

    [page] [id] [num] [value]          32bit

    [reserve] [endCode[][][]]        32bit

 * * * * * * * * * * * * * * * * * * * * * * * */

#define NX_ENDCODE 0xFFFFFFFF

/*** COMMON COMMAND ***/
typedef enum
{
    NX_PAGE_WAITING = 1,
    NX_PAGE_CONTROl,
    NX_PAGE_FILE_SELECT,
    NX_PAGE_PRINTING,
    NX_PAGE_PAUSE,
    NX_PAGE_SETUP,
    NX_PAGE_INIT,
    NX_PAGE_NONE
} nx_page_t;

typedef enum
{
    NX_TYPE_BUTTON = 1,
    NX_TYPE_TOGGLE,
    NX_TYPE_SCREEN_TOUCH,
    NX_TYPE_NONE
} nx_type_t;

typedef enum
{
    NX_ID_BUTTON_HOME = 0x1F,
    NX_ID_BUTTON_PRINT,
    NX_ID_BUTTON_YES,
    NX_ID_BUTTON_NO,
    NX_ID_NONE
} nx_id_t;

typedef enum
{
    NX_VALUE_NOT_USED = 0x00
} nx_value_t;

/*** ID COMMAND ***/

typedef enum
{
    NX_ID_WAITING_BUTTON_PRINT   = 0x01,
    NX_ID_WAITING_BUTTON_CONTROL,
} nx_waiting_id;

typedef enum
{
    NX_ID_FILE_SELECT_BUTTON_OPTION_UP   = 0x01,
    NX_ID_FILE_SELECT_BUTTON_OPTION_DOWN,
    NX_ID_FILE_SELECT_BUTTON_PAGE_UP,
    NX_ID_FILE_SELECT_BUTTON_PAGE_DOWN,
    NX_ID_FILE_SELECT_BUTTON_FILE_SELECT,
    NX_ID_FILE_SELECT_BUTTON_PRINT_START
} nx_file_select_id;

typedef enum
{
    NX_ID_PRINTING_BUTTON_PAUSE   = 0x01,
    NX_ID_PRINTING_BUTTON_STOP,
} nx_printing_id;

typedef enum
{
    NX_ID_PAUSE_BUTTON_RESUME   = 0x01,
    NX_ID_PAUSE_BUTTON_STOP,
} nx_pause_id;


typedef enum
{
    NX_ID_CONTROL_BUTTON_Z_UP   = 0x01,
    NX_ID_CONTROL_BUTTON_Z_DOWN,
    NX_ID_CONTROL_BUTTON_Z_HOMMING
} nx_control_id;

typedef enum
{
    NX_ID_INIT_BUTTON_RESERVE   = 0x01,
} nx_init_id;


#endif /* SN_SYS_SERIAL_NEXTION_ */
