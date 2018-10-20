/*
 * SN_MODULE_IMAGE_VIEWER.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "SN_API.h"
#include "SN_MODULE_IMAGE_VIEWER.h"

/******** STATIC DEFINE ********/
typedef struct image_viewer
{
    SDL_Window     *window;
    SDL_Renderer *renderer;
    SDL_Texture   *texture;

    SDL_Rect     dest_rect;

    machineInfo_t machineInfo;
    printInfo_t   printInfo;

    char*       image_path;
    uint32_t    image_w;
    uint32_t    image_h;
} moduleImageViewer_t;

/******** SYSTEM DEFINE ********/
/**** MODULE HANDLER  ****/
static moduleImageViewer_t moduleImageViewer;

/******** GLOBAL VARIABLE ********/

/******** STATIC FUNCTIONS ********/
/**** SYSTEM ****/
void check_error_sdl(bool check, const char* message);
void check_error_sdl_img(bool check, const char* message);

/**** IMAGE ****/
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);

#if(!IMAGE_VIEWER_OFF)
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int flags=IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    /** GET MACHINE INFO **/
    moduleImageViewer.machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(!moduleImageViewer.machineInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    SN_SYS_Log("MODULE INIT => IMAGE VIEWER.");

    /** SDL INIT **/
    check_error_sdl(SDL_Init(SDL_INIT_VIDEO) != 0, "Unable to initialize SDL");

    /** SDL CURSOR INIT **/
    SDL_ShowCursor(SDL_DISABLE);

    /** SDL WINDOWS INIT **/
    moduleImageViewer.window = SDL_CreateWindow("SN3D", \
            SDL_WINDOWPOS_UNDEFINED, \
            SDL_WINDOWPOS_UNDEFINED, \
            moduleImageViewer.machineInfo.display.weight, \
            moduleImageViewer.machineInfo.display.height, \
            SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL );
    check_error_sdl(moduleImageViewer.window == NULL, "Unable to create window");

    /** SDL RENDERER INIT **/
    moduleImageViewer.renderer = SDL_CreateRenderer(moduleImageViewer.window, -1, 0);
    check_error_sdl(moduleImageViewer.renderer == NULL, "Unable to create a renderer");

    /** IMAGE INIT **/
    check_error_sdl_img((initted & flags) != flags, "Unable to initialize SDL_image");
    SN_MODULE_IMAGE_VIEWER_CLEAR();

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char path[256] = {'\0', };
    int image_w = 0, image_h = 0;

    /** GET PRINT TARGET INFO **/
    printInfo_t printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
    if(!printInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    sprintf(path,"%s%s%04d.png", printInfo.printTarget.tempFilePath, printInfo.printTarget.tempFileName, sliceIndex);

    moduleImageViewer.texture = load_texture(path, moduleImageViewer.renderer);
    SDL_QueryTexture(moduleImageViewer.texture, NULL, NULL, &image_w, &image_h);

    moduleImageViewer.dest_rect.x = 0;
    moduleImageViewer.dest_rect.y = 0;
    moduleImageViewer.dest_rect.w = image_w;
    moduleImageViewer.dest_rect.h = image_h;

    SDL_RenderClear(moduleImageViewer.renderer);
    SDL_RenderCopy(moduleImageViewer.renderer, moduleImageViewer.texture, NULL, &moduleImageViewer.dest_rect);
    SDL_RenderPresent(moduleImageViewer.renderer);

    SDL_DestroyTexture(moduleImageViewer.texture);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    SDL_RenderClear(moduleImageViewer.renderer);
    SDL_SetRenderDrawColor(moduleImageViewer.renderer, 0, 0, 0, 255);
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

void check_error_sdl(bool check, const char* message)
{
    const char *error = SDL_GetError();

    if (*error) {
      SDL_ClearError();
      SDL_Quit();
      SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, error);
    }
}

void check_error_sdl_img(bool check, const char* message)
{
    const char *error = SDL_GetError();

    if (*error) {
      SDL_ClearError();
      IMG_Quit();
      SDL_Quit();
      SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, error);
    }
}

SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer)
{
    SDL_Surface *image = IMG_Load(fname);
    check_error_sdl_img(image == NULL, "Unable to load image");
    SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
    check_error_sdl_img(img_texture == NULL, "Unable to create a texture from the image");
    SDL_FreeSurface(image);
    return img_texture;
}
