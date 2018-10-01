/*
 * SN_SYS_SERIAL_COMM.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SN_SYS_SERIAL_COMM
#define SN_SYS_SERIAL_COMM

extern int  SN_SYS_SERIAL_COMM_init(void);
extern int  SN_SYS_SERIAL_COMM_init_interface(const char *fileName, int oflags);
extern int  SN_SYS_SERIAL_COMM_set_interface_attribs(int uartID, int baudRate, int parity);
extern void SN_SYS_SERIAL_COMM_set_blocking(int uartID, int should_block);

extern int  SN_SYS_SERIAL_COMM_rx_uart(int uartID, int bytes_size, unsigned char* rx_buffer);
extern void SN_SYS_SERIAL_COMM_tx_uart(int uartID, char* tx_buffer, int txOption);



#define SN_SYS_SERIAL_COMM_BAUD_RATE_9600 B9600
#define SN_SYS_SERIAL_COMM_BAUD_RATE_19200 B19200
#define SN_SYS_SERIAL_COMM_BAUD_RATE_115200 B115200

#define SN_SYS_SERIAL_COMM_RX_REALTIME 0
#define SN_SYS_SERIAL_COMM_RX_BYTE_4 4
#define SN_SYS_SERIAL_COMM_RX_BYTE_8 8
#define SN_SYS_SERIAL_COMM_RX_BYTE_16 16
#define SN_SYS_SERIAL_COMM_RX_BYTE_32 32

#define SN_SYS_SERIAL_COMM_TX_NORMAL 0
#define SN_SYS_SERIAL_COMM_TX_WITH_NULL (-1)


#define SN_SYS_SERIAL_COMM_NO_DATA 0
#define SN_SYS_SERIAL_COMM_ON_DATA 1

#define SN_SYS_SERIAL_COMM_BUFFER_SIZE 255

#define SN_SYS_SERIAL_COMM_INVAILD_UART_ID (-1)

#endif /* SN_SYS_SERIAL_COMM */
