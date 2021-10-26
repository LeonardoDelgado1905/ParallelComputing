#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DIM  64
#define XDIM  DIM
#define YDIM  DIM
#define MATRIXSIZE  XDIM*YDIM
#define NUM_HILOS DIM


/*****************************************************************************/
int printMatrix(int *ap)
{
	int x, y;
	for(y = 0; y < YDIM; y++)
	{
		printf("\n");
		for(x = 0; x < XDIM; x++)
		{
		    printf("%i \t", *(ap + (y*XDIM) + x));
		}
	}
	printf("\n");
}

int *A, *B, *C;
/*****************************************************************************/
void * multMatrix(void *idv)
{
	int *id, yOffset;
	id = idv;
    int i, x, y;
    y = *id;
    yOffset = y * XDIM;
    for(x = 0; x < XDIM; x++)
    {
        for(i = 0; i < XDIM; i++){
            *(C + yOffset + x) = *(C + yOffset + x) + (*(A + yOffset + i)* (*(B + (i*YDIM) + x )));
        }
    } 
}

/*****************************************************************************/
int main()
{
    int i, r, *rh0, id[NUM_HILOS];
    pthread_t hilo[NUM_HILOS];
        
    A = malloc(MATRIXSIZE*sizeof(int));
    B = malloc(MATRIXSIZE*sizeof(int));
    C = malloc(MATRIXSIZE*sizeof(int));
    
    for(i = 0; i < MATRIXSIZE; i++){
        *(A + i) = rand() & 0xF;
        *(B + i) = rand() & 0xF;        
        *(C + i) = 0;
    }
    
    for(i = 0; i < NUM_HILOS; i++)
    {	id[i] = i;
        r = pthread_create(&hilo[i],NULL,(void *)multMatrix,(void *)&id[i]);
        if(r != 0)
        {
            perror("\n-->No puedo crear hilo");
            exit(-1);
        }
    }
    
    for(i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilo[i], (void *) &rh0);
    }
      
    //printMatrix(A);
    //printMatrix(B);
    //printMatrix(C);
}


