// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(void) {
    int width, height, channels;
    //unsigned char *img = stbi_load("sky.jpg", &width, &height, &channels, 0);
    unsigned char *img = stbi_load("./bicho.jpg", &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    // Convert the input image to gray
    size_t img_size = width * height * channels;
    int gray_channels = 1;
    size_t gray_img_size = width * height * gray_channels;

    unsigned char *gray_img = malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }
     stbi_write_png("./bicho_gris.png", width, height, gray_channels, gray_img, width * gray_channels);
     unsigned char *sobel_image = malloc(gray_img_size);
     
     //Arreglo convolucion = [0,1,0,1,-4,1,0,1,0]
     //                                ^
     //    
     int conv[] = {0,1,0,1,-4,1,0,1,0};

     for(unsigned char *p = gray_img + width + 1, *pg = sobel_image + width + 1; p != gray_img + gray_img_size - width - 1; p++, pg++) {
          size_t pos = p - gray_img;
          if(pos % width == 0 || pos % (width-1) == 0){
               *pg = 0;
               continue;
          }

          int new_val = *(p-width) * *(conv+1) + 
                *(p-1) * *(conv+3) + *p * *(conv+4) + *(p+1) * *(conv+5) + 
                *(p+width) * *(conv+7);

         *pg = new_val < 0 ? 0 : new_val;
          
        /*if(pos == width + 1){
            printf("arriba. %d", *(p-width));
            printf("abajo. %d", *(p+width));
            printf("centro. %d", *p);
            printf("derecha. %d", *(p-1));
            printf("izquierda. %d", *(p+1));
            printf("pg: %d\n", *pg);
        }*/
         
     }

    // stbi_write_jpg("sky_gray.jpg", width, height, gray_channels, gray_img, 100);
    stbi_write_png("./bicho_borde.png", width, height, gray_channels, sobel_image, width * gray_channels);
    
    stbi_image_free(img);
    free(gray_img);
}
