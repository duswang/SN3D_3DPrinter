/*
 * MODUEL_DISPLAY.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bart
 */


#include <MODUEL_DISPLAY.h>

#include <SERIAL_COMM.h>

#include <pthread.h>



#define BYTE_SIZE SERIAL_COMM_READ_BYTE_8
#define BAUD_RATE  SERIAL_COMM_BAUD_RATE_9600
#define UART_DEVICE "/dev/serial0"
#define UART_FLAGS  O_RDWR | O_NOCTTY | O_NONBLOCK

pthread_mutex_t ptm_Display = PTHREAD_MUTEX_INITIALIZER;
static int uartID;
unsigned char rx_buffer_Display[SERIAL_COMM_BUFFER_SIZE];

void* MODUELS_DISPLAY_SerialMonitor()
{
    int i = 0;

    while(1)
    {
        pthread_mutex_lock(&ptm_Display);
        if(SERIAL_COMM_rx_uart(uartID, BYTE_SIZE, rx_buffer_Display))
        {
            //byte data
            printf("%i bytes read :", BYTE_SIZE);

            while(i < BYTE_SIZE)
            {
                printf(" 0x%02x", (unsigned int) rx_buffer_Display[i++]);
            }

            printf("\n");
            fflush(stdout);

            i = 0;
        }
        pthread_mutex_unlock(&ptm_Display);
    }

    return 0;
}

void MODUELS_DISPLAY_Messenger(unsigned char* Message)
{

}

void MODUELS_DISPLAY_Serial_Init(void)
{
  int rc1;
  pthread_t ptDisplay;

  uartID = SERIAL_COMM_init(UART_DEVICE, UART_FLAGS);

  SERIAL_COMM_set_interface_attribs (uartID, BAUD_RATE, 0);
  SERIAL_COMM_set_blocking (uartID, 0);

  sleep(3);

  if (pthread_mutex_init(&ptm_Display, NULL) != 0)
  {
      printf("\n mutex init failed\n");
  }

  if((rc1=pthread_create(&ptDisplay, NULL, MODUELS_DISPLAY_SerialMonitor, NULL)))
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

void MODUELS_DISPLAY_Serial_Uninit(void)
{
  close(uartID);
}
