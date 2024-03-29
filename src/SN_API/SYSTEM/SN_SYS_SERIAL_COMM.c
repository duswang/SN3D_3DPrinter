/**
 * @file SN_SYS_SERIAL_COMM.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_SYS_SERIAL_COMM.h"

/* ******* STATIC DEFINE ******* */

/* ******* SYSTEM DEFINE ******* */
/* *** SYSTEM THREAD *** */
pthread_mutex_t ptmSerial[MAX_NUM_OF_SERIAL] = { \
                                  PTHREAD_MUTEX_INITIALIZER, \
                                  PTHREAD_MUTEX_INITIALIZER };
pthread_mutex_t ptmSerialArray  = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptSerial[MAX_NUM_OF_SERIAL];

/* ******* GLOBAL VARIABLE ******* */
static uint32_t guiNumSerial;
static sysSerialQ aSerial[MAX_NUM_OF_SERIAL];

/* ******* STATIC FUNCTIONS ******* */

/* *** HANDLER *** */
static SN_STATUS  sSerial_RX_Hdlr_r(sysSerialId serialId); /**< Reentrant fucntions */

/* *** RX HANDLER *** */
static bool       sSerial_RX_Parsing_NotCustom(sysSerialId serialId);
static bool       sSerial_RX_Parsing_NextionsDisplay(sysSerialId serialId);

/* *** SERIAL *** */
static uint32_t   sSerialInterfaceInit(const char *device, uint32_t oflags);
static uint32_t   sSerialinterfaceSetattribs(uint32_t uartID, uint32_t baud_rate, uint32_t parity);
static void       sSerialSetBlocking(uint32_t uartID, bool should_block);

/* *** Thread Funtion Def *** */
sysSerialThreadFuncdef(0)
sysSerialThreadFuncdef(1)
//sysSerialThreadFuncdef(num) - CHECK => ( num < MAX_NUM_OF_SERIAL )
void* (*pfSerialThreadFunc[MAX_NUM_OF_SERIAL])()     = { \
                                  sysSerialThreadFunc(0), \
                                  sysSerialThreadFunc(1), \
                                  //sysSerialThreadFunc(num) - CHECK => ( num < MAX_NUM_OF_SERIAL )
                          };

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS SN_SYS_SERIAL_Init(void)
{
    SN_STATUS retStauts = SN_STATUS_OK;
    int i = 0;

    if (pthread_mutex_init(&ptmSerialArray, NULL) != 0)
    {
        SN_SYS_ERROR_StatusCheck(SN_STATUS_NOT_INITIALIZED, "Serial System Mutex Init Failed.");
    }

    for(i = 0; i < MAX_NUM_OF_SERIAL; i++)
    {
        aSerial[i].isAvailable = false;
        aSerial[i]._serialId    = NULL;
    }

    return retStauts;
}

SN_STATUS SN_SYS_SERIAL_Uninit(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

sysSerialId SN_SYS_SERIAL_Create(const sysSerialDef_t* serialDef, void* (*pfCallBack)(char*))
{
    uint32_t idxSerial = 0;
    SN_STATUS retStatus = SN_STATUS_OK;
    sysSerialId serialId = malloc(sizeof(struct sys_serial_id));

    if(guiNumSerial == MAX_NUM_OF_SERIAL)
    {
        free(serialId);
        serialId = NULL;

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

SN_STATUS SN_SYS_SERIAL_Remove(sysSerialId serialId)
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
            aSerial[i]._serialId = NULL;

            if(guiNumSerial != 0)
            {
                guiNumSerial--;
            }
        }
    }

    pthread_mutex_unlock(&ptmSerialArray);

    return retStatus;
}

SN_STATUS SN_SYS_SERIAL_Tx(sysSerialId serialId, char* buffer, size_t bufferSize)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    int error = 0;
    char nxReturn[3] = NX_RETURN;


    if(serialId == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if(buffer == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if (serialId->uartId != SN_SYS_SERIAL_COMM_INVAILD_UART_ID)
    {
        error = write(serialId->uartId, buffer, bufferSize - 1);

        switch(serialId->_serialDef->returnMode)
        {
        case SN_SYS_SERIAL_COMM_TX_RETURN:
            break;
        case SN_SYS_SERIAL_COMM_TX_CARRIAGE_RETURN:
            error = write(serialId->uartId, CARRIAGE_RETURN, RETURN_SIZE);
            break;
        case SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN:
            error = write(serialId->uartId, NEW_LINE_RETURN, RETURN_SIZE);
            break;
        case SN_SYS_SERIAL_COMM_TX_NX_RETURN:
            error = write(serialId->uartId, nxReturn, RETURN_SIZE * 3);
            break;
        default:
            break;
        }

        if(error < 0)
        {
            //TX ERROR
        }
    }

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  SERIAL CONTROL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
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
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeou

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

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  RX HANDLER
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
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

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  HANDLER
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
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

            SN_SYS_TIMER_Delay(100);
        }
    }
    else
    {
        printf("error read UART\n"); fflush(stdout);
    }

    return SN_STATUS_OK;
}
