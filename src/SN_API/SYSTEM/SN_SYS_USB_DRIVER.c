/**
 * @file SN_SYS_USB_DRIVER.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @bug some USB is not working. need more test and debugging.
 */

#ifdef __APPLE__
#include <libusb-1.0/libusb.h>
#endif

#ifdef linux
#include <libusb-1.0/libusb.h>
#endif

#include "SN_API.h"
#include "SN_SYS_USB_DRIVER.h"

/* ******* STATIC DEFINE ******* */
#define USB_UNMOUNT     0
#define USB_MOUNT       1

/* ******* SYSTEM DEFINE ******* */
/* ** SYSTEM THREAD ** */
static pthread_mutex_t ptmUSBDriver = PTHREAD_MUTEX_INITIALIZER;
static pthread_t       ptUSBDriverl;

/* ******* GLOBAL VARIABLE ******* */
static libusb_hotplug_callback_handle handle;

static bool isMount = USB_UNMOUNT;
static void* (*USBEvent_callBack)(int);

/* ******* STATIC FUNCTIONS ******* */
/* *** SYSTEM *** */
static void* sUSBDriverThread();

/* *** CALL BACK *** */
static ERROR_T LIBUSB_CALL hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data);

SN_STATUS SN_SYS_USBDriverInit(void* (pfCallBack)(int evt))
{
    SN_STATUS retStatus = SN_STATUS_OK;
    ERROR_T  error = 0;

    if(pfCallBack != NULL)
    {
        USBEvent_callBack = pfCallBack;
    }
    else
    {
        return -1;
    }

    error = libusb_init(NULL);

    if (LIBUSB_SUCCESS != error)
    {
        libusb_exit(NULL);
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "libusb init failed");
    }

    if (pthread_mutex_init(&ptmUSBDriver, NULL) != 0)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "USB Driver Mutext Init Failed.");
    }

    if(pthread_create(&ptUSBDriverl, NULL, sUSBDriverThread, NULL))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "USB Driver Thread Init Failed.");
    }

    error = libusb_hotplug_register_callback(NULL, \
                                               LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, \
                                               0, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               hotplug_callback, \
                                               NULL,
                                               &handle);

    error = libusb_hotplug_register_callback(NULL, \
                                               LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, \
                                               0, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               LIBUSB_HOTPLUG_MATCH_ANY, \
                                               hotplug_callback, \
                                               NULL,
                                               &handle);

    if (LIBUSB_SUCCESS != error)
    {
        libusb_exit(NULL);
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Error creating a hotplug callback");
    }

    return retStatus;
}

SN_STATUS SN_SYS_USBDriverTerminate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    libusb_hotplug_deregister_callback(NULL, handle);
    libusb_exit(NULL);

    return retStatus;
}

bool SN_SYS_USBDriverIsMount(void)
{
    return isMount;
}

static ERROR_T LIBUSB_CALL hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev, libusb_hotplug_event event, void *user_data)
{
    ERROR_T rc;

    struct libusb_device_descriptor desc;


    (void)libusb_get_device_descriptor(dev, &desc);

    if(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event)
    {
        isMount = USB_MOUNT;
        USBEvent_callBack(MSG_USB_EVT_MOUNT);
    }
    else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event)
    {
        isMount = USB_UNMOUNT;
        USBEvent_callBack(MSG_USB_EVT_UNMOUNT);
    }
    else
    {
        SN_SYS_Log("USB Driver => System => Unhandled event.");
    }

    return rc = LIBUSB_SUCCESS;
}

static void* sUSBDriverThread()
{
    while (true)
    {
        libusb_handle_events_completed(NULL, NULL);
        SN_SYS_Delay(100);
    }
    return NULL;
}
