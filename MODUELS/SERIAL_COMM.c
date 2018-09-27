/*
 * serial.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include <SERIAL_COMM.h>

int SERIAL_COMM_init(const char *fileName, int oflags)
{
  int uartID = 0;

  /* open the device */
  uartID = open(fileName, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (uartID == 0)
  {
      perror(fileName);
      printf("Failed to init %s \n", fileName);
      fflush(stdout);
      exit(-1);
  }
  else
  {
      printf("Completed to init %s \n", fileName);
      fflush(stdout);
  }

  return uartID;
}
int SERIAL_COMM_set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr (fd, &tty) != 0)
    {
        printf ("error %d from tcgetattr\n", errno);
        fflush(stdout);
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetispeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo,
                                    // no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                    // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    //tty.c_cflag &= ~CRTSCTS;

    if(tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        return -1;
    }

    return 0;
}

void SERIAL_COMM_set_blocking (int fd, int should_block)
{
    struct termios tty;
    memset (&tty, 0, sizeof tty);

    if (tcgetattr(fd, &tty) != 0)
    {
        printf("error %d from tggetattr\n", errno);
        fflush(stdout);
        return;
    }

    tty.c_cc[VMIN]  = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 10;            // 0.5 seconds read timeout

    if (tcsetattr(fd, TCSANOW, &tty) != 0)
    {
        printf("error %d setting term attributes\n", errno);
        fflush(stdout);
    }
}
void SERIAL_COMM_tx_uart(int uartID, int bytes_size, unsigned char* tx_buffer)
{

}

int SERIAL_COMM_rx_uart(int uartID, int byte_size, unsigned char* buffer)
{
    int bufferStatus = 0;

    unsigned char rx_buffer[255];
    static int rx_length = 0;
    static int buffer_length = 0;
    int i = 0;

    if(uartID != -1)
    {
        while(1)
        {
            rx_length = read(uartID, (void*)rx_buffer, ((byte_size == SERIAL_COMM_READ_REALTIME) ? 255 : byte_size));

            if(rx_length < 0)
            {
                //An error occured (will occur if there are no bytes)
            }
            else if(rx_length == SERIAL_COMM_NO_DATA)
            {
                //No data waiting
            }
            else
            {
                for(i = 0; i < rx_length; i++)
                {
                    buffer[buffer_length++] = rx_buffer[i];
                }

                if(byte_size == SERIAL_COMM_READ_REALTIME)
                {
                    buffer_length = rx_length;
                }
                else if(buffer_length >= byte_size)
                {
                    buffer_length = byte_size;
                }

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

  return bufferStatus;
}
