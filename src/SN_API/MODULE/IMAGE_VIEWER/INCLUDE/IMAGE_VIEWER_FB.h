/**
 * @file IMAGE_VIEWER_FB.h
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @defgroup IMAGE_VIEWER_FB FrameBuffer
 * @ingroup MODULE_IMAGE_VIEWER
 * @brief Image Viewer FramBuffer Functions.
 * @{
 */
#ifndef IMAGE_VIEWER_FB_H_
#define IMAGE_VIEWER_FB_H_

/*************************************************************
 * @name
 * @brief
 * @{
 */

#define DEFAULT_WINDOW_WIDTH  1440
#define DEFAULT_WINDOW_HEIGHT 2560

#define WINDOW_NAME "/dev/fb0"

/*************************************************************@}*/

/*************************************************************
 * @name
 * @brief
 * @{
 */

#define DEFAULT_NEXTION_THUMBNAIL_OFFSET_X 12
#define DEFAULT_NEXTION_THUMBNAIL_OFFSET_Y 45
/* 16:10 */
#define DEFAULT_NEXTION_THUMBNAIL_WIDTH    192
#define DEFAULT_NEXTION_THUMBNAIL_HEIGHT   120

/* 16:9
#define DEFAULT_NEXTION_THUMBNAIL_WIDTH    192
#define DEFAULT_NEXTION_THUMBNAIL_HEIGHT   104
*/

#define DEFAULT_NEXTION_THUMBNAIL_ON_PIXEL_COLOR        NX_COLOR_TABLE_WHITE
#define DEFAULT_NEXTION_THUMBNAIL_OFF_PIXEL_COLOR       NX_COLOR_TABLE_BLACK

/*************************************************************@}*/


/*************************************************************
 * @name
 * @brief
 * @{
 */

typedef struct frameBuffer_Image
{
    unsigned char *rgb;
    unsigned char *alpha;

    int             w;
    int             h;

    int     colorType;
    int           bpp;
} FB_Image_t;

typedef struct frameBuffer_Window
{
    const char* name;

    int            w;
    int            h;
    int          bpp;

    long  screenSize;
} FB_Window_t;

/*************************************************************@}*/


/*************************************************************
 * @name
 * @brief
 * @{
 */
typedef struct image_viewer
{
    FB_Window_t window;
    FB_Image_t  image;
    FB_Image_t  thumbnail;
} moduleImageViewer_t;

/*************************************************************@}*/


/*************************************************************
 * @name Image Viewer FramBuffer
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

/*************************************************************
 * @name Image Viewer Thumbnail
 * @brief
 * @{
 */

/** @brief
 *
 *  @return SN_STATUS
 *  @note
 */
extern SN_STATUS ImageViewer_ThumbnailClean(moduleImageViewer_t* moduleImageViewer);

/*************************************************************@}*/

#endif /* IMAGE_VIEWER_FB_H_ */
