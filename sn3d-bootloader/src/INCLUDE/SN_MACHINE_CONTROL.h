/**
 * @file SN_MACHINE_CONTROL.h
 * @author Bato
 * @date 21 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup MACHINE_CONTROL Machine Control
 * @ingroup BOOTLOADER_MODULE
 * @brief Machine Control Functions.
 * @{
 */

#ifndef SN_MACHINE_CONTROL
#define SN_MACHINE_CONTROL

/*************************************************************
 * @name Machine Control
 *  Description of 3D Printr Module Init and Uninit funtions.
 *////@{
/** @brief
 *
 *  @param filename - Firmware fileName.
 *  @return isNeedReboot - When need reboot it return 'true'
 *
 *  @note Machine info always update(copy & pasted) bootconfig when calling this function.
 */
extern bool SN_VERSION_CONTROL_MachineInfoUpdate(const char* fileName);

/*************************************************************@}*/

#endif /* SN_MACHINE_CONTROL */
/**@}*/
