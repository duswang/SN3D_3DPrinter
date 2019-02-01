/**
 * @file IMAGE_VIEWER_FB.c
 * @author Bato
 * @date 7 Nov 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 */

#ifdef __APPLE__
#include <png.h>
#include <zlib.h>
#endif

#ifdef linux
#include <linux/fb.h>
#include <png.h>
#include <zlib.h>
#endif

#include "SN_API.h"
#include "SN_MODULE_IMAGE_VIEWER.h"

#include "IMAGE_VIEWER_FB.h"

/* ******* STATIC DEFINE ******* */

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
/* *** IMAGE *** */
static SN_STATUS sLoadImage(const char* filename, FB_Image_t* image);
static SN_STATUS sDistroyImage(FB_Image_t* pImage);

/* *** FRAME BUFFER *** */
static SN_STATUS sLoadWindow(const char* devicename, FB_Window_t* window);
static SN_STATUS sUpdateWindow(const FB_Window_t window, const FB_Image_t image);
static SN_STATUS sCleanWindow(const FB_Window_t window);
static SN_STATUS sDistroyWindow(FB_Window_t* window);

/* *** THUMBNAIL *** */
static SN_STATUS sLoadThumbnail(FB_Image_t* image, FB_Image_t* thumbnail, int thumbnailWidth);
static SN_STATUS sLoadThumbnailInfo(FB_ThumbnailInfo_t* thumbnailInfo);
static SN_STATUS sUpdateNextionThumbnail(const FB_Image_t thumbnail, const FB_ThumbnailInfo_t thumbnaileInfo);
static SN_STATUS sCleanNextionThumbnail(const FB_ThumbnailInfo_t thumbnailInfo);

static uint32_t sCalculateWidthNextionThumbnail(const FB_Image_t image, const FB_ThumbnailInfo_t thumbnailInfo);

/* *** RGB CONTROL *** */
inline static unsigned char  make8color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make15color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make16color(unsigned char r, unsigned char g, unsigned char b);
static void* convertRGB2FB(int fh, unsigned char *rgbbuff, unsigned long count, int bpp, int *cpp);

/* *** UTIL *** */
static SN_STATUS sRotateImage(FB_Image_t* image);


/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
SN_STATUS ImageViewer_Init(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(moduleImageViewer == NULL)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load FrameBuffer Window */
    retStatus = sLoadWindow(WINDOW_NAME, &moduleImageViewer->window);

    /* Init Window */
    retStatus = sCleanWindow(moduleImageViewer->window);

    /* Init Thumbnail Info */
    retStatus = sLoadThumbnailInfo(&moduleImageViewer->thumbnailInfo);
    return retStatus;
}

SN_STATUS ImageViewer_WindowUpdate(moduleImageViewer_t* moduleImageViewer, uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char* path = NULL;
    const printTarget_t* printTarget = NULL;
    const machineInfo_t* machineInfo = NULL;

    /* GET PRINT TARGET INFO */
    printTarget = SN_MODULE_FILE_SYSTEM_TargetGet();
    if(printTarget == NULL)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    /* Get Mahcine Info */
    machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(machineInfo == NULL)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load and Update Image to Screen */
    path = SN_MODULE_FILE_SYSTEM_TargetSlicePathGet(sliceIndex);

    retStatus = sLoadImage(path, &moduleImageViewer->image);
    SN_SYS_ERROR_CHECK(retStatus, "Load Image Failed.");

    retStatus = sUpdateWindow(moduleImageViewer->window, moduleImageViewer->image);
    SN_SYS_ERROR_CHECK(retStatus, "Window Update Failed.");

    /* Load and Update Thumbnail Nextion Display */
    retStatus = sLoadThumbnail(&moduleImageViewer->image, &moduleImageViewer->thumbnail, sCalculateWidthNextionThumbnail(moduleImageViewer->image, moduleImageViewer->thumbnailInfo));
    SN_SYS_ERROR_CHECK(retStatus, "Load thumbnail Failed.");

    retStatus = sUpdateNextionThumbnail(moduleImageViewer->thumbnail, moduleImageViewer->thumbnailInfo);
    SN_SYS_ERROR_CHECK(retStatus, "Thumbnail Update Failed.");

    /* Distroy Image */
    retStatus = sDistroyImage(&moduleImageViewer->image);
    SN_SYS_ERROR_CHECK(retStatus, "Image Distroy Failed.");

    free(path);
    path = NULL;

    return retStatus;
}

