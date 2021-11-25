// Load image using the stb_image library, convert the image to gray and sepia, write it back to disk
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <sys/time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int handleGray(unsigned char *pg, unsigned char *pgg, int ID, int iterations)
{   
    int start, end;      
    start = (iterations/omp_get_num_threads()) * ID;
    end = (iterations/omp_get_num_threads()) * (ID+1);
    //printf("Iterations: %d \n" ,iterations);
    //printf("Start: %d. ID: %d \n", start, ID);
    //printf("End: %d. ID: %d \n", end, ID);
    int i = start;
    do{
        *(pgg + (i)) = ((*(pg + (i * 3)) + *(pg + (i * 3) + 1) + *(pg + (i * 3) + 2))/3.0);
        i++;
    }while(i < end);
    return 0;
}

int handleSobel(int ID, int iterations, unsigned char *gray_img, unsigned char *sobel_img, int width, int height)
{   
    int start, end;      
    start = (iterations/omp_get_num_threads()) * ID;
    end = (iterations/omp_get_num_threads()) * (ID+1);

    unsigned char *p = gray_img + start;
    unsigned char *pg = sobel_img + start;
    // printf("Iterations: %d \n" ,iterations);
    // printf("Start: %d. ID: %d \n", start, ID);
    // printf("End: %d. ID: %d \n", end, ID);
    // printf("*** Gray image comienza en %d\n", gray_img);
    // printf("Empiezo en el pixel gris %d, y en el pixel sobel %d, ID: %d\n", p, pg, ID);
    long long i = start;
    do{  
        pg++;
        p++;
        i++;
        int conv[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
        size_t pos = p - gray_img;
        if(pos % width == 0 || pos % (width-1) == 0 || (pos/width) % (height-1) == 0) { 
            *pg = 0;
            continue;
        }
        int new_val = *(p-width-1) * *(conv) + *(p-width) * *(conv+1) + *(p-width+1) * *(conv+2) + 
            *(p-1) * *(conv+3) + *p * *(conv+4) + *(p+1) * *(conv+5) + 
            *(p+width-1) * *(conv+6) + *(p+width) * *(conv+7) + *(p+width+1) * *(conv+8) ;
        *(pg) = new_val < 0 ? 0 : new_val;
        
    }while(i < end);
    //printf("El ultimo pixel de gray fue: %d, y el de sobel fue %d, ID:%d\n", p, pg, ID);
    return 0;
}

int main(int argc, char** argv) {
    /*Declaración de variables para medir el tiempo de procesamiento y escibirlo en
    un archivo de texto plano*/
    double begin = omp_get_wtime();
    int width, height, channels;
    
    if(argc != 4){
        printf("beware of usage: ./sobel_image input_image output_image num_threads \n");
        return 0;

    }
    //Asignamos los nombres de input y output para las imágenes en variables
    char* input = argv[1];
    char* output = argv[2];
    int thread_count = atoi(argv[3]);
    //printf("Num threads: %d\n ",thread_count);
    
    //Cargamos la imagen con el nombre de input ingresado
    unsigned char *img = stbi_load(input, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    //printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

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
    
    //Hacemos de forma paralela la conversión a escala de grises de la imagen
    unsigned char *pi = img;
    unsigned char *pgi = gray_img;    
    #pragma omp parallel num_threads(thread_count)
    {
        int ID = omp_get_thread_num();
        handleGray(pi, pgi, ID, gray_img_size);
    }
    stbi_write_png(output, width, height, gray_channels, gray_img, width * gray_channels);

    //Utilizamos memoria dinámica para el número de bits de la imagen que tendrá el filtro aplicado
    unsigned char *sobel_image = malloc(gray_img_size);     
    //Definimos el kernel del filtro
    
    int conv[] = {-1,-1,-1,-1,8,-1,-1,-1,-1};
    #pragma omp parallel num_threads(thread_count)
    {
        int ID = omp_get_thread_num();
        handleSobel(ID, gray_img_size, gray_img, sobel_image, width, height);
    }
    
    stbi_write_png(output, width, height, gray_channels, sobel_image, width * gray_channels);    
    double end = omp_get_wtime();
    
    //Liberamos las memorias dinámicas
    stbi_image_free(img);
    free(gray_img);
    free(sobel_image);

    //Calculamos el tiempo de ejecuación del programa
    double time_spent = (double)(end- begin);   
    
    //Imprimimos la información 
    printf("The execution time for %s with resolution: %dx%d and %d threads was: %f \n", input, width, height, thread_count, time_spent);
}