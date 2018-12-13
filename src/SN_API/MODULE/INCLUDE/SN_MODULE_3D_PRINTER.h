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
/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Init(void);

/** @brief
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

/** @brief
 *
 *  @param pageIndex
 *  @param itemIndex
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Start(uint32_t pageIndex, uint32_t itemIndex, uint32_t option);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Stop(void);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Pause(void);


/** @brief
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


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_MotorInit(void);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_MotorUninit(void);


/** @brief
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_Homing(void);


/** @brief
 *
 *  @param mm
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS SN_MODULE_3D_PRINTER_Z_Move(float mm);

/** @brief
 *
 *  @param mm
 *
 *  @return SN_STATUS
 *
 *  @note
 */
extern bool SN_MODULE_3D_PRINTER_IsMotorBusy(void);

/*************************************************************@}*/

#endif /* SN_MODULE_3D_PRINTER */
/**@}*/