SN_STATUS ImageViewer_WindowClean(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus= sCleanWindow(moduleImageViewer->window);
    SN_SYS_ERROR_CHECK(retStatus, "Image Clean Failed.");

    return retStatus;
}

SN_STATUS ImageViewer_WindowDestroy(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    sDistroyImage(&moduleImageViewer->image);
    sDistroyImage(&moduleImageViewer->thumbnail);
    sDistroyWindow(&moduleImageViewer->window);

    return retStatus;
}

SN_STATUS ImageViewer_ThumbnailClean(moduleImageViewer_t* moduleImageViewer)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    const machineInfo_t* machineInfo = NULL;

    /* Get Mahcine Info */
    machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(machineInfo == NULL)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    retStatus= sCleanNextionThumbnail(moduleImageViewer->thumbnailInfo);
    SN_SYS_ERROR_CHECK(retStatus, "Thumbnail Clean Failed.");

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  IMAGE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sLoadImage(const char* filename, FB_Image_t* pImage)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    unsigned char header[8];    // 8 is the maximum size that can be checked

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    int transparent = 0;

    unsigned char* row_pointer;
    unsigned char* fbptr;

    png_bytep rptr[2];

    int pass = 0, i = 0;;

    if(filename == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "filename is NULL.");
    }

    if(pImage == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "image structure is NULL.");
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("PATH => %s \n", filename);
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "load image faild. check image path.");
    }

    /* HEDER CHECK */
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_SUPPORTED, "is not png file.");
    }

    /* INIT PNG STRUCTURE */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "create png struct faild.");
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "create png info struct faild.");
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "Error during init_io.");
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    /* READ PNG IMAGE INFO  */
    pImage->w = png_get_image_width(png_ptr, info_ptr);
    pImage->h = png_get_image_height(png_ptr, info_ptr);
    pImage->colorType = png_get_color_type(png_ptr, info_ptr);
    pImage->bpp = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

    /* READ PNG IMAGE */
    if (setjmp(png_jmpbuf(png_ptr)))
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "Error during");
    }

    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        transparent = 1;
        png_set_tRNS_to_alpha(png_ptr);
    }

    /* CREATE PNG RGB BUFFER */
    pImage->rgb = (unsigned char*)malloc((pImage->w) * (pImage->h) * 3); //RGB 888
    if (pImage->rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_OUT_OF_MEM, " failed to allocate image rgb buffer");
    }

    /* CREATE PNG ALPHA BUFFER */
    if((pImage->colorType ==  PNG_COLOR_TYPE_RGB_ALPHA) || \
       (pImage->colorType == PNG_COLOR_TYPE_GRAY_ALPHA) || \
                                      transparent != 0)
    {
        unsigned char* alpha_buffer = (unsigned char *)malloc(pImage->w * pImage->h);
        unsigned char* aptr;

        row_pointer = (unsigned char*)malloc((pImage->w) * 4);
        rptr[0] = (png_bytep) row_pointer;

        pImage->alpha = alpha_buffer;

        for (pass = 0; pass < number_of_passes; pass++)
        {
            fbptr = pImage->rgb;
            aptr  = alpha_buffer;

            for (i = 0; i < pImage->h; i++)
            {
                int n;
                unsigned char *trp = row_pointer;

                png_read_rows(png_ptr, rptr, NULL, 1);

                for(n = 0; n < pImage->w; n++, fbptr+=3, trp+=4)
                {
                    memcpy(fbptr, trp, 3);
                    *(aptr++) = trp[3];
                }
            }
        }

        free(row_pointer);
        row_pointer = NULL;
    }
    else
    {
        for(pass = 0; pass < number_of_passes; pass++)
        {
            fbptr = pImage->rgb;
            for(i = 0; i < pImage->h; i++, fbptr += pImage->w * 3)
            {
                rptr[0] = (png_bytep)fbptr;
                png_read_rows(png_ptr, rptr, NULL, 1);
            }
        }
    }

    /* PNG DONE */
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, (NULL));

    fclose(fp);

    //printf("Module => Image Viwer => Image Info : [ %d x %d], %dbpp %d\n", pImage->w, pImage->h, pImage->bpp, pImage->colorType);

    return retStatus;
}

