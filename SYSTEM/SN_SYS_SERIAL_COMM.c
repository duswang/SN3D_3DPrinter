/*
 * SN_SYS_SERIAL_COMM.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_SERIAL_COMM.h"

/** System **/
/* Thread */
pthread_mutex_t ptmSerial = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptSerial;

/** Global Variables **/

/** Static Funtions **/
/* system */
static void* sSerialThread();

/* hdlr */
static int   sSerial_RX_Hdlr(sysSerialId serialId);

/* local */
static int   sSerialInterfaceInit(const char *device, int oflags);
static int   sSerialinterfaceSetattribs(int uartID, int baud_rate, int parity);
static void  sSerialSetBlocking(int uartID, int should_block);

static void  sSerialOpen(sysSerialId serialId);
static void  sSerialClose(sysSerialId serialId);

/* Globla Variables */
sysSerialQ serialQ[SERIAL_Q_SIZE];

int SN_SYS_SerialInit(void)
{
    int i = 0;
    int retValue = 0;

    for(i = 0; i < SERIAL_Q_SIZE; i++)
    {
        serialQ[i].serialId = NULL;
        serialQ[i].isAvailable = false;
        serialQ[i].serialStatus = SN_SYS_SERIAL_COMM_NONE_SET;
    }

    if (pthread_mutex_init(&ptmSerial, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if((retValue = pthread_create(&ptSerial, NULL, sSerialThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retValue);
        fflush(stdout);
    }

    return retValue;
}

sysSerialId SN_SYS_SerialCreate(const sysSerialDef_t* serialDef, void* pfCallBack)
{
    sysSerialId serialId = malloc(sizeof(struct sys_serial_id));

    serialId->_serialDef  = serialDef;
    serialId->pfSerialCallBack  = pfCallBack;

    serialId->uartId = sSerialInterfaceInit(serialDef->device, serialDef->oflags);

    if(serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        sSerialinterfaceSetattribs(serialId->uartId, serialDef->baudRate, 0);
        sSerialSetBlocking(serialId->uartId, 1);
    }

    sSerialOpen(serialId);

    return serialId;
}

int SN_SYS_SerialRemove(sysSerialId serialId)
{
    if(serialId == NULL)
    {
        return -1;
    }


    sSerialClose(serialId);

    return 0;
}

char* SN_SYS_SerialRx(sysSerialId serialId)
{
    char* retBuffer = NULL;

    if(serialId == NULL)
    {
        return NULL;
    }

    pthread_mutex_lock(&ptmSerial);
    retBuffer = serialId->_serialDef->buffer;
    pthread_mutex_unlock(&ptmSerial);

    return retBuffer;
}


int SN_SYS_SerialTx(sysSerialId serialId, char* buffer, size_t bufferSize)
{
    int count = 0;
    char* CarriageReturn = "\r\n";

    if(serialId == NULL)
    {
        return -1;
    }

    if (serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        printf("TX DATA => %s\n", buffer); fflush(stdout);

        count = write(serialId->uartId, buffer, bufferSize - 1);

        if (count < 0)
        {
            printf("UART TX error\n");
            fflush(stdout);
        }

        count = write(serialId->uartId, CarriageReturn, (ssize_t)2);

        if (count < 0)
        {
            printf("UART TX error\n");
            fflush(stdout);
        }
    }

    return 0;
}

static void* sSerialThread()
{
    int serialIndex = 0;

    while(1)
    {
        for(serialIndex = 0; serialIndex < SERIAL_Q_SIZE; serialIndex++)
        {
            if(serialQ[serialIndex].isAvailable && (serialQ[serialIndex].serialStatus != SN_SYS_SERIAL_COMM_BUSY))
            {
                sSerial_RX_Hdlr(serialQ[serialIndex].serialId);
            }
        }
    }

    return 0;
}

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
static int sSerialInterfaceInit(const char *device, int oflags)
{
  int uartID = 0;

  /* open the device */
  uartID = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (uartID != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
  {
      printf("Completed to init %s \n", device);
      fflush(stdout);
  }
  else
  {
      perror(device);
      printf("Failed to init %s \n", device);
      fflush(stdout);
      exit(-1);
  }

  return uartID;
}
static int sSerialinterfaceSetattribs(int uartID, int baud_rate, int parity)
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

static void sSerialSetBlocking(int uartID, int should_block)
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

static void sSerialOpen(sysSerialId serialId)
{
    int i = 0;

    for(i = 0; i < SERIAL_Q_SIZE; i++)
    {
        if(!serialQ[i].isAvailable)
        {
            serialQ[i].serialId = serialId;
            serialQ[i].isAvailable = true;
            serialQ[i].serialStatus = SN_SYS_SERIAL_COMM_WAITING;
        }
    }
}

static void sSerialClose(sysSerialId serialId)
{
    int i = 0;

    for(i = 0; i < SERIAL_Q_SIZE; i++)
    {
        if(serialQ[i].serialId == serialId && serialQ[i].isAvailable)
        {
            serialQ[i].serialId = NULL;
            serialQ[i].isAvailable = false;
            serialQ[i].serialStatus = SN_SYS_SERIAL_COMM_NONE_SET;

            break;
        }
    }

    free(serialId);
}

static int sSerial_RX_Hdlr(sysSerialId serialId)
{
    char rx_buffer[SN_SYS_SERIAL_COMM_BUFFER_SIZE];
    static int rx_length = 0;
    static int buffer_length = 0;
    int i = 0;

    pthread_mutex_lock(&ptmSerial);

    if(serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        //Init Static Variables
        rx_length = 0;
        buffer_length = 0;

        //Start Reading Serial Data
        while(true)
        {
            rx_length = read(serialId->uartId, (void*)rx_buffer, ((serialId->_serialDef->rxByteSize == SN_SYS_SERIAL_COMM_RX_REALTIME) ? 255 : serialId->_serialDef->rxByteSize));

            if(rx_length < 0)
            {
                //An error occured (will occur if there are no bytes)
            }
            else if(rx_length == 0)
            {
                rx_length = 0;
                buffer_length = 0;
                //No data waiting
            }
            else
            {
                for(i = 0; i < rx_length; i++)
                {
                    serialId->_serialDef->buffer[buffer_length++] = rx_buffer[i];
                }

                if(serialId->_serialDef->rxByteSize == SN_SYS_SERIAL_COMM_RX_REALTIME)
                {
                    buffer_length = rx_length;
                }
                else if(buffer_length >= serialId->_serialDef->rxByteSize)
                {
                    buffer_length = serialId->_serialDef->rxByteSize;
                }

                //Finished.
                if(buffer_length >= serialId->_serialDef->rxByteSize)
                {
                    serialId->_serialDef->buffer[buffer_length] = '\0';
                    buffer_length = 0;
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

  pthread_mutex_unlock(&ptmSerial);

  serialId->pfSerialCallBack();

  return 0;
}
