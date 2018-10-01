/*
 * SN_MODUEL_DISPLAY.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */

#include "SN_API.h"
#include "SN_MODUEL_DISPLAY.h"


#define BYTE_SIZE SN_SYS_SERIAL_COMM_RX_BYTE_8
#define BAUD_RATE  SN_SYS_SERIAL_COMM_BAUD_RATE_9600
#define UART_DEVICE "/dev/serial0"
#define UART_FLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

pthread_mutex_t ptmDisplay = PTHREAD_MUTEX_INITIALIZER;
pthread_t        ptDisplay;

static int uartID;
static unsigned char rxBufferDisplay[SN_SYS_SERIAL_COMM_BUFFER_SIZE];

/** static functions */
static void* sDisplayThread();

static void* sDisplayThread()
{
    int i = 0;

    while(1)
    {
        if(SN_SYS_SERIAL_COMM_rx_uart(uartID, BYTE_SIZE, rxBufferDisplay))
        {
            //byte data
            printf("%i bytes read :", BYTE_SIZE);

            while(i < BYTE_SIZE)
            {
                printf(" 0x%02x", (unsigned int) rxBufferDisplay[i++]);
            }

            printf("\n");
            fflush(stdout);

            i = 0;
        }
    }

    return 0;
}

void SN_MODUEL_DISPLAY_Init(void)
{
  int retValue;

  uartID = SN_SYS_SERIAL_COMM_init_interface(UART_DEVICE, UART_FLAGS);

  SN_SYS_SERIAL_COMM_set_interface_attribs(uartID, BAUD_RATE, 0);
  SN_SYS_SERIAL_COMM_set_blocking(uartID, 1);

  sleep(1);

  if (pthread_mutex_init(&ptmDisplay, NULL) != 0)
  {
      printf("\n mutex init failed\n");
  }

  if((retValue = pthread_create(&ptDisplay, NULL, sDisplayThread, NULL)))
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

void SN_MODUEL_DISPLAY_Uninit(void)
{
    close(uartID);
}
