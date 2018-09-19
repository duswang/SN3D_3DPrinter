/*
 * MODUEL_HW_UART.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */


#include <MODUEL_HW_UART.h>

#include <serial.h>

//#define BAUDRATE 115200
//#define UART_DEVICE "/dev/ttyACM0"
#define BAUDRATE 9600
#define UART_DEVICE "/dev/serial0"


void MODUELS_HW_UART_demo(void)
{
  int fd;

  printf("Start Hardware Uart Monitor \n");
  fflush(stdout);

  /* open the device */
  fd = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (fd == 0)
  {
      perror(UART_DEVICE);
      printf("Failed to open %s \n", UART_DEVICE);
      fflush(stdout);
      exit(-1);
  }
  else
  {
      printf("Completed to open %s \n", UART_DEVICE);
      fflush(stdout);
  }

  set_interface_attribs (fd, BAUDRATE, 0);  // set speed to 115200 bps, 8n1 (no parity)
  //set_blocking (fd, 0);                     // set no blocking

  sleep(3);
  fflush(stdout);

  rx_uart(fd, 8);

  close(fd);
}
