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
    clock_t begin = clock();
    double time_spent = 0.0;    
    int width, height, channels;
    
    if(argc != 3){
        printf("beware of usage: ./sobel_image input_image output_image \n");
        return 0;

    }
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
    int gray_channels = 1;
    size_t img_size = width * height * channels;
    size_t gray_img_size = width * height * gray_channels;

    //Utilizamos memoria dinámica para el número de bits de la imagen en gris
    unsigned char *gray_img = malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }

    //Convertimos la imagen de input, bit por bit, a tonalidades de gris
    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }
    
    //Utilizamos memoria dinámica para el número de bits de la imagen que tendrá el filtro aplicado
    unsigned char *sobel_image = malloc(gray_img_size);     
    //Definimos el kernel del filtro
    int conv[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    //int conv[] = {1,-2,1,-2,5,-2,1,-2,1}; //sharpen
    //int conv[] = {1,1,1,1,-2,1,-1,-1,-1}; //Norte
    //int conv[] = {-1,1,1,-1,-2,1,-1,1,1}; // Este
    //int conv[] = {-1-1,0,-1,0,1,0,1,1}; //Estampado en relieve
    //Aplicamos el kernel a la imagen en tonalidades de gris
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
    //Nombramos la imagen con el filtro aplicado con el nombre que el usuario ha ingresado
    if (strstr(output, ".png") != NULL) {
        stbi_write_png(output, width, height, gray_channels, sobel_image, width * gray_channels);
    }else{
        stbi_write_jpg(output, width, height, gray_channels, sobel_image, 100);
    }
    
    //Liberamos las memorias dinámicas
    stbi_image_free(img);
    free(gray_img);
    free(sobel_image);

    clock_t end = clock();
    
    //Calculamos el tiempo de ejecuación del programa
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    char message[80], resolution[10], time[80];
    snprintf(time, 80, "%f", time_spent);
    strcpy(message, "The execution time for ");
    strcat(message, input);
    strcat(message, " (with resolution: ");
    snprintf(resolution, 10, "%dx%d", width, height);
    strcat(message, resolution);
    strcat(message, ") was: ");
    strcat(message, time);
    strcat(message, " \n");

    //Escribimos el tiempo de ejecución del programa en un archivo de texto plano cuyo nombre será "times.txt"
    fp = fopen("times.txt", "a");      
    fputs(message, fp);
    fclose(fp);
}