//mpicc mpi-omp_pi.c -o mpi-omp_pi -lm -fopenmp
// mpirun -np 4 --hostfile mpi-hosts ./mpi-omp_pi
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
typedef unsigned char     uint8_t;
#define MAXTHREADS 32

int apply_gray_filter(unsigned char *gray_img, unsigned char *img, int gray_img_size, int ID, int numprocs)
{   int start, end;
    start = (gray_img_size/numprocs) * ID;
    end = (gray_img_size/numprocs) * (ID + 1);
    int i = start;
    //printf("el id: %d, desde %d, hasta %d\n", ID, start, end);

    do{       
        unsigned char *dir_gray_pixel = gray_img + i;
        unsigned char *dir_ch_pixel = img + 3*i;
        *dir_gray_pixel = (int)((*dir_ch_pixel + *(dir_ch_pixel + 1) + *(dir_ch_pixel + 2))/3.0);
        i++;
    }while(i <= end);
}


int apply_sobel_filter(unsigned char *gray_img, unsigned char *sobel_img, int width, int height, int gray_img_size, int ID, int numprocs)
{   int start, end;
    start = (gray_img_size/numprocs) * ID;
    end = (gray_img_size/numprocs) * (ID + 1);
    int i = start;
    //printf("el id: %d, desde %d, hasta %d\n", ID, start, end);
    int conv[3][3] = {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}    
    };

    do{  
      //for(int j = 0; j < width; j++){   
        unsigned char *dir_sobel_pixel = sobel_img + i;
        unsigned char *dir_gray_pixel = gray_img + i;   
        int sum = 0;
        if(i <= width +1 || i > (gray_img_size - width - 2) || (i % width == 0) || ((i - 1) % width == 0)){
            *dir_sobel_pixel = (uint8_t)sum;
        }else{
            for(int k = -1; k <= 1; ++k){
                for(int l = -1; l <= 1; ++l){
                    unsigned char *dir_n_pixel = dir_gray_pixel + k*width + l;
                    sum += *dir_n_pixel * conv[k+1][l+1];
                    if(i == 233){
                      printf("el valor de: %d es: %d", dir_n_pixel, *dir_n_pixel);
                    }
                }
            }
            sum = sum < 0 ? 0 : sum;
            *dir_sobel_pixel = (uint8_t)sum;
        }  
     //}
      i++;
    }while(i <= end);
}

int main(int argc, char *argv[])
{
	int done = 0, n, processId, numprocs, I, rc, i;
  int width, height, channels;

  char *input = "./4k/bicho.jpg";
  char *output = "./4k/bicho_sobel.jpg";

  //Creamos variables para convertir la imagen de input a tonalidades de gris
  unsigned char *img;
  unsigned char *gray_img;
  unsigned char *sobel_img;
  unsigned char *total_gray;
  unsigned char *total_sobel;



	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  
  if(processId == 0){
    img = stbi_load(input, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
  }


  MPI_Bcast(&width, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&height, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&channels, 1, MPI_INT, 0, MPI_COMM_WORLD);
  //MPI_Barrier(MPI_COMM_WORLD);
  printf("Yo, el proceso %d tengo una imagen de %dpx, a height of %dpx and %d channels\n", processId, width, height, channels);
  
  
  size_t img_size = width * height * channels;
  if(processId != 0){
    img = malloc(img_size);
  }
  MPI_Bcast(img, img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  //MPI_Barrier(MPI_COMM_WORLD);

  int gray_channels = channels == 4 ? 2 : 1;
  size_t gray_img_size = width * height * gray_channels;
  gray_img = malloc(gray_img_size);
  //Utilizamos memoria dinámica para el número de bits de la imagen en gris
    
  if(gray_img == NULL) {
      printf("Unable to allocate memory for the gray image.\n");
      exit(1);
  }
  //Utilizamos memoria dinámica para el número de bits de la imagen sobel
  sobel_img = malloc(gray_img_size);
  
  if(sobel_img == NULL) {
      printf("Unable to allocate memory for the sobel image.\n");
      exit(1);
  }    

  if (processId == 0){
    printf("\nLaunching with %i processes", numprocs);
  }
  
  
  double begin = omp_get_wtime();
  #pragma omp parallel num_threads(4)
  {
    int threadId = omp_get_thread_num();
    int threadsTotal = omp_get_num_threads();
    int globalId = (processId * threadsTotal) + threadId;
    apply_gray_filter(gray_img, img, gray_img_size, globalId, threadsTotal*numprocs);
  }
  //MPI_Barrier(MPI_COMM_WORLD);
  double end = omp_get_wtime();
  double time_spent = (double)(end- begin);  
  if(processId == 0){
    printf("\nThe execution time for %s with resolution: %dx%d and %d processes was: %f \n", input, width, height, numprocs, time_spent);
  }
  //printf("termina el proceso: %d\n", processId);
  int data = gray_img_size/numprocs;
  if(processId == 0){
    total_gray = malloc(data*numprocs);
  }
  MPI_Gather(gray_img + ((gray_img_size/numprocs) * processId), data, MPI_UNSIGNED_CHAR, total_gray, data, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  if(processId != 0){
    total_gray = malloc(gray_img_size);
  }
  MPI_Bcast(total_gray, gray_img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  
  #pragma omp parallel num_threads(4)
  {
    int threadId = omp_get_thread_num();
    int threadsTotal = omp_get_num_threads();
    int globalId = (processId * threadsTotal) + threadId;
    apply_sobel_filter(total_gray, sobel_img, width, height, gray_img_size, globalId, threadsTotal*numprocs);
  }
  
  if(processId == 0){
    total_sobel = malloc(data * numprocs);
  }
  
  MPI_Gather(sobel_img + ((gray_img_size/numprocs) * processId), data, MPI_UNSIGNED_CHAR, total_sobel, data, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);


  MPI_Finalize();

  
  //Nombramos la imagen con el filtro aplicado con el nombre que el usuario ha ingresado
  if(processId == 0){
    stbi_write_png(output, width, height, gray_channels, total_sobel, width * gray_channels);
  }
  
	return 0;
}
