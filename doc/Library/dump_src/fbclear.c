#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <png.h>
#include <zlib.h>

void clearscreen(char * pFramebuffer, int screenwidth, int screenheight, int bpp)
{
  int i = 0;
  int j = 0;
  long int location = 0;

  // Figure out where in memory to put the pixel
  for (i = 0; i < screenheight; i++)
  {
    for (j = 0; j < screenwidth; j++)
    {

        location = (j * (bpp/8)) +
                   (i * (screenwidth * (bpp / 8)));

        if (bpp == 32)
        {
            *(pFramebuffer + location) = 0;        // black
            *(pFramebuffer + location + 1) = 0;
            *(pFramebuffer + location + 2) = 0;
            *(pFramebuffer + location + 3) = 0;
    //location += 4;
        }
        else
        { //assume 16bpp
            int b = 0;    //black
            int g = 0;
            int r = 0;
            unsigned short int t = r<<11 | g << 5 | b;
            *((unsigned short int*)(pFramebuffer + location)) = t;
        }
    }
  }
}

int main(int argc, const char **argv)
{
    png_image image;
    memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
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

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    clearscreen(fbp, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
