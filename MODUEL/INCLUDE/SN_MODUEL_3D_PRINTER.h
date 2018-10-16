/*
 * SN_MODUEL_3D_PRINTER.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#ifndef SN_MODUEL_3D_PRINTER
#define SN_MODUEL_3D_PRINTER

/** Static Define **/
#define BYTE_SIZE   SN_SYS_SERIAL_COMM_RX_REALTIME
#define BAUD_RATE   SN_SYS_SERIAL_COMM_BAUD_RATE_115200
#define RETURN_MODE SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN

#define FIRST_SLICE_DELAY_TIME 5000 //ms ( 5sec )

#ifdef __APPLE__
#define UART_DEVICE "/dev/cu.usbmodem1421"
#endif
#ifdef linux
#define UART_DEVICE "/dev/ttyACM0"
#endif

#define UART_OFLAGS  (O_RDWR | O_NOCTTY | O_NONBLOCK)

/** Module Message **/
typedef enum {
    MSG_3D_PRINTER_PRINTING_INIT = 0,
    MSG_3D_PRINTER_PRINTING_CYCLE,
    MSG_3D_PRINTER_PRINTING_FINISH,
    MSG_3D_PRINTER_PRINTING_PAUSE,
    MSG_3D_PRINTER_PRINTING_RESUME,
    MSG_3D_PRINTER_PRINTING_STOP,
    MSG_3D_PRINTER_HOMMING,
    MSG_3D_PRINTER_STANDBY,
    MSG_3D_PRINTER_NONE
} evt3DPrinter_t;

typedef enum {
    DEVICE_STANDBY,
    DEVICE_INIT,
    DEVICE_PRINTING,
    DEVICE_PAUSE,
    DEVICE_RESUME,
    DEVICE_STOP,
    DEVICE_FINISH,
    DEVICE_BUSY,
    DEVICE_HOMING,
    DEVICE_NONE
} deviceState_t;

typedef struct moduel_3d_printer {
    deviceState_t   state;

    /** Print **/
    printInfo_t printInfo;
    uint32_t   sliceIndex;

    /* Gcode */
    char gcodeLiftUp[30];
    char gcodeLiftDown[30];

    bool exitFlag;
} module3DPrinter_t;


#endif /* MODUEL_HW_UART_H_ */
