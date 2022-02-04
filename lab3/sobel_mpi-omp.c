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
    printf("la image size es: %d\n", gray_img_size);
    printf("el id: %d, desde %d, hasta %d\n", ID, start, end);

    do{       
        unsigned char *dir_gray_pixel = gray_img + i;
        unsigned char *dir_ch_pixel = img + 3*i;
        *dir_gray_pixel = (int)((*dir_ch_pixel + *(dir_ch_pixel + 1) + *(dir_ch_pixel + 2))/3.0);
        i++;
    }while(i <= end);
}

int main(int argc, char *argv[])
{
	int done = 0, n, processId, numprocs, I, rc, i;
  int width, height, channels;

  char *input = "./720p/castle.jpg";
  char *output = "./720p/castle_gray.jpg";
  

  unsigned char *gray_img;
  //Creamos variables para convertir la imagen de input a tonalidades de gris
  

  printf("vamos\n");
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  if (processId == 0){
    printf("\nLaunching with %i processes", numprocs);
    unsigned char *img = stbi_load(input, &width, &height, &channels, 0);
    if(img == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);
    int gray_channels = channels == 4 ? 2 : 1;
    
    
    //Utilizamos memoria dinámica para el número de bits de la imagen en gris
     
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
  }
  MPI_Bcast(width, img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(height, img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(channels, img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  MPI_Bcast(img, img_size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
  size_t img_size = width * height * channels;
  size_t gray_img_size = width * height * gray_channels;
  gray_img = malloc(gray_img_size);
  
  #pragma omp parallel num_threads(4)
  {
    int threadId = omp_get_thread_num();
    int threadsTotal = omp_get_num_threads();
    int globalId = (processId * threadsTotal) + threadId;
    apply_gray_filter(gray_img, img, gray_img_size, globalId, threadsTotal*numprocs);
  }
  //MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

  
  //Nombramos la imagen con el filtro aplicado con el nombre que el usuario ha ingresado
  stbi_write_png(output, width, height, gray_channels, gray_img, width * gray_channels);
  
	return 0;
}
