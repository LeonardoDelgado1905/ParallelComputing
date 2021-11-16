// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include <unistd.h> 

double sobelFunction(char *inputName, char *outputName);

int main() {

    char c[50] = "720p", c1[50] = "1080p", c2[50] = "4k";
    double first, second, third; 
    first = sobelFunction("720p.jpg", "720psobel.jpg");
    second = sobelFunction("1080p.jpg", "1080psobel.jpg");
    third = sobelFunction("1p.jpg", "1psobel.jpg");
    third = sobelFunction("2p.jpg", "2psobel.jpg");
    third = sobelFunction("3p.jpg", "3psobel.jpg");
    third = sobelFunction("4k.jpg", "4ksobel.jpg");
    third = sobelFunction("7p.jpg", "7psobel.jpg");

    return 0;

}

double sobelFunction(char *inputName, char *outputName){
    
    clock_t begin = clock();
    double time_spent = 0.0;    
    int width, height, channels;
    char* input = inputName;
    char* output = outputName;

    unsigned char *img = stbi_load(input, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

    // Convert the input image to gray
    int gray_channels = 1;
    size_t img_size = width * height * channels;
    size_t gray_img_size = width * height * gray_channels;

    unsigned char *gray_img = malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }
    //stbi_write_png("./bicho_gris.png", width, height, gray_channels, gray_img, width * gray_channels);
     
    unsigned char *sobel_image = malloc(gray_img_size);     
    int conv[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    
    for(unsigned char *p = gray_img + width + 1, *pg = sobel_image + width + 1; p != gray_img + gray_img_size - width - 1; p++, pg++) {
        size_t pos = p - gray_img;
        if(pos % width == 0 || pos % (width-1) == 0) { 
            *pg = 0;
            continue;
        }
        int new_val = *(p-width-1) * *(conv) + *(p-width) * *(conv+1) + *(p-width+1) * *(conv+0) + 
            *(p-1) * *(conv+3) + *p * *(conv+4) + *(p+1) * *(conv+5) + 
            *(p+width-1) * *(conv+6) + *(p+width) * *(conv+7) + *(p+width+1) * *(conv+8) ;
        *pg = new_val < 0 ? 0 : new_val;
        
    }
    if (strstr(output, ".png") != NULL) {
        stbi_write_png(output, width, height, gray_channels, sobel_image, width * gray_channels);
    }else{
        stbi_write_jpg(output, width, height, gray_channels, sobel_image, 100);
    }    
    
    stbi_image_free(img);
    free(gray_img);
    free(sobel_image);

    clock_t end = clock();
 
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("The elapsed time was %f seconds \n", time_spent);

    return time_spent;
}