static SN_STATUS sRotateImage(FB_Image_t* image)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    FB_Image_t rotatedImage;
    int i = 0;
    int j = 0;

    int rotatedPixel_offset = 0;
    int ImagePixel_offset = 0;

    if(image == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "image is NULL");
    }

    if(image->rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "image rgb is NULL");
    }

    rotatedImage.rgb = (unsigned char*)malloc(((image->w * image->h) * sizeof(unsigned char)));
    if (rotatedImage.rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail rgb failed memory allocate.");
    }

    rotatedImage.bpp       = image->bpp;
    rotatedImage.colorType = image->colorType;
    rotatedImage.h         = image->w;
    rotatedImage.w         = image->h;

    for(i = 0; i < rotatedImage.h; i++)
    {
        for(j = 0; j < rotatedImage.w; j++)
        {
            rotatedPixel_offset = (i * rotatedImage.w) + j;
            ImagePixel_offset = (((image->h - 1) * image->w) - (j * image->w)) + i;

            rotatedImage.rgb[rotatedPixel_offset] = image->rgb[ImagePixel_offset];
        }
    }

    free(image->rgb);
    image->rgb = NULL;

    free(image->alpha);
    image->alpha = NULL;

    *image = rotatedImage;

    return retStatus;
}

static SN_STATUS sLoadThumbnail(FB_Image_t* image, FB_Image_t* thumbnail, int thumbnailWidth)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int thumbnailHeight = 0;

    int i = 0;
    int j = 0;

    unsigned char *thumbnailPixel = NULL;
    int thumbnailPixel_offset = 0;
    int ImagePixel_offset = 0;

    if(image == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "image is NULL");
    }

    if(thumbnail == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "thumbnail is NULL");
    }

    thumbnailHeight = image->h * thumbnailWidth / image->w;

    thumbnail->rgb = (unsigned char*)malloc(((thumbnailWidth * thumbnailHeight) * sizeof(unsigned char)));
    if (thumbnail->rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail rgb failed memory allocate.");
    }

    thumbnail->alpha = NULL;

    thumbnail->bpp       = image->bpp;
    thumbnail->colorType = image->colorType;
    thumbnail->h         = thumbnailHeight;
    thumbnail->w         = thumbnailWidth;

    for(i = 0; i < thumbnailHeight; i++)
    {
        for (j = 0; j < thumbnailWidth; j++)
        {
            thumbnailPixel_offset = (i * thumbnailWidth) + j;
            thumbnailPixel = thumbnail->rgb + thumbnailPixel_offset;

            ImagePixel_offset = ((i * image->w) * (image->w / thumbnailWidth) * 3) + (j * (image->w / thumbnailWidth) * 3); // * 3 for RGB

            // get first color channel of the rgb image buffer
            if(image->rgb[ImagePixel_offset] > 127)
            {
                *thumbnailPixel = 1;
            }
            else
            {
                *thumbnailPixel = 0;
            }
        }
    }

    /*
    printf("\nThumbnail Info : [ %d x %d], %dbpp %d\n",thumbnail->w, thumbnail->h, thumbnail->bpp, thumbnail->colorType);
    for (i = 0; i < thumbnail->h; i++)
    {
        for (j = 0; j < thumbnail->w; j++)
        {
            printf("%d", !thumbnail->rgb[i * thumbnail->w + j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
    */
    if(thumbnail->h > thumbnail->w)
    {
        sRotateImage(thumbnail);
    }

    /*
    printf("\nThumbnail Info : [ %d x %d], %dbpp %d\n",thumbnail->w, thumbnail->h, thumbnail->bpp, thumbnail->colorType);
    for (i = 0; i < thumbnail->h; i++)
    {
        for (j = 0; j < thumbnail->w; j++)
        {
            printf("%d", !thumbnail->rgb[i * thumbnail->w + j]);
        }
        printf("\n");
    }
    printf("\n\n\n");
    */

    return retStatus;
}


