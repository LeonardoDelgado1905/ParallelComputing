// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc, char** argv) {
    /*Declaración de variables para medir el tiempo de procesamiento y escibirlo en
    un archivo de texto plano*/
    FILE *fp;
    //clock_t begin = clock();
    //double time_spent = 0.0;    
    int width, height, channels;
    
    
    if(argc != 3){
        printf("beware of usage: ./sobel_image input_image output_image \n");
        return 0;

    }
    /**/
    //Asignamos los nombres de input y output para las imágenes en variables
    char* input = argv[1];
    char* output = argv[2];

    //Cargamos la imagen con el nombre de input ingresado
    unsigned char *img = stbi_load(input, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    

    //Creamos variables para convertir la imagen de input a tonalidades de gris
    int gray_channels = channels == 4 ? 2 : 1;
    size_t img_size = width * height * channels;
    size_t gray_img_size = width * height * gray_channels;
    //Utilizamos memoria dinámica para el número de bits de la imagen en gris
    unsigned char *gray_img = malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
    
    //Convertimos la imagen de input, bit por bit, a tonalidades de gris
    for(int i = 0; i < height * gray_channels; i += gray_channels){
        for(int j = 0; j < width * gray_channels; j += gray_channels){
            unsigned char *dir_gray_pixel = gray_img + i*width + j;
            unsigned char *dir_ch_pixel = img + channels*(i*width + j);
            *dir_gray_pixel = (uint8_t)((*dir_ch_pixel + *(dir_ch_pixel + 1) + *(dir_ch_pixel + 2))/3.0);
            if(channels == 4) *(dir_gray_pixel+1) = *(dir_ch_pixel+3);
            //printf("i: %d, j: %d", i, j);
        }
    }

    //fflush(stdout);
    //Utilizamos memoria dinámica para el número de bits de la imagen Sobel
    
    unsigned char *sobel_img = malloc(gray_img_size);
    if(sobel_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    //int conv[] = {-1,-1,-1,-1,9,-1,-1,-1,-1};
    int conv[3][3] = {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}    
    };
    //Sobel
    for(int i = 0; i < height*gray_channels; i+=gray_channels){
        for(int j = 0; j < width*gray_channels; j+=gray_channels){
            unsigned char *dir_sobel_pixel = sobel_img + i*width + j;
            unsigned char *dir_gray_pixel = gray_img + i*width + j;   
            int sum = 0;
            if(i == 0 || i == (height-1) * gray_channels || j == 0 || j == (width - 1) * gray_channels){
                *dir_sobel_pixel = (uint8_t)sum;
            }else{
                for(int k = -1; k <= 1; ++k){
                    for(int l = -1; l <= 1; ++l){
                        unsigned char *dir_n_pixel = dir_gray_pixel + k*width + l;
                        sum += *dir_n_pixel * conv[k+1][l+1];
                        if(i == 354 && j == 356){
                            printf("dir_n_pixel: %d \n", *dir_n_pixel);
                            printf("conv: %d \n", conv[k+1][l+1]);
                            printf("sum: %d \n", sum);
                        }
                    }
                }
                sum = sum < 0 ? 0 : sum;
                *dir_sobel_pixel = (uint8_t)sum;
            }
        }
    }

    //Nombramos la imagen con el filtro aplicado con el nombre que el usuario ha ingresado
    stbi_write_png("sobel.png", width, height, gray_channels, sobel_img, width * gray_channels);
    return 0;
}