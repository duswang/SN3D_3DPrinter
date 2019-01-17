/**
 * @file SN_MODULE_3D_PRINTER.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup MODULE_3D_PRINTER 3D Printer
 * @ingroup MODULE
 * @brief 3D Printer Module Functions.
 * @{
 */

#ifndef SN_MODUEL_3D_PRINTER
#define SN_MODUEL_3D_PRINTER

/*************************************************************
 * @name 3D Printr Module
 *  Description of 3D Printr Module Init and Uninit funtions.
 *////@{
/** @brief 3D Printer Initiation
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Init(void);

/** @brief 3D Printer Uninitiation
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Uninit(void);

/*************************************************************@}*/

/*************************************************************
 * @name 3D Printer Module :: Printing.
 *  Description of 3D Printr Module funtions.
 *////@{

/** @brief 3D Printer start printing
 *
 *  @param pageIndex - Page index of number
 *  @param itemIndex - Item index of number
 *  @param optionIndex - Option index of number
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex, uint32_t option);


/** @brief 3D Printer All stop
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Stop(void);


/** @brief 3D Printer Pause
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Pause(void);


/** @brief 3D Printer Resume
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Resume(void);

/*************************************************************@}*/

/*************************************************************
 * @name 3D Printer Module :: Z Control.
 *  Description of 3D Printr Module funtions.
 *////@{


/** @brief Motor Initialization
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_MotorInit(void);


/** @brief Motor Uninitialization
 *
 *  @return SN_STATUS
 *
 *  @note Now Doing Noting.
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_MotorUninit(void);


/** @brief Motor Z Axis Homing
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_Homing(void);


/** @brief Motor Z Move.
 *
 *  @param mm - mm
 *
 *  @return SN_STATUS
 *
 *  @note Z Limit variable get from 'machieninfo.xml' file
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_Move(float mm);

/** @brief Get Z Position.
 *
 *
 *  @return mm - mm
 *
 */
extern float SN_MODULE_3D_PRINTER_Z_Position(void);

/** @brief Check Motor State
 *
 *  @return IsMotorBusy?s
 *
 *  @note
 */
extern bool SN_MODULE_3D_PRINTER_IsMotorBusy(void);

/** @brief Check Motor State
 *
 *  @return IsMotorBusy?s
 *
 *  @note
 */
extern bool SN_MODULE_3D_PRINTER_IsPrinting(void);

/** @brief Check Motor State
 *
 *  @return IsMotorBusy?s
 *
 *  @note
 */
extern bool SN_MODULE_3D_PRINTER_IsPause(void);


/*************************************************************@}*/

#endif /* SN_MODULE_3D_PRINTER */
/**@}*/
