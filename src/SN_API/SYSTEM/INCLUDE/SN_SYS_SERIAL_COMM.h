/**
 * @file SN_SYS_SERIAL_COMM.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_SERIAL_COMM Serial
 * @ingroup SYSTEM
 * @brief Serial System Functions.
 * @{
 */

#ifndef SN_SYS_SERIAL_COMM
#define SN_SYS_SERIAL_COMM

/*************************************************************
 * @name Serial Return Macro
 *
 *////@{

#define CARRIAGE_RETURN  "\r"
#define NEW_LINE_RETURN  "\n"
#define NX_RETURN        { 0xFF, 0XFF, 0xFF }

#define RETURN_SIZE      1
#define NX_RETURN_SIZE   5
/*************************************************************@}*/

/*************************************************************
 * @name Serial Config
 *
 *////@{
#define MAX_NUM_OF_SERIAL 2 /**< Max Serial Terminal */

#define SN_SYS_SERIAL_COMM_BUFFER_SIZE 255
#define SN_SYS_SERIAL_COMM_INVAILD_UART_ID (-1)

/*************************************************************@}*/

/*************************************************************
 * @name Serial Structure
 *
 *////@{
typedef struct sys_serial_stat {
  int buffer_length;
  int rx_length;
  int endCodeChk;
  int mode;
} sysSerialStat_t;

typedef struct sys_serial_def {
    const char* device;
    int oflags;
    int baudRate;
    int rxByteSize;
    int returnMode;
    char*  buffer;
} sysSerialDef_t;

typedef struct sys_serial_id {
    const sysSerialDef_t* _serialDef;
    sysSerialStat_t state;
    void* (*pfSerialCallBack)(char* rxBuffer);
    uint32_t    uartId;
} *sysSerialId;

typedef struct sys_serial_q {
    sysSerialId _serialId;
    bool isAvailable;
} sysSerialQ;

/*************************************************************@}*/

/*************************************************************
 * @name Serial Macro Function
 *
 *////@{

/** @brief def Serial
 *  def serial for get SerialId
 *
 *  @param name
 *  @param device
 *  @param oflags
 *  @param baudRate
 *  @param rxByteSize
 *  @param rxReturnMode
 *
 *  @note
 */
#define sysSerialDef(name, device, oflags, baudRate, rxByteSize, rxReturnMode)   \
    static char buffer##name[SN_SYS_SERIAL_COMM_BUFFER_SIZE]; \
    static const sysSerialDef_t sys_serial_def_##name = \
    { (device), (oflags), (baudRate), (rxByteSize), (rxReturnMode),buffer##name }

#define sysSerialThreadFuncdef(num) \
    static void* sSerialThread_##num() \
    { \
        while(true) \
        { \
            pthread_mutex_lock(&ptmSerial[(num)]);\
            if(aSerial[(num)].isAvailable) sSerial_RX_Hdlr_r(aSerial[(num)]._serialId); \
            pthread_mutex_unlock(&ptmSerial[(num)]); \
        } \
        return NULL; \
    }

#define sysSerial(name)  \
    (&sys_serial_def_##name)

#define sysSerialThreadFunc(num) \
    (sSerialThread_##num)

/*************************************************************@}*/



/*************************************************************
 * @name Serial Baud Rate Paramter Macro
 *
 *////@{
#define SN_SYS_SERIAL_COMM_BAUD_RATE_9600     B9600 /**< Baud Rate 9600    */
#define SN_SYS_SERIAL_COMM_BAUD_RATE_19200   B19200 /**< Baud Rate 19200   */
#define SN_SYS_SERIAL_COMM_BAUD_RATE_115200 B115200 /**< Baud Rate 115200  */
///@}

/*************************************************************
 * @name Serial Send Parameter Macro
 *
 *////@{
#define SN_SYS_SERIAL_COMM_RX_REALTIME   0   /**< RX real time    */
#define SN_SYS_SERIAL_COMM_RX_BYTE_4     4   /**< RX read  4 byte */
#define SN_SYS_SERIAL_COMM_RX_BYTE_8     8   /**< RX read  8 byte */
#define SN_SYS_SERIAL_COMM_RX_BYTE_16   16   /**< RX read 16 byte */
#define SN_SYS_SERIAL_COMM_RX_BYTE_32   32   /**< RX read 32 byte */
///@}

/*************************************************************
 * @name Serial Return Parameter Macro
 *
 *////@{
#define SN_SYS_SERIAL_COMM_TX_RETURN          0 /**< Send with no return  */
#define SN_SYS_SERIAL_COMM_TX_CARRIAGE_RETURN 1 /**< Send with carriage return    */
#define SN_SYS_SERIAL_COMM_TX_NEW_LINE_RETURN 2 /**< Send with new line return    */
#define SN_SYS_SERIAL_COMM_TX_NX_RETURN       3 /**< Send with Nextion endcode return    */
///@}

/**
 * @name Serial System
 *  @brief Description of Serial System Init and Uninit funtions.
 *////@{

/** @brief
 *
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS   SN_SYS_SerialInit (void);


/** @brief
 *
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_SerialUninit(void);

/*************************************************************@}*/

/*************************************************************
 * @name Serial System :: Create & Remove
 *  @brief
 *////@{

/** @brief
 *
 *  @param serialDef
 *  @param pfCallback
 *
 *  @return SN_STATUS
 *  @note
 */
extern sysSerialId SN_SYS_SerialCreate(const sysSerialDef_t* serialDef, void* (*pfCallBack)(char*));


/** @brief
 *
 *  @param serialId
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS   SN_SYS_SerialRemove(sysSerialId serialId);

/*************************************************************@}*/

/*************************************************************
 * @name Serial System :: Tx & Rx
 *  @brief
 *  @note RX is Callback, you create 'SN_SYS_SerialCreate' you can get rxBuffer from pfCallback.
 *////@{

/** @brief
 *
 *  @param serialId
 *  @param buffer
 *  @param bufferSize
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS  SN_SYS_SerialTx(sysSerialId serialId, char* buffer, size_t bufferSize);

/*************************************************************@}*/


#endif /* SN_SYS_SERIAL_COMM */
/**@}*/
