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


pthread_mutex_t ptmSerial[MAX_NUM_OF_SERIAL] = { \
                                  PTHREAD_MUTEX_INITIALIZER, \
                                  PTHREAD_MUTEX_INITIALIZER, \
                                  PTHREAD_MUTEX_INITIALIZER, \
                                  PTHREAD_MUTEX_INITIALIZER };
pthread_mutex_t ptmSerialArray  = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptSerial[MAX_NUM_OF_SERIAL];

/** Global Variables **/

/** Static Funtions **/
/* system */

/* hdlr */
static SN_STATUS  sSerial_RX_Hdlr_r(sysSerialId serialId);

/* local */
static uint32_t   sSerialInterfaceInit(const char *device, uint32_t oflags);
static uint32_t   sSerialinterfaceSetattribs(uint32_t uartID, uint32_t baud_rate, uint32_t parity);
static void       sSerialSetBlocking(uint32_t uartID, bool should_block);

static bool       sSerial_RX_Parsing_NotCustom(sysSerialId serialId);
static bool       sSerial_RX_Parsing_NextionsDisplay(sysSerialId serialId);

/* Globla Variables */
static uint32_t guiNumSerial;
static sysSerialQ aSerial[MAX_NUM_OF_SERIAL];

/* Thread Functions Def */
sysSerialThreadFuncdef(0)
sysSerialThreadFuncdef(1)
sysSerialThreadFuncdef(2)
sysSerialThreadFuncdef(3)
//sysSerialThreadFuncdef(num) - CHECK => ( num < MAX_NUM_OF_SERIAL )

void* (*pfSerialThreadFunc[MAX_NUM_OF_SERIAL])()     = { \
                                  sysSerialThreadFunc(0), \
                                  sysSerialThreadFunc(1), \
                                  sysSerialThreadFunc(2), \
                                  sysSerialThreadFunc(3)  \
                                  //sysSerialThreadFunc(num) - CHECK => ( num < MAX_NUM_OF_SERIAL )
                          };

