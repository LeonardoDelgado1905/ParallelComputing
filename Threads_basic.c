#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
 
#define XDIM  1024
#define YDIM  1024
#define MATRIXSIZE  XDIM*YDIM
 
 
/*To measure response time*/ 
 long int tiempo1, tiempo2;
 float ResponseTime;
 
/*To measure response time*/
typedef long long timestamp_t;
static timestamp_t get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return now.tv_usec + (timestamp_t)now.tv_sec *1000000 ;
}
 
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
 
/*****************************************************************************/
 
int multMatrix(int *A, int *B, int *C)
{
    int i, x, y;
    int yOffset, xOffset;
    
    for(y = 0; y < YDIM; y++)
    {   yOffset = y * XDIM;
        for(x = 0; x < XDIM; x++)
        {   
            for(i = 0; i < XDIM; i++){
                *(C + yOffset + x) = *(C + yOffset + x) + (*(A + yOffset + i) * (*(B + (i*YDIM) + x )));
            }
        }
    }
}
 
/*****************************************************************************/
 
int main()
{
    int *A, *B, *C;
    int i;
    
    A = malloc(MATRIXSIZE*sizeof(int));
    B = malloc(MATRIXSIZE*sizeof(int));
    C = malloc(MATRIXSIZE*sizeof(int));
    
    for(i = 0; i < MATRIXSIZE; i++){
        *(A + i) = rand() & 0xF;
        *(B + i) = rand() & 0xF;        
        *(C + i) = 0;
    }
    
    tiempo1 = get_timestamp();
    multMatrix(A, B, C);
    tiempo2 = get_timestamp();
    ResponseTime = tiempo2 - tiempo1;
    printf("\n tiempo = %f", ResponseTime/1000000);  
    
    //printMatrix(A);
    //printMatrix(B);
    //printMatrix(C);
 
    free(A);
    free(B);
    free(C);          
}