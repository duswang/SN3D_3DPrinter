/*
 * MODUEL_HW_UART.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */
#include <MODUELS.h>
#include <MODUEL_3D_PRINTER.h>

#include <SERIAL_COMM.h>

#define BYTE_SIZE SERIAL_COMM_RX_REALTIME
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
      if(SERIAL_COMM_rx_uart(uartID, BYTE_SIZE, rx_buffer_3DPrinter))
      {
          printf("%s", rx_buffer_3DPrinter);
          fflush(stdout);
      }
  }

  return 0;
}


void MODUELS_3D_PRINTER_Init(void)
{

}

void MODUELS_3D_PRINTER_Serial_Init(void)
{
  int rc1;
  pthread_t pt3DPrinter;

  uartID = SERIAL_COMM_init_interface(UART_DEVICE, UART_FLAGS);

  SERIAL_COMM_set_interface_attribs(uartID, BAUD_RATE, 0);
  SERIAL_COMM_set_blocking(uartID, 1);

  sleep(3);

  if (pthread_mutex_init(&ptm_3DPrinter, NULL) != 0)
  {
      printf("\n mutex init failed\n");
  }

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

void MODUELS_3D_PRINTER_FOR_DEMO(void)
{
    SERIAL_COMM_tx_uart(uartID, GCODE_LCD_ON, SERIAL_COMM_TX_WITH_NULL);
    sleep(1);

    SERIAL_COMM_tx_uart(uartID, GCODE_LCD_OFF, SERIAL_COMM_TX_WITH_NULL);
    sleep(1);
}
