/*
 * SN_MACHINE_CONTROL.h
 *
 *  Created on: 2018. 12. 13.
 *      Author: BartKim
 */

#ifndef SN_MACHINE_CONTROL_H_
#define SN_MACHINE_CONTROL_H_

typedef struct frameBuffer_Window
{
    const char* name;

    int            w;
    int            h;
    int          bpp;

    long  screenSize;
} FB_Window_t;

extern FB_Window_t* SN_MACHINE_CONTROL_DisplayCheck(void);

#endif /* SN_MACHINE_CONTROL_H_ */
