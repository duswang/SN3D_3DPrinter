/**
 * @file SN_MODULE_IMAGE_VIEWER.c
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @bug  SDL :: 2048 x 2048 Over Resolution it can't make Window(surface)
 *
 * @todo 8 bit png image file support.
 *
 * @note Image Resize not supported. MUST BE image resolution is same with display resolution.
 */

#include "SN_API.h"
#include "SN_MODULE_IMAGE_VIEWER.h"

#if(IMAGE_VIEWER_USE_SDL)
#include "IMAGE_VIEWER_SDL.h"
#else
#include "IMAGE_VIEWER_FB.h"
#endif

/* ******* STATIC DEFINE ******* */

/* *** MODULE *** */


/* ******* SYSTEM DEFINE ******* */
/* *** MODULE HANDLER  *** */
static moduleImageViewer_t moduleImageViewer;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
#if(!IMAGE_VIEWER_OFF)
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = ImageViewer_Init(&moduleImageViewer);
    SN_SYS_ERROR_CHECK(retStatus, "Image Viewer Init Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = ImageViewer_WindowUpdate(&moduleImageViewer, sliceIndex);
    SN_SYS_ERROR_CHECK(retStatus, "Window Update Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowClean(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = ImageViewer_WindowClean(&moduleImageViewer);
    SN_SYS_ERROR_CHECK(retStatus, "Window Clean Failed.");

    return retStatus;
}


SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = ImageViewer_WindowDestroy(&moduleImageViewer);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Thumbnail Clean Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailClean(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = ImageViewer_ThumbnailClean(&moduleImageViewer);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Thumbnail Clean Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailDistroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}
#else
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowClear(void)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailUpdate(void)
{
    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailClean(void)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_ThumbnailDistroy(void)
{
    return SN_STATUS_OK;
}
#endif
