#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <png.h>
#include <zlib.h>

void read_png(const char* filename, unsigned char **buffer, unsigned char **alpha, int* width, int * height)
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
  printf("%d\n", __LINE__);

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

void display_to_fb(const char* devicename, unsigned char * rgb, unsigned char* alpha, int w, int h)
{
  int fbfd = 0;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;

  unsigned long x_stride;
  int bp = 0;
  unsigned short *fbbuff = NULL;

  unsigned char* fbptr;
  unsigned char* imptr;

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

  fbbuff = convertRGB2FB(fbfd, rgb, w * h, vinfo.bits_per_pixel, &bp);

  // Map the device to memory
  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
  if ((int)fbp == -1) {
      perror("Error: failed to map framebuffer device to memory");
      exit(4);
  }
  printf("The framebuffer device was mapped to memory successfully.\n");

  fbptr = fbp;
  imptr = fbbuff;

  for (int i = 0; i<h; i++, fbptr += w * bp, imptr += w * bp)
  {
    memcpy(fbptr, imptr, w * bp);
  }

  munmap(fbp, screensize);
  close(fbfd);
  return;
}

int main(int argc, const char **argv)
{
  int x = 0;
  int y = 0;

  int width = 0, height = 0;
  unsigned char * image = NULL;
  unsigned char * alpha = NULL;

  read_png(argv[1], &image, &alpha, &width, &height);

  // display decoded rgb buffer to framebuffer
  display_to_fb("/dev/fb0", image, alpha, width, height);


  return 0;
}
