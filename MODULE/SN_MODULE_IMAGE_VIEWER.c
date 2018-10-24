/**
 * @file SN_MODULE_IMAGE_VIEWER.c
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 */

#include "SN_API.h"
#include "SN_MODULE_IMAGE_VIEWER.h"

/* ******* STATIC DEFINE ******* */
/** @def SN_SYS_ERROR_CHECK_SDL(msg)
 *  @brief
 *
 *  @param msg
 *
 *  @return SN_STATUS
 */
#define SN_SYS_ERROR_CHECK_SDL(msg) \
        sCheckError_SDL((msg), __FILE__, __FUNCTION__, __LINE__)

/** @def SN_SYS_ERROR_CHECK_SDL_IMAGE(msg)
 *  @brief
 *
 *  @param msg
 *
 *  @return SN_STATUS
 */
#define SN_SYS_ERROR_CHECK_SDL_IMAGE(msg) \
        sCheckError_SDL_Image((msg), __FILE__, __FUNCTION__, __LINE__)


/* *** MODULE *** */
typedef struct image_viewer
{
    SDL_Window     *window;
    SDL_Renderer *renderer;
    SDL_Texture   *texture;
    SDL_DisplayMode dm;

    SDL_Rect     dest_rect;

    machineInfo_t machineInfo;
    printInfo_t   printInfo;

    char*       image_path;
    uint32_t    image_w;
    uint32_t    image_h;
} moduleImageViewer_t;

/* ******* SYSTEM DEFINE ******* */
/* *** MODULE HANDLER  *** */
static moduleImageViewer_t moduleImageViewer;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** IMAGE *** */
static SDL_Texture* sLoadTexture(const char* fname, SDL_Renderer *renderer);

/* *** ERROR ****/
static void sCheckError_SDL(const char* message, const char* _file, const char* _func, const int _line);
static void sCheckError_SDL_Image(const char* message, const char* _file, const char* _func, const int _line);

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
#if(!IMAGE_VIEWER_OFF)
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* GET MACHINE INFO */
    moduleImageViewer.machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(!moduleImageViewer.machineInfo.isInit)
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
    SDL_GetDesktopDisplayMode(0, &moduleImageViewer.dm);

    printf("Image Viwer => Module => h%d w%d\n", moduleImageViewer.dm.h, moduleImageViewer.dm.w);

    /* SDL CURSOR INIT */
    SDL_ShowCursor(SDL_DISABLE);

    /* SDL WINDOWS INIT */
    moduleImageViewer.window = SDL_CreateWindow("SN3D", \
            SDL_WINDOWPOS_UNDEFINED, \
            SDL_WINDOWPOS_UNDEFINED, \
            moduleImageViewer.dm.w, \
            moduleImageViewer.dm.h, \
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );

    if(moduleImageViewer.window == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL("Unable to create window");
    }

    /* SDL RENDERER INIT */
    moduleImageViewer.renderer = SDL_CreateRenderer(moduleImageViewer.window, -1, 0);
    if(moduleImageViewer.renderer == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL("Unable to create window");
    }

    /* IMAGE INIT */
    int flags=IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    if((initted & flags) != flags)
    {
        SN_SYS_ERROR_CHECK_SDL("Unable to initialize SDL_image");
    }

    SN_MODULE_IMAGE_VIEWER_CLEAR();

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
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
    sprintf(path,"%s%s%04d.png", printInfo.printTarget.tempFilePath, printInfo.printTarget.tempFileName, sliceIndex);

    moduleImageViewer.texture = sLoadTexture(path, moduleImageViewer.renderer);
    error += SDL_QueryTexture(moduleImageViewer.texture, NULL, NULL, &image_w, &image_h);

    moduleImageViewer.dest_rect.x = 0;
    moduleImageViewer.dest_rect.y = 0;
    moduleImageViewer.dest_rect.w = moduleImageViewer.dm.w;
    moduleImageViewer.dest_rect.h = moduleImageViewer.dm.h;

    /* Drawing Image */
    error += SDL_RenderClear(moduleImageViewer.renderer);
    error += SDL_RenderCopy(moduleImageViewer.renderer, moduleImageViewer.texture, NULL, &moduleImageViewer.dest_rect);
    if(!error)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Get error from renderer");
    }

    SDL_RenderPresent(moduleImageViewer.renderer);

    SDL_DestroyTexture(moduleImageViewer.texture);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    ERROR_T error = 0;

    /* Clear */
    error += SDL_RenderClear(moduleImageViewer.renderer);
    error += SDL_SetRenderDrawColor(moduleImageViewer.renderer, 0, 0, 0, 255);
    if(!error)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Get error from renderer");
    }

    SDL_RenderPresent(moduleImageViewer.renderer);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void)
{
    SDL_DestroyTexture(moduleImageViewer.texture);
    IMG_Quit();
    SDL_DestroyRenderer(moduleImageViewer.renderer);
    SDL_DestroyWindow(moduleImageViewer.window);
    SDL_Quit();

    return SN_STATUS_OK;
}
#else
SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void)
{
    return SN_STATUS_OK;
}

#endif

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  IMAGE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SDL_Texture* sLoadTexture(const char* fname, SDL_Renderer *renderer)
{
    if(fname == NULL || renderer == NULL)
    {
        SN_SYS_ERROR_CHECK_SDL_IMAGE("Invalid Paramter in Load Texture.");
    }

    /* INIT IMAGE */
    SDL_Surface *image = IMG_Load(fname);
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
