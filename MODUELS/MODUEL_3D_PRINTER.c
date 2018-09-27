/*
 * MODUEL_HW_UART.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#include <MODUEL_3D_PRINTER.h>

#include <SERIAL_COMM.h>

#define BYTE_SIZE SERIAL_COMM_READ_REALTIME
#define BAUD_RATE SERIAL_COMM_BAUD_RATE_115200
#define UART_DEVICE "/dev/ttyACM0"
#define UART_FLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

pthread_mutex_t ptm_3DPrinter = PTHREAD_MUTEX_INITIALIZER;
static int uartID;

unsigned char rx_buffer_3DPrinter[SERIAL_COMM_BUFFER_SIZE];

void* MODUELS_3D_PRINTER_SerialMonitor()
{
  while(1)
  {
      pthread_mutex_lock(&ptm_3DPrinter);
      if(SERIAL_COMM_rx_uart(uartID, BYTE_SIZE, rx_buffer_3DPrinter))
      {
          printf("%s\n", rx_buffer_3DPrinter);
          fflush(stdout);
      }
      pthread_mutex_lock(&ptm_3DPrinter);
  }
  return 0;
}

void MODUELS_3D_PRINTER_Serial_Init(void)
{
  int rc1;
  pthread_t pt3DPrinter;

  uartID = SERIAL_COMM_init(UART_DEVICE, UART_FLAGS);

  SERIAL_COMM_set_interface_attribs (uartID, BAUD_RATE, 0);
  SERIAL_COMM_set_blocking (uartID, 0);

  sleep(3);

  if((rc1=pthread_create(&pt3DPrinter, NULL, MODUELS_3D_PRINTER_SerialMonitor, NULL)))
  {
      printf("Thread Creation Fail %d\n", rc1);
      fflush(stdout);
  }
  else
  {
      printf("Start Serial Monitor %s\n", UART_DEVICE);
      fflush(stdout);
  }

}

void MODUELS_3D_PRINTER_Serial_Uninit(void)
{
  close(uartID);
}
