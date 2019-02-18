/**
 * @file SN_SYSTEM.h
 * @author Bato
 * @date 1 Oct 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM System
 * @ingroup API
 */

#ifndef SN_SYSTEM_H_
#define SN_SYSTEM_H_

/* POSIX */
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>

/* System */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>

#include <errno.h>

#include <openssl/md5.h>

/* SN SYSTEM */
#include "SN_SYS_ERROR.h"
#include "SN_SYS_MESSAGE_Q.h"
#include "SN_SYS_TIMER.h"
#include "SN_SYS_SERIAL_COMM.h"
/* Serial Command */
    #include "SN_SYS_SERIAL_NEXTION.h"
    #include "SN_SYS_SERIAL_GCODE.h"
#include "SN_SYS_USB_DRIVER.h"

#endif /* SN_SYSTEM_H_ */
/**@}*/
