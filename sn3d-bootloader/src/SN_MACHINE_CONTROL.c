/**
 * @file SN_MACHINE_CONTROL.c
 * @author Bato
 * @date 18 Sep 2018
 * @brief
 *
 * @see https://bitbucket.org/xengiennering/sn3d-project/src/master/
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_BOOTLOADER.h"
#include "SN_MACHINE_CONTROL.h"

#ifdef __APPLE__
#include <png.h>
#include <zlib.h>
#endif

#ifdef linux
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <png.h>
#include <zlib.h>
#endif

/* ******* STATIC DEFINE ******* */
typedef struct frameBuffer_Window
{
    const char* name;

    int            w;
    int            h;
    int          bpp;

    long  screenSize;
} FB_Window_t;

#define WINDOW_NAME "/dev/fb0"

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
static bool sMachineInfoCompare(void);

static void sUpdateBootConfig(float inch);
static FB_Window_t* sScreenSizeCheck(void);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
bool SN_VERSION_CONTROL_MachineInfoUpdate(const char* fileName)
{
    /* Always Update Machine Info File. */
    SN_SYS_Log(" * Machiine Info XML File Loading *\n");
    FileSystem_fctl_CopyFile(TEMP_FIRMWARE_MACHINE_INFO_PATH, FIRMWARE_MACHINE_INFO_PATH);

    SN_SYS_Log("\n * Machiine Info XML File Copy to SN3D Service *\n");
    /* Update to SN3D Service */
    FileSystem_fctl_RemoveFiles(SN3D_MACHINE_CONFIG_FOLDER_PATH);
    FileSystem_fctl_CopyFile(FIRMWARE_MACHINE_INFO_PATH, SN3D_MACHINE_CONFIG_PATH);


    return sMachineInfoCompare();
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Static Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static FB_Window_t* sScreenSizeCheck(void)
{
    FB_Window_t* window = malloc(sizeof(FB_Window_t));
    if(window == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "cannot allocate memory a var 'window'");
    }

    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    fbfd = open(WINDOW_NAME, O_RDWR);
    if (fbfd == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "cannot open framebuffer device.");
    }

    /* GET FIX SCREEN INFO */
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Error reading fixed information.");
    }

    /* GET VARIABLE SCREEN INFO */
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Error reading variable information.");
    }

    window->h   = vinfo.yres;
    window->w   = vinfo.xres;
    window->bpp = vinfo.bits_per_pixel;

    window->screenSize = (window->w * window->h) * (window->bpp / 8);

    printf("\n * Rasbperry Pi Screen Resolution [ %d x %d ] %dbpp * \n", window->w, window->h, window->bpp);

    close(fbfd);

    return window;
}

static bool sMachineInfoCompare(void)
{
    bool isNeedUpdate = false;
    machineInfo_t* currentMachineInfo = NULL;

    FB_Window_t* currentDisplay = NULL;

    currentMachineInfo = FileSystem_machineInfoXMLLoad(FIRMWARE_MACHINE_INFO_PATH);

    currentDisplay = sScreenSizeCheck();

    printf("\n\n * Current Machine Info * \n");
    printf(" Name       : [ %s ] \n", currentMachineInfo->name);
    printf(" Resolution : [ %ld x %ld px ] \n", currentMachineInfo->screenWidth, currentMachineInfo->screenHeight);
    printf(" Display    : [ %.1f inch ] \n", currentMachineInfo->inch);
    printf(" Z Limit    : [ %ld mm ] \n\n", currentMachineInfo->machineHeight);

    if((currentDisplay->w != currentMachineInfo->screenWidth) || (currentDisplay->h != currentMachineInfo->screenHeight))
    {
        isNeedUpdate = true;
        sUpdateBootConfig(currentMachineInfo->inch);
        FileSystem_fctl_CopyFile(TEMP_FIRMWARE_MACHINE_INFO_PATH, FIRMWARE_MACHINE_INFO_PATH);
    }
    else
    {
        SN_SYS_Log(" => Don't need to update machine./b b information. <= \n\n\n");
    }

    free(currentDisplay);

    return isNeedUpdate;
}

static void sUpdateBootConfig(float inch)
{
    SN_SYS_Log(" *\n Updating Machine... *");

    if(inch == (float)5.5)
    {
        SN_SYS_Log("\n 5.5 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_5_5_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)8.9)
    {
        SN_SYS_Log("\n 8.9 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_8_9_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)15.6)
    {
        SN_SYS_Log("\n 15.6 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_15_6_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else if(inch == (float)23.8)
    {
        SN_SYS_Log("\n 23.8 Inch Updating...\n\n");
        FileSystem_fctl_CopyFile(MACHINE_23_8_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
    else
    {
        SN_SYS_Log("\n Default :: 5.5 Inch.\n\n");
        FileSystem_fctl_CopyFile(MACHINE_DEFAULT_INCH_PATH, MACHINE_BOOTCONFIG_PATH);
    }
}