static SN_STATUS sDistroyImage(FB_Image_t* image)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(image == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    if(image->rgb != NULL)
    {
        free(image->rgb);
        image->rgb = NULL;
    }

    if(image->alpha != NULL)
    {
        free(image->alpha);
        image->alpha = NULL;
    }

    image->h = 0;
    image->w = 0;

    image->colorType = 0;
    image->bpp       = 0;

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  WINDOW
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sLoadWindow(const char* devicename, FB_Window_t* window)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if(devicename == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "devicename param is NULL.");
    }

    if(window == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_INVALID_PARAM, "window param is NULL.");
    }

    fbfd = open(devicename, O_RDWR);
    if (fbfd == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "cannot open framebuffer device.");
    }
    window->name = devicename;

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

    printf("Image Viwer => Module => Window Resolution [ %d x %d ], %dbpp\n", window->w, window->h, window->bpp);

    close(fbfd);

    return retStatus;
}


static SN_STATUS sUpdateWindow(const FB_Window_t window, const FB_Image_t image)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int fbfd = 0;
    int bp = 0;

    char* fbp = 0;

    unsigned short* fbbuff = NULL;
    unsigned char*  fbptr  = NULL;
    unsigned char*  imptr  = NULL;

    if(window.name == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "window not initialzed.");
    }

    fbfd = open(window.name, O_RDWR);
    if (fbfd == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "cannot open framebuffer device.");
    }

    fbbuff = convertRGB2FB(fbfd, image.rgb, image.w * image.h, window.bpp, &bp);

    /* Map the device to memory */
    fbp = (char *)mmap(0, window.screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "failed to map framebuffer device to memory.");
    }

    fbptr = (unsigned char *)fbp;
    imptr = (unsigned char *)fbbuff;

    for (int i = 0; i < image.h; i++, fbptr += image.w * bp, imptr += image.w * bp)
    {
        memcpy(fbptr, imptr, image.w * bp);
    }

    free(fbbuff);
    fbbuff = NULL;

    munmap(fbp, window.screenSize);

    close(fbfd);

    return retStatus;
}


static SN_STATUS sCleanWindow(const FB_Window_t window)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    int fbfd = 0;
    int i = 0;
    int j = 0;
    long int location = 0;
    char *fbp = 0;

    if(window.name == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "window not initialzed.");
    }

    fbfd = open(window.name, O_RDWR);
    if (fbfd == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "cannot open framebuffer device.");
    }

    /* Map the device to memory */
    fbp = (char *)mmap(0, window.screenSize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_OK, "failed to map framebuffer device to memory.");
    }

    /* Figure out where in memory to put the pixel */
    for (i = 0; i < window.h; i++)
    {

        for (j = 0; j < window.w; j++)
        {
            location = (j * (window.bpp / 8)) +
                       (i * (window.w * (window.bpp / 8)));

            if(window.bpp == 32)
            {
                *(fbp + location)     = 0;        // black
                *(fbp + location + 1) = 0;
                *(fbp + location + 2) = 0;
                *(fbp + location + 3) = 255;
                //location += 4;
            }
            else //assume 16bpp
            {
                int b = 0;    //black
                int g = 0;
                int r = 0;
                unsigned short int t = r<<11 | g << 5 | b;
                *((unsigned short int*)(fbp + location)) = t;
            }
            //else 8bpp
        }
    }

    munmap(fbp, window.screenSize);

    close(fbfd);

    return retStatus;
}

static SN_STATUS sDistroyWindow(FB_Window_t* window)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(window == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    window->name       = NULL;

    window->screenSize = 0;
    window->h          = 0;
    window->w          = 0;
    window->bpp        = 0;

    return retStatus;
}

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  THUMBNAIL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

