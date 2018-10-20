/*
 * SN_SYS_SERIAL_GCODE.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#ifndef SN_SYS_SERIAL_GCODE_H_
#define SN_SYS_SERIAL_GCODE_H_

/**** GCODE ****/
#define GCODE_INIT_SET_MM                "G21"
#define GCODE_INIT_POSITION_ABSOLUTE     "G90"
#define GCODE_INTT_POSITION_RELATIVE     "G91"
#define GCODE_INIT_MOTOR                 "M17"

#define GCODE_CLEAR_BUFFER               "M400"

#define GCODE_DEVICE_STOP                "M0"
#define GCODE_UNINIT_MOTOR               "M18"

#define GCODE_HOMING                     "G28 Z0 F150.0"

#define GCODE_LCD_ON                     "M106 S25"
#define GCODE_LCD_OFF                    "M107"

#define GCODE_GET_CURRENT_POSITION       "M114"

#define GCODE_LIFT_UNINIT                ""


/**** GCODE DEFAULT VALUSE ****/
#define DEFAULT_FEEDRATE                 (150.0)
#define DEFAULT_FAST_FEEDRATE            (300.0)

#define GCODE_MOTOR_RESPONSE                "k"
#define GCODE_GET_CURRENT_POSITION_RESPONSE "X"

#endif /* APP_COMMANDS_H_ */
