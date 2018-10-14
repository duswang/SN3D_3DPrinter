/*
 * SN_SYS_USB_DRIVER.h
 *
 *  Created on: Sep 27, 2018
 *      Author: pi
 */

#ifndef SN_SYS_USB_DRIVER_H_
#define SN_SYS_USB_DRIVER_H_

/** Def Config **/
#define VERDOR_ID  1921
#define PRODUCT_ID 21808

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

extern int SN_SYS_USBDriverInit(void* (pfCallBack)(int evt));
extern int SN_SYS_USBDriverTerminate(void);
extern int SN_SYS_USBDriverIsMount(void);

#define USB_EVT_MOUNT   0x01
#define USB_EVT_UNMOUNT 0x02
#define USB_EVT_WAITING 0x03
#define USB_EVT_NONE    0x04

#define USB_UNMOUNT     0
#define USB_MOUNT       1

#endif /* SN_SYS_USB_DRIVER_H_ */
