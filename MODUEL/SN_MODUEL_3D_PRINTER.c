/*
 * SN_MODUEL_3D_PRINTER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: pi
 */

#include "SN_API.h"
#include "SN_MODUEL_3D_PRINTER.h"

// Serial Info
#define BYTE_SIZE SN_SYS_SERIAL_COMM_RX_REALTIME
#define BAUD_RATE SN_SYS_SERIAL_COMM_BAUD_RATE_115200

#if(0)
#define UART_DEVICE "/dev/ttyACM0"
#else
#define UART_DEVICE "/dev/cu.usbmodem1421"
#endif
#define UART_FLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

pthread_mutex_t ptm3DPrinter = PTHREAD_MUTEX_INITIALIZER;
pthread_t        pt3DPrinter;
static int uartID;

unsigned char rxBuffer3DPrinter[SN_SYS_SERIAL_COMM_BUFFER_SIZE];

/** static functions **/
static void* s3DPrinterThread();



static void* s3DPrinterThread()
{
  while(true)
  {
      if(SN_SYS_SERIAL_COMM_rx_uart(uartID, BYTE_SIZE, rxBuffer3DPrinter))
      {
          printf("%s", rxBuffer3DPrinter);
          fflush(stdout);
      }
  }

  return 0;
}

void SN_MODUEL_3D_PRINTER_Init(void)
{
  int retValue;

  uartID = SN_SYS_SERIAL_COMM_init_interface(UART_DEVICE, UART_FLAGS);

  if(uartID != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
  {
      SN_SYS_SERIAL_COMM_set_interface_attribs(uartID, BAUD_RATE, 0);
      SN_SYS_SERIAL_COMM_set_blocking(uartID, 1);

      sleep(3);

      if (pthread_mutex_init(&ptm3DPrinter, NULL) != 0)
      {
          printf("\n mutex init failed\n");
      }

      if((retValue = pthread_create(&pt3DPrinter, NULL, s3DPrinterThread, NULL)))
      {
          printf("Thread Creation Fail %d\n", retValue);
          fflush(stdout);
      }
      else
      {
          printf("Start Serial Monitor %s\n", UART_DEVICE);
          fflush(stdout);
      }
  }
}

void SN_MODUEL_3D_PRINTER_Uninit(void)
{
    close(uartID);
}

