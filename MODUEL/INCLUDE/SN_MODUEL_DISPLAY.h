/*
 * SN_MODUEL_DISPLAY.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#ifndef SN_MODUEL_DISPLAY
#define SN_MODUEL_DISPLAY

/** Static Define **/
#define BYTE_SIZE SN_SYS_SERIAL_COMM_RX_BYTE_8
#define BAUD_RATE  SN_SYS_SERIAL_COMM_BAUD_RATE_9600

#define UART_DEVICE "/dev/serial0"
#define UART_OFLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

/** Module Messgae **/
typedef enum {
    MSG_DISPLAY_DATA_RX = 0,
    MSG_DISPLAY_DATA_TX,
    MSG_DISPLAY_RESERVE,
    MSG_DISPLAY_NONE
} evtDisplay_t;

#endif /* SN_MODUEL_DISPLAY */
