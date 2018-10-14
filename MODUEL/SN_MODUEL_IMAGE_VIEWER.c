/*
 * SN_MODUEL_IMAGE_VIEWER.c
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#include "SN_API.h"
#include "SN_MODUEL_IMAGE_VIEWER.h"

/** Static Define **/
/** System **/

/** Module Controller **/
static moduleImageViewer_t moduleImageViewer;

/** Global Vaerialbe **/


/** Static Funtions **/
// Manage error messages
void check_error_sdl(bool check, const char* message);
void check_error_sdl_img(bool check, const char* message);

// Load an image from "fname" and return an SDL_Texture with the content of the image
SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer);

#if(1)
SN_STATUS SN_MODUEL_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    return retStatus;
}

SN_STATUS SN_MODUEL_IMAGE_VIEWER_CLEAR(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODUEL_IMAGE_VIEWER_Init(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODUEL_IMAGE_VIEWER_Destory(void)
{
    return SN_STATUS_OK;
}
#else
SN_STATUS SN_MODUEL_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    char path[256] = {'\0', };
    int image_w, image_h;

    printInfo_t printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();

    if(!printInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }


    sprintf(path,"%s%s%04d.png", printInfo.printTarget.tempFilePath, printInfo.printTarget.tempFileName, sliceIndex);
    puts(path);

    moduleImageViewer.texture = load_texture(path, moduleImageViewer.renderer);

    SDL_QueryTexture(moduleImageViewer.texture, NULL, NULL, &image_w, &image_h);

    moduleImageViewer.dest_rect.x = 0;
    moduleImageViewer.dest_rect.y = 0;
    moduleImageViewer.dest_rect.w = moduleImageViewer.machineInfo.deviceParameter.weight;
    moduleImageViewer.dest_rect.h = moduleImageViewer.machineInfo.deviceParameter.height;

    SDL_RenderClear(moduleImageViewer.renderer);

    SDL_RenderCopy(moduleImageViewer.renderer, moduleImageViewer.texture, NULL, &moduleImageViewer.dest_rect);

    SDL_RenderPresent(moduleImageViewer.renderer);

    SDL_Delay(100);

    return retStatus;
}

SN_STATUS SN_MODUEL_IMAGE_VIEWER_CLEAR(void)
{
    SDL_RenderClear(moduleImageViewer.renderer);

    SDL_RenderPresent(moduleImageViewer.renderer);
    return SN_STATUS_OK;
}

SN_STATUS SN_MODUEL_IMAGE_VIEWER_Init(void)
{
    check_error_sdl(SDL_Init(SDL_INIT_VIDEO) != 0, "Unable to initialize SDL");

    moduleImageViewer.machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();

    if(!moduleImageViewer.machineInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    moduleImageViewer.window = SDL_CreateWindow("SN3D", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            moduleImageViewer.machineInfo.deviceParameter.weight, \
            moduleImageViewer.machineInfo.deviceParameter.height, \
            SDL_WINDOW_OPENGL);

    check_error_sdl(moduleImageViewer.window == NULL, "Unable to create window");

    moduleImageViewer.renderer = SDL_CreateRenderer(moduleImageViewer.window, -1,\
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    check_error_sdl(moduleImageViewer.renderer == NULL, "Unable to create a renderer");

    SDL_SetRenderDrawColor(moduleImageViewer.renderer, 0, 0, 0, 255);

    int flags=IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);

    check_error_sdl_img((initted & flags) != flags, "Unable to initialize SDL_image");

    SN_MODUEL_IMAGE_VIEWER_CLEAR();

    return SN_STATUS_OK;
}

SN_STATUS SN_MODUEL_IMAGE_VIEWER_Destory(void)
{
    // Clear the allocated resources
    SDL_DestroyTexture(moduleImageViewer.texture);
    IMG_Quit();
    SDL_DestroyRenderer(moduleImageViewer.renderer);
    SDL_DestroyWindow(moduleImageViewer.window);
    SDL_Quit();

    return SN_STATUS_OK;
}

#endif

void check_error_sdl(bool check, const char* message) {
    if (check) {
        printf("%s\n", SDL_GetError());
        SDL_Quit();
        exit(-1);
    }
}

void check_error_sdl_img(bool check, const char* message) {
    if (check) {
        printf("%s\n", IMG_GetError());
        IMG_Quit();
        SDL_Quit();
        exit(-1);
    }
}

SDL_Texture* load_texture(const char* fname, SDL_Renderer *renderer) {
    SDL_Surface *image = IMG_Load(fname);
    check_error_sdl_img(image == NULL, "Unable to load image");

    SDL_Texture *img_texture = SDL_CreateTextureFromSurface(renderer, image);
    check_error_sdl_img(img_texture == NULL, "Unable to create a texture from the image");
    SDL_FreeSurface(image);
    return img_texture;
}
