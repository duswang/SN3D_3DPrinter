/*
 * serial.h
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stropts.h>

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include <errno.h>

extern int set_interface_attribs(int fd, int speed, int parity);
extern void set_blocking(int fd, int should_block);
extern unsigned char rx_uart(int fd, int bytes);

#endif /* SERIAL_H_ */
