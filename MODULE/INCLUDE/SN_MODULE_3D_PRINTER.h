/*
 * SN_MODUEL_3D_PRINTER.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#ifndef SN_MODUEL_3D_PRINTER
#define SN_MODUEL_3D_PRINTER

/**** MODULE ****/
extern SN_STATUS SN_MODULE_3D_PRINTER_Init(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Uninit(void);

/**** PRINTING ****/
extern SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex);
extern SN_STATUS SN_MODULE_3D_PRINTER_Stop(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Pause(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Resume(void);

/**** CONTROL ****/
extern SN_STATUS SN_MODULE_3D_PRINTER_MOTOR_INIT(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_HOMING(void);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_UP(float mm);
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_DOWN(float mm);
#endif /* MODUEL_HW_UART_H_ */
