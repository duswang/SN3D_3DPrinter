/**
 * @file SN_MODULE_IMAGE_VIEWER.c
 * @author Bato
 * @date 1 Sep 2018
 * @brief
 *
 * @see http://www.stack.nl/~dimitri/doxygen/docblocks.html
 * @see http://www.stack.nl/~dimitri/doxygen/commands.html
 *
 * @todo direct contorl framebuffer?
 * @bug 2048 x 2048 Over Resolution it can't make Window(surface)
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
    unsigned char **buffer;
    unsigned char **alpha;

    uint32_t        w;
    uint32_t        h;
} FB_Image_t;

typedef struct frameBuffer_Window
{
    char*    name;

    uint32_t w;
    uint32_t h;
} FB_Window_t;

typedef struct image_viewer
{
    machineInfo_t   machineInfo;
    printInfo_t     printInfo;

    FB_Window_t window;
    FB_Image_t image;
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
static void sLoadImage(const char* filename, unsigned char **buffer, unsigned char **alpha, int* width, int* height);

/* *** UTIL *** */
inline static unsigned char make8color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make15color(unsigned char r, unsigned char g, unsigned char b);
inline static unsigned short make16color(unsigned char r, unsigned char g, unsigned char b);
void* convertRGB2FB(int fh, unsigned char *rgbbuff, unsigned long count, int bpp, int *cpp);

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
    moduleImageViewer.window = SDL_CreateWindow("SN3D", \
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

#if(IMAGE_VIEWER_FIX_RESOLUTION)
    printf("Image Viwer => Module => Window Resolution [ %d x %d ]\n", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
#endif

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

     /* Load Texture */
     sprintf(path,"%s%s%04d.png", printInfo.printTarget.tempFilePath, printInfo.printTarget.tempFileName, sliceIndex);


    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_CLEAR(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    return retStatus;
}

SN_STATUS SN_MODULE_IMAGE_VIEWER_Destroy(void)
{
    SN_STATUS retStatus = SN_STATUS_OK;

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
static void sLoadImage(const char* filename, unsigned char **buffer, unsigned char **alpha, int* width, int* height)
{
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


      /* open file and test for it being a png */
      FILE *fp = fopen(filename, "rb");
      if (!fp)
      {
        printf("[read_png_file] File %s could not be opened for reading", filename);
        exit(-1);
      }
      fread(header, 1, 8, fp);
      if (png_sig_cmp(header, 0, 8))
      {
        printf("[read_png] File %s is not recognized as a PNG file\n", filename);
        exit(-1);
      }

      /* initialize stuff */
      png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

      if (!png_ptr)
      {
        printf("[read_png] png_create_read_struct failed\n");
        exit(-1);
      }

      info_ptr = png_create_info_struct(png_ptr);
      if (!info_ptr)
      {
        printf("[read_png] png_create_info_struct failed\n");
        exit(-1);

      }

      if (setjmp(png_jmpbuf(png_ptr)))
      {
        printf("[read_png] Error during init_io\n");
        exit(-1);

      }

      png_init_io(png_ptr, fp);
      png_set_sig_bytes(png_ptr, 8);

      png_read_info(png_ptr, info_ptr);

      *width = png_get_image_width(png_ptr, info_ptr);
      *height = png_get_image_height(png_ptr, info_ptr);
      color_type = png_get_color_type(png_ptr, info_ptr);
      bit_depth = png_get_bit_depth(png_ptr, info_ptr);

      number_of_passes = png_set_interlace_handling(png_ptr);
      png_read_update_info(png_ptr, info_ptr);

      printf("[readpng] w = %d, h = %d, %d bpp\n", *width, *height, bit_depth);

      /* read file */
      if (setjmp(png_jmpbuf(png_ptr)))
      {
        printf("[read_png_file] Error during read_image\n");
        exit(-1);
      }

    printf("%d\n", __LINE__);
      if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
      {
        transparent = 1;
        png_set_tRNS_to_alpha(png_ptr);
      }

      *buffer = (unsigned char*)malloc((*width) * (*height) * 3); //RGB 888
      if (*buffer == NULL)
      {
        printf("[read_png]out of memory: failed to allocate image rgb buffer\n");
        exit(-1);
      }

      if (color_type == PNG_COLOR_TYPE_RGB_ALPHA || color_type == PNG_COLOR_TYPE_GRAY_ALPHA || transparent != 0)
      {
        unsigned char* alpha_buffer = (unsigned char *)malloc((*width) * (*height));
        unsigned char* aptr;

        row_pointer = (unsigned char*)malloc((*width) * 4);
        rptr[0] = (png_bytep) row_pointer;

        *alpha = alpha_buffer;

        for (pass = 0; pass < number_of_passes; pass++)
        {
          fbptr = *buffer;
          aptr = alpha_buffer;

          for (i = 0; i<(*height); i++)
          {
            int n;
            unsigned char *trp = row_pointer;

            png_read_rows(png_ptr, rptr, NULL, 1);

            for(n = 0; n < (*width); n++, fbptr+=3, trp+=4)
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
          fbptr = *buffer;
          for(i = 0; i< (*height); i++, fbptr += *width * 3)
          {
            rptr[0] = (png_bytep)fbptr;
            png_read_rows(png_ptr, rptr, NULL, 1);
          }
        }
      }
      png_read_end(png_ptr, info_ptr);
      png_destroy_read_struct(&png_ptr, &info_ptr, (NULL));
      printf("%d\n", __LINE__);

    //  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    //  for (y=0; y<height; y++)
    //          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    //  png_read_image(png_ptr, row_pointers);

      fclose(fp);
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

SN_STATUS sGetDisplayMode(const char* deviceName)
{
    SN_STATUS retStatus = SN_STATUS_OK;

    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    long int screenSize = 0;
    char *fbp = NULL;






    return retStatus;
}

void display_to_fb(const char* devicename, unsigned char * rgb, unsigned char* alpha, int w, int h)
{
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;

  unsigned long x_stride;
  int bp = 0;
  unsigned short* fbbuff = NULL;

  unsigned char*  fbptr = NULL;
  unsigned char*  imptr = NULL;

  // Open the file for reading and writing
  fbfd = open(devicename, O_RDWR);
  if (fbfd == -1) {
      perror("Error: cannot open framebuffer device");
      exit(1);
  }
  printf("The framebuffer device was opened successfully.\n");

  // Get fixed screen information
  if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
      perror("Error reading fixed information");
      exit(2);
  }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
      perror("Error reading variable information");
      exit(3);
  }

  printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

  // Figure out the size of the screen in bytes
  screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
  //x_stride = (vinfo.line_length * 8) / finfo.bits_per_pixel;

  //////////////////////////////////////////////////////////////////////////////////

  fbbuff = convertRGB2FB(fbfd, rgb, w * h, vinfo.bits_per_pixel, &bp);

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((int)fbp == -1) {
      perror("Error: failed to map framebuffer device to memory");
      exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");

  fbptr = fbp;
  imptr = (unsigned char* )fbbuff;

  for (int i = 0; i<h; i++, fbptr += w * bp, imptr += w * bp)
  {
    memcpy(fbptr, imptr, w * bp);
  }

  free(imptr);

  munmap(fbp, screensize);
  close(fbfd);
  return;
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
