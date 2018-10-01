/*
 * SN_SYS_SERIAL_COMM.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_SERIAL_COMM.h"

//OPEN THE UART
        //The flags (defined in fcntl.h):
        //      Access modes (use 1 of these):
        //              O_RDONLY - Open for reading only.
        //              O_RDWR - Open for reading and writing.
        //              O_WRONLY - Open for writing only.
        //
        //      O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
        //                                                                                      if there is no input immediately available (instead of blocking). Likewise, write requests can also return
        //                                                                                      immediately with a failure status if the output can't be written immediately.
        //
        //      O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
int SN_SYS_SERIAL_COMM_init_interface(const char *fileName, int oflags)
{
  int uartID = 0;

  /* open the device */
  uartID = open(fileName, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (uartID != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
  {
      printf("Completed to init %s \n", fileName);
      fflush(stdout);
  }
  else
  {
      perror(fileName);
      printf("Failed to init %s \n", fileName);
      fflush(stdout);
      exit(-1);
  }

  return uartID;
}
int SN_SYS_SERIAL_COMM_set_interface_attribs(int uartID, int baud_rate, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (uartID, &tty) != 0)
    {
        printf ("error %d from tcgetattr\n", errno);
        fflush(stdout);
        return -1;
    }

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 10;            // 0.5 seconds read timeou

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    //tty.c_cflag &= ~CRTSCTS;

    if(tcsetattr(uartID, TCSANOW, &tty) != 0)
    {
        return -1;
    }

    return 0;
}

void SN_SYS_SERIAL_COMM_set_blocking(int uartID, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr(uartID, &tty) != 0)
    {
        printf("error %d from tggetattr\n", errno);
        fflush(stdout);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 10;

    if (tcsetattr(uartID, TCSANOW, &tty) != 0)
    {
        printf("error %d setting term attributes\n", errno);
        fflush(stdout);
    }
}

void SN_SYS_SERIAL_COMM_tx_uart(int uartID, char* tx_buffer, int txOption)
{
    int count = 0;

    if (uartID != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        count = write(uartID, tx_buffer, sizeof(tx_buffer) + txOption);

        if (count < 0)
        {
            printf("UART TX error\n");
            fflush(stdout);
        }
    }
}

int SN_SYS_SERIAL_COMM_rx_uart(int uartID, int byte_size, unsigned char* buffer)
{
    int bufferStatus = 0;

    unsigned char rx_buffer[SN_SYS_SERIAL_COMM_BUFFER_SIZE];
    static int rx_length = 0;
    static int buffer_length = 0;
    int i = 0;

    //pthread_mutex_lock(&ptm_Serial);

    if(uartID != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        //Init Static Variables
        rx_length = 0;
        buffer_length = 0;

        //Start Reading Serial Data
        while(1)
        {
            rx_length = read(uartID, (void*)rx_buffer, ((byte_size == SN_SYS_SERIAL_COMM_RX_REALTIME) ? 255 : byte_size));

            if(rx_length < 0)
            {
                //An error occured (will occur if there are no bytes)
            }
            else if(rx_length == SN_SYS_SERIAL_COMM_NO_DATA)
            {
                rx_length = 0;
                buffer_length = 0;
                //No data waiting
            }
            else
            {
                for(i = 0; i < rx_length; i++)
                {
                    buffer[buffer_length++] = rx_buffer[i];
                }

                if(byte_size == SN_SYS_SERIAL_COMM_RX_REALTIME)
                {
                    buffer_length = rx_length;
                }
                else if(buffer_length >= byte_size)
                {
                    buffer_length = byte_size;
                }

                //Finished.
                if(buffer_length >= byte_size)
                {
                    buffer[buffer_length] = '\0';
                    buffer_length = 0;
                    bufferStatus = 1;
                    break;
                }
            }

            fflush(stdout);
      }
  }
  else
  {
      printf("error read UART\n");
  }

  //pthread_mutex_unlock(&ptm_Serial);

  return bufferStatus;
}
