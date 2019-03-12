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

#define WINDOW_NAME "/dev/fb0"

/*************************************************************@}*/

/*************************************************************
 * @struct frameBuffer_Image
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

typedef struct frameBuffer_Thumbnail_Info
{
    int thumbnail_offset_x;
    int thumbnail_offset_y;
    int thumbnail_width;
    int thumbnail_height;

}FB_ThumbnailInfo_t;
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

    FB_ThumbnailInfo_t thumbnailInfo;

} moduleImageViewer_t;

/*************************************************************@}*/


/*************************************************************
 * @name Image Viewer FramBuffer
 * @brief
 * @{
 */

/** @brief Screen Window Initiation
 *
 *  @param moduleImageViewer - moduleImageViewer_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS ImageViewer_Init(moduleImageViewer_t* moduleImageViewer);

/** @brief Update Target Image to Main Screen with Thumbnail on Nextion Display
 *
 *  @param moduleImageViewer - moduleImageViewer_t pointer
 *  @param sliceIndex - Slice index of number
 *  @return SN_STATUS
 *
 *  @note Slice image path get from MODULE :: FileSystem :: SN_MODULE_FILE_SYSTEM_TargetSlicePathGet
 */
extern SN_STATUS ImageViewer_WindowUpdate(moduleImageViewer_t* moduleImageViewer, uint32_t sliceIndex);

/** @brief Clean Target Image to Main Screen with Thumbnail on Nextion Display
 *
 *  @param moduleImageViewer - moduleImageViewer_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS ImageViewer_WindowClean(moduleImageViewer_t* moduleImageViewer);

/** @brief Display Image Viewer
 *
 *  @param moduleImageViewer - moduleImageViewer_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS ImageViewer_WindowDestroy(moduleImageViewer_t* moduleImageViewer);

/*************************************************************@}*/

/*************************************************************
 * @name Image Viewer Thumbnail
 * @brief
 * @{
 */

/** @brief Clean thumbnail on Nextion Display
 *
 *  @param moduleImageViewer - moduleImageViewer_t pointer
 *  @return SN_STATUS
 *
 *  @note
 */
extern SN_STATUS ImageViewer_ThumbnailClean(moduleImageViewer_t* moduleImageViewer);

/*************************************************************@}*/

#endif /* IMAGE_VIEWER_FB_H_ */
