/*
 * MODUELS.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef MODUELS_H_
#define MODUELS_H_

#include "GCode_Commands.h"
#include "Nextion_Commands.h"

extern void MODUELS_DISPLAY_Serial_Init(void);
extern void MODUELS_DISPLAY_Serial_Uninit(void);

extern void MODUELS_3D_PRINTER_Serial_Init(void);
extern void MODUELS_3D_PRINTER_Serial_Uninit(void);

//@DEBUG

extern void MODUELS_3D_PRINTER_FOR_DEMO(void);

#endif /* MODUELS_H_ */
