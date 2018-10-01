/*
 * MODUEL_FILE_SYSTEM.c
 *
 *  Created on: Sep 24, 2018
 *      Author: bato
 */

#include "SN_API.h"
#include "SN_MODUEL_FILE_SYSTEM.h"

pthread_mutex_t ptmFileSystem = PTHREAD_MUTEX_INITIALIZER;
pthread_t        ptFileSystem;
/** static functions **/
static void* sFileSystemThread();

#define SN_MODUEL_FILE_SYSTEM_QKEY      IPC_PRIVATE
#define SN_MODUEL_FILE_SYSTEM_MSG_TYEP  EVT_ID_MODUEL_FILE_SYSTEM

sysMessageQId msgQIdFileSystem;

//@DEBUG
int sPrintdev(libusb_device *dev);

/** static functions **/
static void sFileSystemMessagePut(event_id evtId, uint32_t value);
static void* sFileSystemThread();

static void sFileSystemMessagePut(event_id evtId, uint32_t value)
{
    SN_SYS_MessagePut(&msgQIdFileSystem, evtId, value);
}

static void* sFileSystemThread()
{
    general_evt_t evt;
    int i = 0;

    while(true)
    {
        evt = SN_SYS_MessageGet(&msgQIdFileSystem);

        switch(evt.evt_id)
        {
            case MSG_FILE_SYSTEM_DETECTED:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, MSG_VALUE_FILE_SYSTEM_NONE);
                printf("File System Detected. :: %d %d\n", evt.evt_id, evt.value);
                break;

            case MSG_FILE_SYSTEM_READ:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_UPDATE, MSG_VALUE_FILE_SYSTEM_NONE);
                printf("File System Read. :: %d %d\n", evt.evt_id, evt.value);
                break;

            case MSG_FILE_SYSTEM_UPDATE:
                sFileSystemMessagePut(MSG_FILE_SYSTEM_WAITING, MSG_VALUE_FILE_SYSTEM_NONE);
                SN_SYSTEM_SendApplMessage(0,0);
                printf("File System Update. :: %d %d\n", evt.evt_id, evt.value);
                break;

            case MSG_FILE_SYSTEM_WAITING:
                printf("File System Waiting. :: %d %d\n", evt.evt_id, evt.value);
                break;

            default:
                break;
        }

        //SN_SYS_MessagePut(&msgQIdFileSystem, i, i);

        i++;
    }

    return 0;
}

//@DEBUG
int sPrintdev(libusb_device *dev)
{
    struct libusb_device_descriptor desc;

    struct libusb_config_descriptor *config;

    const struct libusb_interface *inter;
    const struct libusb_interface_descriptor *interdesc;
    const struct libusb_endpoint_descriptor *epdesc;

    int i = 0, j = 0, k = 0;

    int retValue = libusb_get_device_descriptor(dev, &desc);

    if(retValue < 0)
    {
        return retValue;
    }

    printf("Number of possible configurations : %d \n", desc.bNumConfigurations);
    fflush(stdout);
    printf("Device Class : %d \n", desc.bDeviceClass);
    fflush(stdout);
    printf("VendorID : %d \n", desc.idVendor);
    fflush(stdout);
    printf("ProductID : %d \n", desc.idProduct);
    fflush(stdout);

    libusb_get_config_descriptor(dev, 0, &config);
    printf("Interfaces : %d \n", config->bNumInterfaces);

    for(i = 0; i < ((int)(config->bNumInterfaces)); i++)
    {
        inter = &config->interface[i];

        for(j = 0; j <  ((int)inter->num_altsetting); j++)
        {
            interdesc    = &inter->altsetting[j];

            printf("Interface Number : %d \n", interdesc->bInterfaceNumber);
            fflush(stdout);
            printf("Number of endpoints : %d \n", interdesc->bNumEndpoints);
            fflush(stdout);
            /*
            for(k = 0; k < ((int)(interdesc->bNumEndpoints)); k++)
            {
                printf("Descriptor Type : %d \n", epdesc->bDescriptorType);
                fflush(stdout);
                printf("EP Address : %d \n", epdesc->bEndpointAddress);
                fflush(stdout);
            }
            */
        }
    }

    libusb_free_config_descriptor(config);

    return retValue;
}

int SN_MODUEL_FILE_SYSTEM_Init(void)
{
    libusb_device **devs;
    libusb_device_handle *dev_handle;

    libusb_context *ctx = NULL;
    ssize_t cnt;

    pthread_t pt3DPrinter;
    int retValue = 0, i = 0;

    retValue = libusb_init(&ctx);

    if(retValue < 0)
    {
        return retValue;
    }

    cnt = libusb_get_device_list(ctx, &devs);

    if(retValue < 0)
    {
        return retValue;
    }


    /** MESSAGE Q INIT **/
    SN_SYS_MessageInit(&msgQIdFileSystem, SN_MODUEL_FILE_SYSTEM_QKEY, SN_MODUEL_FILE_SYSTEM_MSG_TYEP);

    sFileSystemMessagePut(MSG_FILE_SYSTEM_READ, MSG_VALUE_FILE_SYSTEM_NONE);

    //@DEBUG
    /*
    for(i = 0; i < cnt; i++)
    {
        printf("\n\n- - - - - - - - - - - - - - - \n- - - - Device Info %d - - - - \n- - - - - - - - - - - - - - - \n\n", i);
        fflush(stdout);
        sPrintdev(devs[i])   ; //print specs of this device
        printf("\n\n- - - - - - - - - - - - - - - \n\n- - - - - - - - - - - - - - - \n\n");
        fflush(stdout);
    }
    */
    if (pthread_mutex_init(&ptmFileSystem, NULL) != 0)
    {
        printf("\n mutex init failed\n");
    }

    if((retValue = pthread_create(&ptFileSystem, NULL, sFileSystemThread, NULL)))
    {
        printf("Thread Creation Fail %d\n", retValue);
        fflush(stdout);
    }
    fflush(stdout);

    return retValue;
}

void SN_MODUEL_FILE_SYSTEM_Uninit(void)
{

}