static SN_STATUS sLoadThumbnailInfo(FB_ThumbnailInfo_t* thumbnailInfo)
{
    const machineInfo_t* machineInfo = NULL;

    /* Get Mahcine Info */
    machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(machineInfo == NULL)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    if(!strcmp(machineInfo->touchScreenSize, NEXTION_DISPLAY_TYPE_3_2_INCH))
    {
        thumbnailInfo->thumbnail_offset_x = NEXTION_THUMBNAIL_3_2_OFFSET_X;
        thumbnailInfo->thumbnail_offset_y = NEXTION_THUMBNAIL_3_2_OFFSET_Y;
        thumbnailInfo->thumbnail_width    = NEXTION_THUMBNAIL_3_2_WIDTH;
        thumbnailInfo->thumbnail_height   = NEXTION_THUMBNAIL_3_2_HEIGHT;
    }
    else if(!strcmp(machineInfo->touchScreenSize, NEXTION_DISPLAY_TYPE_4_3_INCH))
    {
        thumbnailInfo->thumbnail_offset_x = NEXTION_THUMBNAIL_4_3_OFFSET_X;
        thumbnailInfo->thumbnail_offset_y = NEXTION_THUMBNAIL_4_3_OFFSET_Y;
        thumbnailInfo->thumbnail_width    = NEXTION_THUMBNAIL_4_3_WIDTH;
        thumbnailInfo->thumbnail_height   = NEXTION_THUMBNAIL_4_3_HEIGHT;
    }
    else if(!strcmp(machineInfo->touchScreenSize, NEXTION_DISPLAY_TYPE_5_0_INCH))
    {
        thumbnailInfo->thumbnail_offset_x = NEXTION_THUMBNAIL_5_0_OFFSET_X;
        thumbnailInfo->thumbnail_offset_y = NEXTION_THUMBNAIL_5_0_OFFSET_Y;
        thumbnailInfo->thumbnail_width    = NEXTION_THUMBNAIL_5_0_WIDTH;
        thumbnailInfo->thumbnail_height   = NEXTION_THUMBNAIL_5_0_HEIGHT;
    }
    else if(!strcmp(machineInfo->touchScreenSize, NEXTION_DISPLAY_TYPE_7_0_INCH))
    {
        thumbnailInfo->thumbnail_offset_x = NEXTION_THUMBNAIL_7_0_OFFSET_X;
        thumbnailInfo->thumbnail_offset_y = NEXTION_THUMBNAIL_7_0_OFFSET_Y;
        thumbnailInfo->thumbnail_width    = NEXTION_THUMBNAIL_7_0_WIDTH;
        thumbnailInfo->thumbnail_height   = NEXTION_THUMBNAIL_7_0_HEIGHT;
    }
    else
    {
        thumbnailInfo->thumbnail_offset_x = NEXTION_THUMBNAIL_3_2_OFFSET_X;
        thumbnailInfo->thumbnail_offset_y = NEXTION_THUMBNAIL_3_2_OFFSET_Y;
        thumbnailInfo->thumbnail_width    = NEXTION_THUMBNAIL_3_2_WIDTH;
        thumbnailInfo->thumbnail_height   = NEXTION_THUMBNAIL_3_2_HEIGHT;
    }

    return SN_STATUS_OK;
}

static SN_STATUS sUpdateNextionThumbnail(const FB_Image_t thumbnail, const FB_ThumbnailInfo_t thumbnaileInfo)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int i = 0, j = 0;
    int lineStart = 0, lineEnd = 0;
    int lineOffset = 0;

    //lineOffset = (DEFAULT_NEXTION_THUMBNAIL_HEIGHT - thumbnail.h) / 2;

    if(thumbnail.rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail image is not initialized.");
    }

    for (i = 0; i < thumbnail.h; i++)
    {
        for (j = 0; j < thumbnail.w; j++)
        {
            if(thumbnail.rgb[i * thumbnail.w + j])
            {
                lineStart = j;

                while(thumbnail.rgb[(i * thumbnail.w) + j])
                {
                    if(j >= thumbnail.w)
                    {
                        break;
                    }

                    j++;
                }
                lineEnd = j;

                SN_MODULE_DISPLAY_NextionDrawLine(thumbnaileInfo.thumbnail_offset_x + lineStart, \
                                                  thumbnaileInfo.thumbnail_offset_y + lineOffset + i, \
                                                  thumbnaileInfo.thumbnail_offset_x + lineEnd, \
                                                  thumbnaileInfo.thumbnail_offset_y + lineOffset + i, \
                                                  DEFAULT_NEXTION_THUMBNAIL_ON_PIXEL_COLOR);
                SN_SYS_Delay(3);
            }
            else
            {
                /* Skip This Line */
            }
        }
    }

    return retStatus;
}
static SN_STATUS sCleanNextionThumbnail(const FB_ThumbnailInfo_t thumbnailInfo)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = SN_MODULE_DISPLAY_NextionDrawFill(thumbnailInfo.thumbnail_offset_x , \
                                                  thumbnailInfo.thumbnail_offset_y, \
                                                  thumbnailInfo.thumbnail_width, \
                                                  thumbnailInfo.thumbnail_height, \
                                                  DEFAULT_NEXTION_THUMBNAIL_OFF_PIXEL_COLOR);

    return retStatus;
}

