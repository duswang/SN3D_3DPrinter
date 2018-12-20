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

#define DEFAULT_WINDOW_WIDTH  1440
#define DEFAULT_WINDOW_HEIGHT 2560

#define WINDOW_NAME "/dev/fb0"

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

FB_Window_t* SN_MACHINE_CONTROL_DisplayCheck(void)
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

    printf("\n\n Rasbperry Pi Screen Resolution [ %d x %d ] %dbpp\n", window->w, window->h, window->bpp);

    close(fbfd);

    return window;
}

