/**
@mainpage SN3D Main Page
  @section INFO Project Info
  - Name  :  SN3D 3D Printer.
 */
/**
 * @file SN_API.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup API API
 * @brief SN_API 0.0.1v
 * @{
 */

#ifndef SN_API_H_
#define SN_API_H_

/* Standard */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

/* File System */
#include <dirent.h>
#include <fcntl.h>

/* Serial */
#include <termios.h>
#include <unistd.h>

/* Type */
#ifndef bool
typedef int bool;
#define true 1
#define false 0
#endif


/*************************************************************
 * @name SN3D System
 * @brief
 * @{
 */
#include "SN_SYSTEM.h"

/*************************************************************@}*/



/*************************************************************
 * @name SN3D Module
 * @brief
 * @{
 */
#include "SN_MODULE.h"

/*************************************************************@}*/






/*************************************************************
 * @name APP Message
 * @brief
 * @{
 */
#include "APP_MESSAGES.h"

/*************************************************************@}*/


#endif /* SN_API_H_ */
/**@}*/
