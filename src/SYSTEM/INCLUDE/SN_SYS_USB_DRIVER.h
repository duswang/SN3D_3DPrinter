/**
 * @file SN_SYS_USB_DRIVER.h
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup SYSTEM_USB_DRIVER USB Driver
 * @ingroup SYSTEM
 * @brief System USB Driver Functions.
 * @{
 */

#ifndef SN_SYS_USB_DRIVER_H_
#define SN_SYS_USB_DRIVER_H_

/** @name USB Event Macro
 *
 *////@{
enum usb_driver_event
{
    MSG_USB_EVT_MOUNT       = 0x01,
    MSG_USB_EVT_UNMOUNT     = 0x02,
    MSG_USB_EVT_WAITING     = 0x03,
    MSG_SB_EVT_NONE         = 0x04
} usbEvt_t;
///@}

/** @name USB Driver Structure
 *
 *////@{
struct usb_dev_handle {
  int fd;

  struct usb_bus *bus;
  struct usb_device *device;

  int config;
  int interface;
  int altsetting;

  /* Added by RMT so implementations can store other per-open-device data */
  void *impl_info;
};
///@}

/** @name USB Driver System
 *  @brief Description of Serial System Init and Uninit funtions.
 *////@{

/** @brief
 *
 *  @param pfCallBack
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_USBDriverInit(void* (pfCallBack)(int evt));


/** @brief
 *
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS SN_SYS_USBDriverTerminate(void);

///@}

/** @name USB Driver System :: Util
 *  @brief Description of Serial System Init and Uninit funtions.
 *////@{
/** @brief
 *
 *
 *  @return isMount
 *  @note
 */

extern bool SN_SYS_USBDriverIsMount(void);
///@}


#endif /* SN_SYS_USB_DRIVER_H_ */
/**@}*/
