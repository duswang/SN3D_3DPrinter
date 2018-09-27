/*
 * serial.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stropts.h>

#include <pthread.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

extern int  SERIAL_COMM_init(void);
extern int  SERIAL_COMM_init_interface(const char *fileName, int oflags);
extern int  SERIAL_COMM_set_interface_attribs(int uartID, int baudRate, int parity);
extern void SERIAL_COMM_set_blocking(int uartID, int should_block);

extern int  SERIAL_COMM_rx_uart(int uartID, int bytes_size, unsigned char* rx_buffer);
extern void SERIAL_COMM_tx_uart(int uartID, char* tx_buffer, int txOption);



#define SERIAL_COMM_BAUD_RATE_9600 B9600
#define SERIAL_COMM_BAUD_RATE_19200 B19200
#define SERIAL_COMM_BAUD_RATE_115200 B115200

#define SERIAL_COMM_RX_REALTIME 0
#define SERIAL_COMM_RX_BYTE_4 4
#define SERIAL_COMM_RX_BYTE_8 8
#define SERIAL_COMM_RX_BYTE_16 16
#define SERIAL_COMM_RX_BYTE_32 32

#define SERIAL_COMM_TX_NORMAL 0
#define SERIAL_COMM_TX_WITH_NULL -1


#define SERIAL_COMM_NO_DATA 0
#define SERIAL_COMM_ON_DATA 1

#define SERIAL_COMM_BUFFER_SIZE 255

#endif /* SERIAL_H_ */
