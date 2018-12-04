/*
 * SN_SYS_SERIAL_GCODE.h
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */
/**
 * @file SN_SYS_SERIAL_GCODE.h
 * @author Bato
 * @date 27 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_SERIAL_GCODE GCode Command
 * @ingroup SYSTEM_SERIAL_COMM
 * @brief GCode command
 * @{
 */
#ifndef SN_SYS_SERIAL_GCODE_H_
#define SN_SYS_SERIAL_GCODE_H_

/*************************************************************
 * @name GCODE
 *
 * @see https://reprap.org/wiki/G-code
 *////@{
#define GCODE_INIT_SET_MM                "G21"
#define GCODE_INIT_POSITION_ABSOLUTE     "G90"
#define GCODE_INTT_POSITION_RELATIVE     "G91"
#define GCODE_INIT_MOTOR                 "M17"

#define GCODE_CLEAR_BUFFER               "M400"

#define GCODE_DEVICE_STOP                "M0"
#define GCODE_UNINIT_MOTOR               "M18"

#define GCODE_HOMING                     "G28 Z0 F150.0"

#define GCODE_LCD_ON                     "M106 S255"
#define GCODE_LCD_OFF                    "M107"

#define GCODE_GET_CURRENT_POSITION       "M114"

#define GCODE_LIFT_UNINIT                ""

/*************************************************************@}*/

/*************************************************************
 * @name GCODE DEFAULT VALUE
 *
 *////@{

#define DEFAULT_FEEDRATE                 (150.0)

/*************************************************************@}*/

/*************************************************************
 * @name GCODE RESPONSE CONDITION
 *
 *////@{

#define GCODE_MOTOR_RESPONSE                    "k"
#define GCODE_GET_CURRENT_X_POSITION_RESPONSE     "X:"
#define GCODE_GET_CURRENT_Y_POSITION_RESPONSE     "Y:"
#define GCODE_GET_CURRENT_Z_POSITION_RESPONSE     "Z:"

/*************************************************************@}*/
#endif /* APP_COMMANDS_H_ */
/**@}*/
