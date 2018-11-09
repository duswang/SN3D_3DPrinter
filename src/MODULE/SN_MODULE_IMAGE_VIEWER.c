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
 * @todo memory test
 * @todo nextion display thumbnail.
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

#define DEFAULT_WINDOW_WIDTH  1440
#define DEFAULT_WINDOW_HEIGHT 2560

#if(IMAGE_VIEWER_USE_SDL)
#define WINDOW_NAME "sn3d"
#else
#define WINDOW_NAME "/dev/fb0"
#endif

/* *** MODULE *** */
#if(IMAGE_VIEWER_USE_SDL)
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
#else

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

typedef struct image_viewer
{
    machineInfo_t   machineInfo;
    printInfo_t     printInfo;

    FB_Window_t window;
    FB_Image_t  image;
    FB_Image_t  thumbnail;
} moduleImageViewer_t;
#endif


/* ******* SYSTEM DEFINE ******* */
/* *** MODULE HANDLER  *** */
static moduleImageViewer_t moduleImageViewer;

/* ******* GLOBAL VARIABLE ******* */

/* ******* STATIC FUNCTIONS ******* */
#if(IMAGE_VIEWER_USE_SDL)
/* *** IMAGE *** */
static SDL_Texture* sLoadTexture(const char* filename, SDL_Renderer *renderer);
/* *** ERROR ****/
static void sCheckError_SDL(const char* message, const char* _file, const char* _func, const int _line);
static void sCheckError_SDL_Image(const char* message, const char* _file, const char* _func, const int _line);
#else
/* *** IMAGE *** */
static SN_STATUS sRotateImage(FB_Image_t* imagem,int mode);
static SN_STATUS sLoadImage(const char* filename, FB_Image_t* image);
static SN_STATUS sLoadThumbnail(FB_Image_t* image, FB_Image_t* thumbnail, int thumbnailWidth);
static SN_STATUS sDistroyImage(FB_Image_t* pImage);

/* *** FRAME BUFFER *** */
static SN_STATUS sLoadWindow(const char* devicename, FB_Window_t* window);
static SN_STATUS sUpdateWindow(const FB_Window_t window, const FB_Image_t image);
static SN_STATUS sCleanWindow(const FB_Window_t window);
static SN_STATUS sDistroyWindow(FB_Window_t* window);

/* *** THUMBNAIL *** */
static SN_STATUS sUpdateNextionThumbnail(FB_Image_t* thumbnail);
static SN_STATUS sCleanNextionThumbnail(void);

/* *** UTIL *** */
inline static unsigned char make8color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make15color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make16color(unsigned char r, unsigned char g, unsigned char b);
static void* convertRGB2FB(int fh, unsigned char *rgbbuff, unsigned long count, int bpp, int *cpp);

#endif

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  Extern Functions
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */
#if(!IMAGE_VIEWER_OFF)
#if(IMAGE_VIEWER_USE_SDL)
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

    printf("Image Viwer => Module => Display Resolution [ %d x %d ] %dHz %d\n", moduleImageViewer.dm.w, moduleImageViewer.dm.h, \
                                                                                 moduleImageViewer.dm.refresh_rate, \
                                                                                 moduleImageViewer.dm.format);



#if(IMAGE_VIEWER_FIX_RESOLUTION)
    printf("Image Viwer => Module => Window Resolution [ %d x %d ]\n", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    moduleImageViewer.dm.w = DEFAULT_WINDOW_WIDTH;
    moduleImageViewer.dm.h = DEFAULT_WINDOW_HEIGHT;
#endif


    /* SDL CURSOR INIT */
    SDL_ShowCursor(SDL_DISABLE);

    /* SDL WINDOWS INIT */
    moduleImageViewer.window = SDL_CreateWindow(WINDOW_NAME, \
            SDL_WINDOWPOS_UNDEFINED, \
            SDL_WINDOWPOS_UNDEFINED, \
            moduleImageViewer.dm.w, \
            moduleImageViewer.dm.h, \
            SDL_WINDOW_OPENGL );

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

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex)
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

