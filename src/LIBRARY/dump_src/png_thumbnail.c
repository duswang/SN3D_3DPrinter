#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <png.h>
#include <zlib.h>
#include <string.h>



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

//  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
//  for (y=0; y<height; y++)
//          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

//  png_read_image(png_ptr, row_pointers);

  fclose(fp);
}

unsigned char* make_thumbnail(const unsigned char* buffer, int width, int height, int target_width)
{
  int target_height = 0;
  unsigned char* thumbnail = NULL;
  int i = 0;
  int j = 0;

  unsigned char * target_pixel = NULL;

  target_height = target_width * height / width;

  thumbnail = (unsigned char*)malloc(target_width * target_height * sizeof(unsigned char));

  if (thumbnail == NULL)
  {
    printf("Failed to allocate thumbnail buffer.\n");
  }

  int target_pixel_offset = 0;
  int source_pixel_offset = 0;

  for(j = 0; j < target_height; j++)
  {
    for (i = 0; i < target_width; i++)
    {
      target_pixel_offset = (j * target_width) + i;
      target_pixel = thumbnail + target_pixel_offset;
      source_pixel_offset = (j * width * (width/target_width) * 3) + (i * (width/target_width) * 3); // * 3 for RGB
      // get first color channel of the rgb image buffer
      if(buffer[source_pixel_offset] > 127)
      {
        *target_pixel = 1;
      }
      else
      {
        *target_pixel = 0;
      }
      printf("%d", *target_pixel);
    }
    printf("\n");
  }
  printf("\n\n\n");

  printf("BW Thumbnail\n");

  for (i=0; i<target_height; i++)
  {
    for (j=0; j<target_width; j++)
    {
      printf("%d", thumbnail[i * target_width + j]);
    }
    printf("\n");
  }
  printf("\n\n\n");

  return thumbnail;
}

int main(int argc, const char **argv)
{
  int x = 0;
  int y = 0;

  int width = 0, height = 0;
  unsigned char * image = NULL;
  unsigned char * alpha = NULL;

  int target_width = 40;

  unsigned char * resized_thumbnail = NULL;

  read_png(argv[1], &image, &alpha, &width, &height);

  resized_thumbnail = make_thumbnail(image, width, height, target_width);

   // display decoded rgb buffer to framebuffer
  //display_to_fb("/dev/fb0", image, alpha, width, height);

  if (image)
  {
    free(image);
  }

  if (alpha)
  {
    free(alpha);
  }

  free(resized_thumbnail);

  return 0;
}
