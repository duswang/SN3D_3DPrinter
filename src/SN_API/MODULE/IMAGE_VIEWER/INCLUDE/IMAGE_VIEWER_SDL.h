/*
 * IMAGE_VIEWER_SDL.h
 *
 *  Created on: 2018. 11. 12.
 *      Author: BartKim
 */
/**
 * @file IMAGE_VIEWER_THUMBNAIL
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup IMAGE_VIEWER_SDL SDL
 * @ingroup MODULE_IMAGE_VIEWER
 * @brief Image Viewer SDL Functions.
 * @{
 */
#ifndef IMAGE_VIEWER_SDL_H_
#define IMAGE_VIEWER_SDL_H_

/*************************************************************
 * @name
 * @brief
 * @{
 */

typedef struct image_viewer
{
    SDL_Window     *window;
    SDL_Renderer *renderer;
    SDL_Texture   *texture;
    SDL_DisplayMode dm;

    SDL_Rect     dest_rect;

    machineInfo_t machineInfo;
    printInfo_t   printInfo;

    unsigned char*       image_path;
    uint32_t    image_w;
    uint32_t    image_h;
} moduleImageViewer_t;

/*************************************************************@}*/

/*************************************************************
 * @name
 * @brief
 * @{
 */

#define DEFAULT_WINDOW_WIDTH  1440
#define DEFAULT_WINDOW_HEIGHT 2560

#define WINDOW_NAME "sn3d"

/*************************************************************@}*/

/*************************************************************
 * @name
 * @brief
 * @{
 */

#define DEFAULT_NEXTION_THUMBNAIL_OFFSET_X 15
#define DEFAULT_NEXTION_THUMBNAIL_OFFSET_Y 50
/* 16:10 */
#define DEFAULT_NEXTION_THUMBNAIL_WIDTH    192
#define DEFAULT_NEXTION_THUMBNAIL_HEIGHT   120

/* 16:9
#define DEFAULT_NEXTION_THUMBNAIL_WIDTH    192
#define DEFAULT_NEXTION_THUMBNAIL_HEIGHT   104
*/

#define DEFAULT_NEXTION_THUMBNAIL_ON_PIXEL_COLOR NX_COLOR_TABLE_WHITE
#define DEFAULT_NEXTION_THUMBNAIL_OFF_PIXEL_COLOR NX_COLOR_TABLE_BLACK

/*************************************************************@}*/

/*************************************************************
 * @name
 * @brief
 * @{
 */

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

/*************************************************************@}*/


/*************************************************************
 * @name Image Viewer SDL
 * @brief
 * @{
 */
/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS ImageVIewer_Init(moduleImageViewer_t* moduleImageViewer);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS ImageVIewer_WindowUpdate(moduleImageViewer_t* moduleImageViewer, uint32_t sliceIndex);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS ImageViewer_WindowClean(moduleImageViewer_t* moduleImageViewer);

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS ImageViewer_WindowDestroy(moduleImageViewer_t* moduleImageViewer);

/*************************************************************@}*/


#endif /* IMAGE_VIEWER_SDL_H_ */
