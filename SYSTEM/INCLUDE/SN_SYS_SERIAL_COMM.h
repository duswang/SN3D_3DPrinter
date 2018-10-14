/*
 * SN_SYS_SERIAL_COMM.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SN_SYS_SERIAL_COMM
#define SN_SYS_SERIAL_COMM

/** Static Define **/
#define SERIAL_Q_SIZE 4

typedef struct sys_serial_def {
    const char* device;
    int oflags;
    int baudRate;
    int rxByteSize;
    char*  buffer;
} sysSerialDef_t;

typedef struct sys_serial_id {
    const sysSerialDef_t* _serialDef;
    void* (*pfSerialCallBack)();
    int    uartId;
}* sysSerialId;

typedef struct sys_serial_q {
    sysSerialId serialId;
    int isAvailable;
    int serialStatus;
} sysSerialQ;

extern int         SN_SYS_SerialInit (void);
extern sysSerialId SN_SYS_SerialCreate(const sysSerialDef_t* serialDef, void* pfCallBack);
extern int         SN_SYS_SerialRemove(sysSerialId serialId);
extern int         SN_SYS_SerialTx(sysSerialId serialId, char* buffer, size_t bufferSize );
extern char*       SN_SYS_SerialRx(sysSerialId serialId);

#define sysSerialDef(name, device, oflags, baudRate, rxByteSize)   \
char buffer##name[SN_SYS_SERIAL_COMM_BUFFER_SIZE]; \
const sysSerialDef_t sys_serial_def_##name = \
{ (device), (oflags), (baudRate), (rxByteSize), buffer##name }

#define sysSerial(name)  \
&sys_serial_def_##name

/** Def Option **/
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

#define SN_SYS_SERIAL_COMM_BUFFER_SIZE 255

/** STATUS CODE **/
#define SN_SYS_SERIAL_COMM_NONE_SET 0
#define SN_SYS_SERIAL_COMM_WAITING  1
#define SN_SYS_SERIAL_COMM_BUSY     2

/** ERROR CODE **/
#define SN_SYS_SERIAL_COMM_INVAILD_UART_ID (-1)

#endif /* SN_SYS_SERIAL_COMM */