int SN_SYS_SerialInit(void)
{
    int i = 0;
    int retValue = 0;

    if (pthread_mutex_init(&ptmSerialArray, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    for(i = 0; i < MAX_NUM_OF_SERIAL; i++)
    {
        aSerial[i].isAvailable = false;
        aSerial[i]._serialId    = NULL;
    }

    return retValue;
}

sysSerialId SN_SYS_SerialCreate(const sysSerialDef_t* serialDef, void* (*pfCallBack)(char*))
{
    uint32_t idxSerial = 0;
    SN_STATUS retStatus = SN_STATUS_OK;
    sysSerialId serialId = malloc(sizeof(struct sys_serial_id));

    if(guiNumSerial == (MAX_NUM_OF_SERIAL - 1))
    {
        free(serialId);
        return NULL;
    }

    pthread_mutex_lock(&ptmSerialArray);

    for (idxSerial = 0; idxSerial < MAX_NUM_OF_SERIAL; idxSerial++)
    {
        if (!aSerial[idxSerial].isAvailable)
        {
            aSerial[idxSerial].isAvailable = true;

            serialId->_serialDef           = serialDef;
            serialId->pfSerialCallBack     = pfCallBack;
            serialId->uartId               = sSerialInterfaceInit(serialDef->device, serialDef->oflags);

            if(serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
            {
                sSerialinterfaceSetattribs(serialId->uartId, serialDef->baudRate, 0);
                sSerialSetBlocking(serialId->uartId, true);
            }

            aSerial[idxSerial]._serialId = serialId;

            guiNumSerial++;

            if (pthread_mutex_init(&ptmSerial[idxSerial], NULL) != 0)
            {
                printf("\n mutex init failed\n");
            }

            if((retStatus = pthread_create(&ptSerial[idxSerial], NULL, pfSerialThreadFunc[idxSerial], NULL)))
            {
                printf("Thread Creation Fail %d\n", retStatus);
                fflush(stdout);
            }


            break;
        }
    }
    pthread_mutex_unlock(&ptmSerialArray);

    return serialId;
}

SN_STATUS SN_SYS_SerialRemove(sysSerialId serialId)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    int i = 0;

    if(serialId == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    pthread_mutex_lock(&ptmSerialArray);

    for(i = 0; i < MAX_NUM_OF_SERIAL; i++)
    {
        if((aSerial[i].isAvailable) && (aSerial[i]._serialId->uartId == serialId->uartId))
        {
            aSerial[i].isAvailable = false;

            free(aSerial[i]._serialId);

            if(guiNumSerial != 0)
            {
                guiNumSerial--;
            }
        }
    }

    pthread_mutex_unlock(&ptmSerialArray);

    return retStatus;
}

int SN_SYS_SerialTx(sysSerialId serialId, char* buffer, size_t bufferSize)
{
    int count = 0;
    unsigned char nxReturn[1];

    if(serialId == NULL)
    {
        return -1;
    }

    if (serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
#if(TX_DEBUG)
        printf("TX DATA => %s\n", buffer); fflush(stdout);
#endif
        count = write(serialId->uartId, buffer, bufferSize - 1);

        switch(serialId->_serialDef->returnMode)
        {
        case SN_SYS_SERIAL_COMM_TX_RETURN:
            break;
        case SN_SYS_SERIAL_COMM_TX_CARRIAGE_RETURN:
            count = write(serialId->uartId, CARRIAGE_RETURN, RETURN_SIZE);
            break;
        case SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN:
            count = write(serialId->uartId, CARRIAGE_RETURN, RETURN_SIZE);
            break;
        case SN_SYS_SERIAL_COMM_TX_NX_RETURN:
            nxReturn[0] = NX_RETURN;
            count = write(serialId->uartId, nxReturn, RETURN_SIZE);
            count = write(serialId->uartId, nxReturn, RETURN_SIZE);
            count = write(serialId->uartId, nxReturn, RETURN_SIZE);
            break;
        default:
            break;
        }

        if (count < 0)
        {
            printf("UART TX error\n");
            fflush(stdout);
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
static uint32_t sSerialInterfaceInit(const char *device, uint32_t oflags)
{
    uint32_t uartID = 0;

    /* open the device */
    uartID = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (uartID == SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        perror(device);
        printf("Failed to Open %s \n", device);
        fflush(stdout);
        exit(-1);
    }

    return uartID;
}
static uint32_t sSerialinterfaceSetattribs(uint32_t uartID, uint32_t baud_rate, uint32_t parity)
{
    if (uartID == SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        printf("Invaild uart id. \n"); fflush(stdout);
        exit(-1);
    }

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

static void sSerialSetBlocking(uint32_t uartID, bool should_block)
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


static bool sSerial_RX_Parsing_NextionsDisplay(sysSerialId serialId)
{
    char rx_buffer[SN_SYS_SERIAL_COMM_BUFFER_SIZE];
    int i = 0;
    bool rxDone = false;

    serialId->state.rx_length = read(serialId->uartId, (void*)rx_buffer, ((serialId->_serialDef->rxByteSize == SN_SYS_SERIAL_COMM_RX_REALTIME) ? 255 : serialId->_serialDef->rxByteSize));

    if(serialId->state.rx_length < 0)
    {
        //An error occured (will occur if there are no bytes)
    }
    else if(serialId->state.rx_length == 0)
    {
        serialId->state.rx_length = 0;
        serialId->state.buffer_length = 0;
        //No data waiting
    }
    else
    {
        for(i = 0; i < serialId->state.rx_length; i++)
        {
            serialId->_serialDef->buffer[serialId->state.buffer_length++] = rx_buffer[i];


            if(rx_buffer[i] == 0xFF)
            {
                serialId->state.endCodeChk++;
            }
        }

        //Finished.
        if(serialId->state.endCodeChk >= 3)
        {
            serialId->_serialDef->buffer[serialId->state.buffer_length] = '\0';
            serialId->state.rx_length     = 0;
            serialId->state.buffer_length = 0;
            serialId->state.endCodeChk    = 0;

            serialId->pfSerialCallBack(serialId->_serialDef->buffer);

            serialId->_serialDef->buffer[0] = '\0';

            rxDone = true;
        }
    }

    return rxDone;
}

static bool sSerial_RX_Parsing_NotCustom(sysSerialId serialId)
{
    char rx_buffer[SN_SYS_SERIAL_COMM_BUFFER_SIZE];
    int i = 0;
    bool rxDone = false;

    serialId->state.rx_length = read(serialId->uartId, (void*)rx_buffer, ((serialId->_serialDef->rxByteSize == SN_SYS_SERIAL_COMM_RX_REALTIME) ? 255 : serialId->_serialDef->rxByteSize));

    if(serialId->state.rx_length < 0)
    {
        //An error occured (will occur if there are no bytes)
    }
    else if(serialId->state.rx_length == 0)
    {
        serialId->state.rx_length     = 0;
        serialId->state.buffer_length = 0;
        serialId->state.endCodeChk    = 0;
        //No data waiting
    }
    else
    {
        for(i = 0; i < serialId->state.rx_length; i++)
        {
            serialId->_serialDef->buffer[serialId->state.buffer_length++] = rx_buffer[i];
        }

        if(serialId->_serialDef->rxByteSize == SN_SYS_SERIAL_COMM_RX_REALTIME)
        {
            serialId->state.buffer_length = serialId->state.rx_length;
        }
        else if(serialId->state.buffer_length >= serialId->_serialDef->rxByteSize)
        {
            serialId->state.buffer_length = serialId->_serialDef->rxByteSize;
        }

        //Finished.
        if(serialId->state.buffer_length >= serialId->_serialDef->rxByteSize)
        {
            serialId->_serialDef->buffer[serialId->state.buffer_length] = '\0';
            serialId->state.rx_length = 0;
            serialId->state.buffer_length = 0;

            serialId->pfSerialCallBack(serialId->_serialDef->buffer);

            serialId->_serialDef->buffer[0] = '\0';

            rxDone = true;
        }
    }

    return rxDone;
}

/** reentrant fucntions **/
static SN_STATUS sSerial_RX_Hdlr_r(sysSerialId serialId)
{
    bool rxDone = false;

    serialId->state.rx_length = 0;
    serialId->state.buffer_length = 0;
    serialId->state.endCodeChk = 0;

    if(serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        while(!rxDone)
        {
            switch(serialId->_serialDef->returnMode)
            {
            case SN_SYS_SERIAL_COMM_TX_NX_RETURN:
                rxDone = sSerial_RX_Parsing_NextionsDisplay(serialId);
                break;
            case SN_SYS_SERIAL_COMM_TX_RETURN:
            case SN_SYS_SERIAL_COMM_TX_CARRIAGE_RETURN:
            case SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN:
            default:
                rxDone= sSerial_RX_Parsing_NotCustom(serialId);
                break;
            }
        }
    }
    else
    {
        printf("error read UART\n"); fflush(stdout);
    }

    return SN_STATUS_OK;
}
