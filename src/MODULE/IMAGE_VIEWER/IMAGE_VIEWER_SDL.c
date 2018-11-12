#if(IMAGE_VIEWER_USE_SDL)
/**
 * @file IMAGE_VIEWER_SDL.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 */

#include "SN_API.h"
#include "SN_MODULE_IMAGE_VIEWER.h"

#include "IMAGE_VIEWER_SDL.h"

/* Library */
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif

#ifdef linux
#include <SDL.h>
#include <SDL_image.h>
#endif

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** IMAGE *** */
static SDL_Texture* sLoadTexture(const char* filename, SDL_Renderer *renderer);
/* *** ERROR ****/
static void sCheckError_SDL(const char* message, const char* _file, const char* _func, const int _line);
static void sCheckError_SDL_Image(const char* message, const char* _file, const char* _func, const int _line);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

SN_STATUS ImageVIewer_Init(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

        /* GET MACHINE INFO */
        moduleImageViewer->machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
        if(!moduleImageViewer->machineInfo.isInit)
        {
            return SN_STATUS_NOT_INITIALIZED;
        }

        SN_SYS_Log("MODULE INIT => IMAGE VIEWER.");

        /* SDL INIT */
        if(SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            SN_SYS_ERROR_CHECK_SDL("Unable to initialize SDL");
        }

        /* GET DISPLAY INFO */
        SDL_GetDesktopDisplayMode(0, &moduleImageViewer->dm);

        printf("Image Viwer => Module => Display Resolution [ %d x %d ] %dHz %d\n", moduleImageViewer->dm.w, moduleImageViewer->dm.h, \
                                                                                     moduleImageViewer->dm.refresh_rate, \
                                                                                     moduleImageViewer->dm.format);

        /* IMAGE INIT */
        int flags=IMG_INIT_JPG | IMG_INIT_PNG;
        int initted = IMG_Init(flags);

        if((initted & flags) != flags)
        {
            SN_SYS_ERROR_CHECK_SDL("Unable to initialize SDL_image");
        }

        ImageViewer_WindowClean(moduleImageViewer);

        return retStatus;
}

SN_STATUS ImageVIewer_WindowUpdate(moduleImageViewer_t* moduleImageViewer, uint32_t slice)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    ERROR_T error = 0;

    char path[MAX_PATH_LENGTH] = {'\0', };
    int image_w = 0, image_h = 0;

    /* GET PRINT TARGET INFO */
    printInfo_t printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
    if(!printInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load Texture */
    sprintf(path,"%s%s%04d.png", printInfo.printTarget.targetPath, printInfo.printTarget.targetName, sliceIndex);

    moduleImageViewer->texture = sLoadTexture(path, moduleImageViewer->renderer);
    error += SDL_QueryTexture(moduleImageViewer->texture, NULL, NULL, &image_w, &image_h);

    moduleImageViewer->dest_rect.x = 0;
    moduleImageViewer->dest_rect.y = 0;
    moduleImageViewer->dest_rect.w = moduleImageViewer->dm.w;
    moduleImageViewer->dest_rect.h = moduleImageViewer->dm.h;


    /* Drawing Image */
    error += SDL_RenderClear(moduleImageViewer->renderer);
    error += SDL_RenderCopy(moduleImageViewer->renderer, moduleImageViewer->texture, NULL, &moduleImageViewer->dest_rect);
    if(!error)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Get error from renderer");
    }

    SDL_RenderPresent(moduleImageViewer->renderer);

    SDL_DestroyTexture(moduleImageViewer->texture);

    return retStatus;
}

SN_STATUS ImageViewer_WindowClean(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    ERROR_T error = 0;

    /* Clear */
    error += SDL_RenderClear(moduleImageViewer->renderer);
    error += SDL_SetRenderDrawColor(moduleImageViewer->renderer, 0, 0, 0, 255);
    if(!error)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Get error from renderer");
    }

    SDL_RenderPresent(moduleImageViewer->renderer);

    return retStatus;
}
SN_STATUS ImageViewer_WindowDestroy(moduleImageViewer_t* moduleImageViewer)
{
    SDL_DestroyTexture(moduleImageViewer->texture);
    IMG_Quit();
    SDL_DestroyRenderer(moduleImageViewer->renderer);
    SDL_DestroyWindow(moduleImageViewer->window);
    SDL_Quit();

    return SN_STATUS_OK;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  IMAGE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SDL_Texture* sLoadTexture(const char* filename, SDL_Renderer *renderer)
{
    if(filename == NULL || renderer == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Invalid Paramter in Load Texture.");
    }

    /* INIT IMAGE */
    SDL_Surface *image = IMG_Load(filename);
    if(image == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Unable to load image");
    }

    /* INIT TEXTURE */
    SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
    if(image == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Unable to create a texture from the image");
    }

    SDL_FreeSurface(image);

    return img_texture;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  ERROR
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
static void sCheckError_SDL(const char* message, const char* _file, const char* _func, const int _line)
{
    const char *errorMessage = SDL_GetError();

    if (*errorMessage)
    {
        SDL_ClearError();
        SDL_Quit();

        SN_SYS_ErrorCheck(SN_STATUS_SDL_ERROR, errorMessage, _file, _func, _line);
    }
}

static void sCheckError_SDL_Image(const char* message, const char* _file, const char* _func, const int _line)
{
    const char *errorMessage = SDL_GetError();

    if(*errorMessage)
    {
        SDL_ClearError();
        IMG_Quit();
        SDL_Quit();

        SN_SYS_ErrorCheck(SN_STATUS_SDL_ERROR, errorMessage, _file, _func, _line);
    }
}
#endif
