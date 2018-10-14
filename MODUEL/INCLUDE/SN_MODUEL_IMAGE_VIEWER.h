/*
 * SN_MODUEL_IMAGE_VIEWER.h
 *
 *  Created on: 2018. 10. 1.
 *      Author: BartKim
 */

#ifndef MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_
#define MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_

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

#endif /* MODUEL_INCLUDE_SN_MODUEL_IMAGE_VIEWER_H_ */
