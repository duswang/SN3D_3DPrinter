/**
 * @file SN_MODULE_IMAGE_VIEWER.c
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @bug SDL :: 2048 x 2048 Over Resolution it can't make Window(surface)
 * @bug FB  :: Not Yet
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

#define DEFAULT_WINDOW_WIDTH 1920
#define DEFAULT_WINDOW_HEIGHT 1080

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
} FB_Image_t;

typedef struct frameBuffer_Window
{
    const char*    name;

    uint32_t w;
    uint32_t h;
    uint32_t bpp;

    long screenSize;
} FB_Window_t;

typedef struct image_viewer
{
    machineInfo_t   machineInfo;
    printInfo_t     printInfo;

    FB_Window_t window;
    FB_Image_t  image;
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
static SN_STATUS sLoadImage(const char* filename, FB_Image_t* image);
static SN_STATUS sDistroyImage(FB_Image_t* pImage);

/* FRAME BUFFER */
static SN_STATUS sLoadWindow(const char* devicename, FB_Window_t* window);
static SN_STATUS sUpdateWindow(const FB_Window_t window, const FB_Image_t image);
static SN_STATUS sCleanWindow(const FB_Window_t window);
static SN_STATUS sDistroyWindow(FB_Window_t* window);

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
    sLoadWindow(WINDOW_NAME, &moduleImageViewer.window);

    /* Init Window */
    sCleanWindow(moduleImageViewer.window);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    char path[MAX_PATH_LENGTH] = {'\0', };
    int image_w = 0, image_h = 0;

    /* GET PRINT TARGET INFO */
    printInfo_t printInfo = SN_MODULE_FILE_SYSTEM_PrintInfoGet();
    if(!printInfo.isInit)
    {
         return SN_STATUS_NOT_INITIALIZED;
    }

    /* Load Image */
    sprintf(path,"%s%s%04d.png", printInfo.printTarget.tempFilePath, printInfo.printTarget.tempFileName, sliceIndex);
    sLoadImage(path, &moduleImageViewer.image);

    /* Display */
    sUpdateWindow(moduleImageViewer.window, moduleImageViewer.image);

    /* Distroy Image */
    sDistroyImage(&moduleImageViewer.image);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    sCleanWindow(moduleImageViewer.window);

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    sDistroyImage(&moduleImageViewer.image);
    sDistroyWindow(&moduleImageViewer.window);

    return retStatus;
}

#endif
#else
SN_STATUS SN_MODULE_IMAGE_VIEWER_UPDATE(uint32_t sliceIndex)
{
    SN_STATUS retStatus = SN_STATUS_OK;
    return retStatus;
}
SN_STATUS SN_MODULE_IMAGE_VIEWER_Init(void)
{
    return SN_STATUS_OK;
}
SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
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

    char header[8];    // 8 is the maximum size that can be checked

    png_byte color_type;
    png_byte bit_depth;

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
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

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
    if(color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA || transparent != 0)
    {
        unsigned char* alpha_buffer = (unsigned char *)malloc(pImage->w * pImage->h);
        unsigned char* aptr;

        row_pointer = (unsigned char*)malloc((pImage->w) * 4);
        rptr[0] = (png_bytep) row_pointer;

        pImage->alpha = alpha_buffer;

        for (pass = 0; pass < number_of_passes; pass++)
        {
            fbptr = pImage->rgb;
            aptr = alpha_buffer;

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

    //  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    //  for (y=0; y<height; y++)
    //          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    //  png_read_image(png_ptr, row_pointers);

    fclose(fp);

    return retStatus;
}

static SN_STATUS sDistroyImage(FB_Image_t* image)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    if(image == NULL)
    {
        return SN_STATUS_INVALID_PARAM;
    }

    free(image->alpha);
    free(image->rgb);

    image->h = 0;
    image->w = 0;

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

    // Figure out the size of the screen in bytes
    window->screenSize = (window->w * window->h) * (window->bpp / 8);
    //x_stride = (vinfo.line_length * 8) / finfo.bits_per_pixel;

    printf("Image Viwer => Module => Window Resolution [ %d x %d ], %dbpp\n", window->w, window->h, window->bpp);

    close(fbfd);

    return retStatus;
}

static SN_STATUS sUpdateWindow(const FB_Window_t window, const FB_Image_t image)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int fbfd = 0;
    long int screensize = 0;
    char *fbp = 0;

    unsigned long x_stride;
    int bp = 0;
    unsigned short* fbbuff = NULL;

    unsigned char*  fbptr = NULL;
    unsigned char*  imptr = NULL;

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

    fbptr = fbp;
    imptr = (unsigned char* )fbbuff;

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
        if(window.bpp == 32)
        {
            memset(fbp + ((i * window.w) * 4), 0, window.w * 4);
        }
        else //assume 16bpp
        {
            memset(fbp + ((i * window.w) * 2), 0, window.w * 2);
        }
        //else 8bpp
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
