/*
 * SN_SYS_SERIAL_COMM.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SN_SYS_SERIAL_COMM
#define SN_SYS_SERIAL_COMM

/** Global Define **/
#define MAX_NUM_OF_SERIAL 4

typedef struct sys_serial_stat {
  int buffer_length;
  int rx_length;
  int mode;
} sysSerialStat_t;

typedef struct sys_serial_def {
    const char* device;
    int oflags;
    int baudRate;
    int rxByteSize;
    int returnMode;
    char*  buffer;
} sysSerialDef_t;

typedef struct sys_serial_id {
    const sysSerialDef_t* _serialDef;
    sysSerialStat_t state;
    void* (*pfSerialCallBack)(char* rxBuffer);
    uint32_t    uartId;
} *sysSerialId;

typedef struct sys_serial_q {
    sysSerialId _serialId;
    bool isAvailable;
} sysSerialQ;

#define sysSerialDef(name, device, oflags, baudRate, rxByteSize, rxReturnMode)   \
    char buffer##name[SN_SYS_SERIAL_COMM_BUFFER_SIZE]; \
    const sysSerialDef_t sys_serial_def_##name = \
    { (device), (oflags), (baudRate), (rxByteSize), (rxReturnMode),buffer##name }

#define sysSerialThreadFuncdef(num) \
    static void* sSerialThread_##num() \
    { \
        while(true) \
        { \
            pthread_mutex_lock(&ptmSerial[(num)]);\
            if(aSerial[(num)].isAvailable) sSerial_RX_Hdlr_r(aSerial[(num)]._serialId); \
            pthread_mutex_unlock(&ptmSerial[(num)]); \
        } \
        return NULL; \
    }

#define sysSerial(name)  \
    (&sys_serial_def_##name)

#define sysSerialThreadFunc(num) \
    (sSerialThread_##num)

/** Def Option **/
#define SN_SYS_SERIAL_COMM_BAUD_RATE_9600 B9600
#define SN_SYS_SERIAL_COMM_BAUD_RATE_19200 B19200
#define SN_SYS_SERIAL_COMM_BAUD_RATE_115200 B115200

#define SN_SYS_SERIAL_COMM_RX_REALTIME 0
#define SN_SYS_SERIAL_COMM_RX_BYTE_4 4
#define SN_SYS_SERIAL_COMM_RX_BYTE_8 8
#define SN_SYS_SERIAL_COMM_RX_BYTE_16 16
#define SN_SYS_SERIAL_COMM_RX_BYTE_32 32

#define SN_SYS_SERIAL_COMM_TX_RETURN          0
#define SN_SYS_SERIAL_COMM_TX_CARRIAGE_RETURN 1
#define SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN  2

#define SN_SYS_SERIAL_COMM_BUFFER_SIZE 255

/** ERROR CODE **/
#define SN_SYS_SERIAL_COMM_INVAILD_UART_ID (-1)


/** Static Define **/

#define CARRIAGE_RETURN "\r"
#define NEW_LINE_RETURN "\n"
#define RETURN_SIZE     1
/** Functions **/
extern int         SN_SYS_SerialInit (void);
extern sysSerialId SN_SYS_SerialCreate(const sysSerialDef_t* serialDef, void* (*pfCallBack)(char*));
extern SN_STATUS   SN_SYS_SerialRemove(sysSerialId serialId);
extern int         SN_SYS_SerialTx(sysSerialId serialId, char* buffer, size_t bufferSize);


#endif /* SN_SYS_SERIAL_COMM */