SN_STATUS SN_MODULE_IMAGE_VIEWER_Window(void)
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

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void)
{
    SDL_DestroyTexture(moduleImageViewer.texture);
    IMG_Quit();
    SDL_DestroyRenderer(moduleImageViewer.renderer);
    SDL_DestroyWindow(moduleImageViewer.window);
    SDL_Quit();

    return SN_STATUS_OK;
}
#else
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    /* GET MACHINE INFO */
    moduleImageViewer.machineInfo = SN_MODULE_FILE_SYSTEM_MachineInfoGet();
    if(!moduleImageViewer.machineInfo.isInit)
    {
        return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load FrameBuffer Window */
    retStatus = sLoadWindow(WINDOW_NAME, &moduleImageViewer.window);

    /* Init Window */
    retStatus = sCleanWindow(moduleImageViewer.window);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char path[MAX_PATH_LENGTH] = {'\0', };

    /* GET PRINT TARGET INFO */
    printInfo_t printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
    if(!printInfo.isInit)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load Image & thumbnail */
    sprintf(path,"%s/%s%04d.png", printInfo.printTarget.targetPath, printInfo.printTarget.targetName, sliceIndex);
    retStatus = sLoadImage(path, &moduleImageViewer.image);
    SN_SYS_ERROR_CHECK(retStatus, "Load Image Failed.");

    retStatus = sLoadThumbnail(&moduleImageViewer.image, &moduleImageViewer.thumbnail, 40);
    SN_SYS_ERROR_CHECK(retStatus, "Load thumbnail Failed.");

    retStatus = sUpdateWindow(moduleImageViewer.window, moduleImageViewer.image);
    SN_SYS_ERROR_CHECK(retStatus, "Window Update Failed.");

    /* Distroy Image */
    retStatus = sDistroyImage(&moduleImageViewer.image);
    SN_SYS_ERROR_CHECK(retStatus, "Image Clean Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowClean(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus= sCleanWindow(moduleImageViewer.window);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_NextionUpdate(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sUpdateNextionThumbnail(&moduleImageViewer.thumbnail);
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Thumbnail Update Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_NextionClean(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    retStatus = sCleanNextionThumbnail();
    SN_SYS_ERROR_CHECK(retStatus, "Nextion Thumbnail Clean Failed.");

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    sDistroyImage(&moduleImageViewer.image);
    sDistroyImage(&moduleImageViewer.thumbnail);
    sDistroyWindow(&moduleImageViewer.window);

    return retStatus;
}

#endif
#else
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowUpdate(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowClear(void)
{
    return SN_STATUS_OK;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_WindowDestroy(void)
{
    return SN_STATUS_OK;
}

#endif

/* * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * *
 *
 *  IMAGE
 *
 * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * */

#if(IMAGE_VIEWER_USE_SDL)
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
#else
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

    printf("\nImage Info : [ %d x %d], %dbpp %d\n", pImage->w, pImage->h, pImage->bpp, pImage->colorType);

    return retStatus;
}

static SN_STATUS sRotateImage(FB_Image_t* image, int mode)
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

            switch(mode)
            {
                case 0:
                    rotatedPixel_offset = (i * rotatedImage.w) + j;
                    ImagePixel_offset = (((image->h - 1) * image->w) - (j * image->w)) + i;
                    break;
                case 1:
                    break;
                case 2:
                    break;
                default :
                    break;
            }

            rotatedImage.rgb[rotatedPixel_offset] = image->rgb[ImagePixel_offset];
        }
    }

    free(image->rgb);
    free(image->alpha);

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

    thumbnailHeight = image->h * ((float)thumbnailWidth / (float)image->w);

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

    sRotateImage(thumbnail, 0);

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
    }

    if(image->alpha != NULL)
    {
        free(image->alpha);
    }

    image->h = 0;
    image->w = 0;

    image->colorType = 0;
    image->bpp       = 0;

    return retStatus;
}

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

static SN_STATUS sUpdateNextionThumbnail(FB_Image_t* thumbnail)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int i = 0, j = 0;
    //int offset_x = 0;
    //int offset_y = 0;

    if(thumbnail == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail is not initialized.");
    }

    if(thumbnail->rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail image is not initialized.");
    }

    for(i = 0; i < thumbnail->h; i++)
    {
        for(j = 0; j < thumbnail->w; j++)
        {
            //SN_MODULE_DISPLAY_NextionDrawLine(offset_x + j, offset_y + i, NX_COLOR_BLACK);
        }
    }

    return retStatus;
}

static SN_STATUS sCleanNextionThumbnail(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int i = 0, j = 0;
    int offset_x = 0;
    int offset_y = 0;

    if(moduleImageViewer.thumbnail.rgb == NULL)
    {
        SN_SYS_ERROR_CHECK(SN_STATUS_NOT_INITIALIZED, "thumbnail is image not initialized.");
    }

    for(i = 0; i < moduleImageViewer.thumbnail.h; i++)
    {
        SN_MODULE_DISPLAY_NextionDrawLine(offset_x + j, offset_y + i, NX_COLOR_BLACK);
        SDL_Delay(3);
    }

    return retStatus;
}
#endif
#if(IMAGE_VIEWER_USE_SDL)
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
