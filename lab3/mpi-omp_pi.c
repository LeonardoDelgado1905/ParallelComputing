//mpicc mpi-omp_pi.c -o mpi-omp_pi -lm -fopenmp
// mpirun -np 4 --hostfile mpi-hosts ./mpi-omp_pi
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>

#define SIZE 2e09
#define MAXTHREADS 32

int gray_img(double *pi, int size, int ID, int numprocs)
{   int start, end;
    start = (size/numprocs) * ID;
    end = (size/numprocs) * (ID + 1);
    int i = start;

    if(height < (index + 1)*(height/totalThreads)){
        end = height - 1;
    }
    int i = start;
    do{       
        for(int j = 0; j < width; j++){
            unsigned char *dir_gray_pixel = gray_img + i*width + j;
            unsigned char *dir_ch_pixel = img + 3*(i*width + j);
            *dir_gray_pixel = (uint8_t)((*dir_ch_pixel + *(dir_ch_pixel + 1) + *(dir_ch_pixel + 2))/3.0);
            //printf("i: %d, j: %d", i, j);
        }
        i++;
    }while(i <= end);
}



int main(int argc, char *argv[])
{
	int done = 0, n, processId, numprocs, I, rc, i;
	double PI25DT = 3.141592653589793238462643;
	double local_pi[MAXTHREADS], global_pi;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
  if (processId == 0) printf("\nLaunching with %i processes", numprocs);
  global_pi = 0.0;

  #pragma omp parallel num_threads(4)
  {
    int threadId = omp_get_thread_num();
    int threadsTotal = omp_get_num_threads();
    int globalId = (processId * threadsTotal) + threadId;
    calculatePi(&local_pi[threadId], globalId, threadsTotal*numprocs);
    #pragma omp single
    {
      for(i = 0; i < threadsTotal; i++)
        global_pi = global_pi + local_pi[i];
    }
    printf("%i ", globalId); fflush(stdout);
  }
  MPI_Reduce(local_pi, &global_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (processId == 0) printf("\npi is approximately %.16f, Error is %.16f\n", global_pi, fabs(global_pi - PI25DT));
	MPI_Finalize();
	return 0;
}
