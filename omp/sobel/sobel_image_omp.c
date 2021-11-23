// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

#define PAD 8

int handleRow(unsigned char *p, unsigned char *pg, int num_fila, int width)
{   
    int start, end;      
    start = width * num_fila;
    end = start + width;
    for(int i = start; i < end; ++i){
        *(pg + i) = (uint8_t)((*(p + (i * 3)) + *(p + (i * 3) + 1) + *(p + (i * 3) + 2))/3.0);
    }
    return 0;
}

int main(int argc, char** argv) {
    /*Declaración de variables para medir el tiempo de procesamiento y escibirlo en
    un archivo de texto plano*/
    FILE *fp;
    clock_t begin = clock();
    double time_spent = 0.0;    
    int width, height, channels;
    
    if(argc != 4){
        printf("beware of usage: ./sobel_image input_image output_image num_threads \n");
        return 0;

    }
    //Asignamos los nombres de input y output para las imágenes en variables
    char* input = argv[1];
    char* output = argv[2];
    int thread_count = atoi(argv[3]);
    printf("Num threads: %d\n ",thread_count);
    
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
    
    unsigned char *p = img;
    unsigned char *pg = gray_img;
    #pragma omp parallel for num_threads(thread_count) schedule(dynamic)
    for(int i = 0; i < (long) height; ++i) {
        handleRow(p, pg, i, width);
    }

    /*#pragma omp parallel for num_threads(thread_count)
    for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    }*/
    //stbi_write_png(output, width, height, gray_channels, gray_img, width * gray_channels);

    //Utilizamos memoria dinámica para el número de bits de la imagen que tendrá el filtro aplicado
    unsigned char *sobel_image = malloc(gray_img_size);     
    //Definimos el kernel del filtro
    int conv[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    
    //Aplicamos el kernel a la imagen en tonalidades de gris
    p = gray_img;
    pg = sobel_image;
    #pragma omp parallel for num_threads(thread_count) schedule(dynamic)
    for(int i = width+1; i < gray_img_size - width - 1; i++) {
        p = gray_img + i;
        size_t pos = p - gray_img;
        if(pos % width == 0 || pos % (width-1) == 0) { 
            *pg = 0;
            continue;
        }
        int new_val = *(p-width-1) * *(conv) + *(p-width) * *(conv+1) + *(p-width+1) * *(conv+0) + 
            *(p-1) * *(conv+3) + *p * *(conv+4) + *(p+1) * *(conv+5) + 
            *(p+width-1) * *(conv+6) + *(p+width) * *(conv+7) + *(p+width+1) * *(conv+8) ;
        *(pg + i) = new_val < 0 ? 0 : new_val;
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

    char message[100], resolution[10], threads[17], time[80];
    
    strcpy(message, "The execution time for ");
    strcat(message, input);
    snprintf(threads, 17, " with %d threads ", thread_count);
    strcat(message, threads);
    strcat(message, " and resolution:");
    snprintf(resolution, 10, "%dx%d", width, height);
    strcat(message, resolution);
    snprintf(time, 24, "%f", time_spent);
    strcat(message, " was: ");
    strcat(message, time);
    strcat(message, " \n");

    printf("%s", time);

    //Escribimos el tiempo de ejecución del programa en un archivo de texto plano cuyo nombre será "times.txt"
    fp = fopen("times.txt", "a");      
    fputs(message, fp);
    fclose(fp);
}