#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

#define MAX_TERMS 1e9

int main(int argc, char** argv)
{
    if(argc != 2){
        printf("usage: ./pi thread count");
        return 0;
    }
    int thread_count = atoi(argv[1]);
    double result = 0;
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);

    #pragma omp parallel for num_threads(thread_count) reduction(+:result)
    for(int i = 0; i < (long) MAX_TERMS; ++i){
        result += 4.0 * (i % 2 == 0 ? 1 : -1) / (2.0 * i + 1);
    }
    
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    
    printf("\npi: %2.12f \n", result);
}
