/*
 * SN_SYS_USB_DRIVER.c
 *
 *  Created on: Sep 18, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_SYS_USB_DRIVER.h"

/** System **/
/* Thread */
pthread_mutex_t ptmUSBDriver = PTHREAD_MUTEX_INITIALIZER;
pthread_t       ptUSBDriverl;

/** Global Variables **/
/* libusb */
static int IsClaimed;

static libusb_device        *device;
static libusb_device_handle *device_handle;
static libusb_hotplug_callback_handle handle;

static int isMount = USB_UNMOUNT;
static void* (*USBEvent_callBack)(int);

/** Static Functions **/
static void* sUSBDriverThread();

static int sUSBDriverClaimInterface(void);
static int sUSBDriverReleaseInterface(void);

static int LIBUSB_CALL hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data);

int SN_SYS_USBDriverInit(void* (pfCallBack)(int evt))
{
    int r = 0;

    r = libusb_init(NULL);

    if (pthread_mutex_init(&ptmUSBDriver, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if((r = pthread_create(&ptUSBDriverl, NULL, sUSBDriverThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", r);
        fflush(stdout);
    }

    if (LIBUSB_SUCCESS != r) {
      printf("Error creating a hotplug callback\n");
      libusb_exit(NULL);
      return EXIT_FAILURE;
    }

    if(pfCallBack != NULL)
    {
        USBEvent_callBack = pfCallBack;
    }
    else
    {
        return -1;
    }

    /** Open Device **/
    device_handle = libusb_open_device_with_vid_pid( 0, VERDOR_ID, PRODUCT_ID );

    if( device_handle != NULL )
    {
        r = sUSBDriverClaimInterface();

        USBEvent_callBack(USB_EVT_MOUNT);
    }
    else
    {
        USBEvent_callBack(USB_EVT_WAITING);
    }

    r = libusb_hotplug_register_callback(NULL, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                          LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, 0, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY,
                                          LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL,
                                          &handle);

    return r;
}

int SN_SYS_USBDriverTerminate(void)
{
    sUSBDriverReleaseInterface();
    libusb_hotplug_deregister_callback(NULL, handle);
    libusb_close( device_handle );
    libusb_exit(NULL);

    return 0;
}

int SN_SYS_USBDriverIsMount(void)
{
    return isMount;
}

static int sUSBDriverClaimInterface(void)
{
    int r = 0;

    if( device_handle != NULL )
    {
        device = libusb_get_device(device_handle);
        if( libusb_kernel_driver_active(device_handle,0))
        {
            r = libusb_detach_kernel_driver(device_handle,0);
            r = libusb_claim_interface( device_handle, 0);
            IsClaimed = 1;
        }
    }

    return r;
}

static int sUSBDriverReleaseInterface(void)
{
    int r = 0;

    if(IsClaimed)
    {
        r = libusb_release_interface( device_handle, 0);
    }

    return r;
}

static int LIBUSB_CALL hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
  static libusb_device_handle *handle = NULL;
  struct libusb_device_descriptor desc;
  int rc;

  (void)libusb_get_device_descriptor(dev, &desc);

  if(SN_SYS_USBDriverIsMount() != USB_MOUNT)
  {
      if(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event)
      {
          rc = libusb_open(dev, &handle);
          isMount = USB_MOUNT;
          USBEvent_callBack(USB_EVT_MOUNT);
          if(LIBUSB_SUCCESS != rc)
          {
              printf("Could not open USB device\n");
          }
      }
      else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event)
      {
          if (handle)
          {
              isMount = USB_UNMOUNT;
              USBEvent_callBack(USB_EVT_UNMOUNT);
              libusb_close(handle);
              handle = NULL;
          }
      }
      else
      {
          printf("Unhandled event %d\n", event); fflush(stdout);
      }
  }
  return 0;
}

static void* sUSBDriverThread()
{
    while (true)
    {
      libusb_handle_events_completed(NULL, NULL);
      usleep(10000);
    }
    return 0;
}
