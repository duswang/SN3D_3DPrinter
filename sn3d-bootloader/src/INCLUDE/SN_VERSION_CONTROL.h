/**
 * @file SN_VERSION_CONTROL.h
 * @author Bato
 * @date 21 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup VERSION_CONTROL Version Control
 * @ingroup BOOTLOADER_MODULE
 * @brief Version Control Functions.
 * @{
 */

#ifndef SN_VERSION_CONTROL
#define SN_VERSION_CONTROL

/*************************************************************
 * @name Version Control
 *  Description of 3D Printr Module Init and Uninit funtions.
 *////@{
/** @brief
 *
 *  @param filename - Firmware fileName.
 *  @return isNeedReboot - When need reboot it return 'true'
 *
 *  @note
 */
extern bool SN_VERSION_CONTROL_VersionUpdate(const char* fileName);

/*************************************************************@}*/

#endif /* SN_VERSION_CONTROL_H_ */