static uint32_t sCalculateWidthNextionThumbnail(const FB_Image_t image, const FB_ThumbnailInfo_t thumbnailInfo)
{
    int width = 0;

    if(image.rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail image is not initialized.");
    }

    if(image.w < image.h) /* Vertical Image */
    {
        width = thumbnailInfo.thumbnail_width * image.w / image.h;
    }
    else /* horizontal */
    {
        width = thumbnailInfo.thumbnail_width;
    }

    return width;
}
/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  RGB CONTROL
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

inline static unsigned char make8color(unsigned char r, unsigned char g, unsigned char b)
{
    return (
    (((r >> 5) & 7) << 5) |
    (((g >> 5) & 7) << 2) |
     ((b >> 6) & 3)       );
}

inline static unsigned short make15color(unsigned char r, unsigned char g, unsigned char b)
{
    return (
    (((r >> 3) & 31) << 10) |
    (((g >> 3) & 31) << 5)  |
     ((b >> 3) & 31)        );
}

inline static unsigned short make16color(unsigned char r, unsigned char g, unsigned char b)
{
    return (
    (((r >> 3) & 31) << 11) |
    (((g >> 2) & 63) << 5)  |
     ((b >> 3) & 31)        );
}

void* convertRGB2FB(int fh, unsigned char *rgbbuff, unsigned long count, int bpp, int *cpp)
{
    unsigned long i;
    void *fbbuff = NULL;
    u_int8_t  *c_fbbuff;
    u_int16_t *s_fbbuff;
    u_int32_t *i_fbbuff;

    switch(bpp)
    {
    case 8:
        *cpp = 1;
        c_fbbuff = (unsigned char *) malloc(count * sizeof(unsigned char));
        for(i = 0; i < count; i++)
        c_fbbuff[i] = make8color(rgbbuff[i*3], rgbbuff[i*3+1], rgbbuff[i*3+2]);
        fbbuff = (void *) c_fbbuff;
        break;
    case 15:
        *cpp = 2;
        s_fbbuff = (unsigned short *) malloc(count * sizeof(unsigned short));
        for(i = 0; i < count ; i++)
        s_fbbuff[i] = make15color(rgbbuff[i*3], rgbbuff[i*3+1], rgbbuff[i*3+2]);
        fbbuff = (void *) s_fbbuff;
        break;
    case 16:
        *cpp = 2;
        s_fbbuff = (unsigned short *) malloc(count * sizeof(unsigned short));
        for(i = 0; i < count ; i++)
        s_fbbuff[i] = make16color(rgbbuff[i*3], rgbbuff[i*3+1], rgbbuff[i*3+2]);
        fbbuff = (void *) s_fbbuff;
        break;
    case 24:
    case 32:
        *cpp = 4;
        i_fbbuff = (unsigned int *) malloc(count * sizeof(unsigned int));
        for(i = 0; i < count ; i++)
        i_fbbuff[i] = ((rgbbuff[i*3] << 16) & 0xFF0000) |
                ((rgbbuff[i*3+1] << 8) & 0xFF00) |
                (rgbbuff[i*3+2] & 0xFF);
        fbbuff = (void *) i_fbbuff;
        break;
    default:
        fprintf(stderr, "Unsupported video mode! You've got: %dbpp\n", bpp);
        exit(1);
    }
    return fbbuff;
}
