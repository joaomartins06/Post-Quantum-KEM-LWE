#include "sampler.h"

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#define M_PI 3.1415926535897932384626433

int64_t sample_uniform(int32_t q){
    //this one is pretty straightforward
    int64_t result = rand() % q;
    return result;
}


int64_t sample_error(double sigma){
    //using Box-Muller transform to sample from a normal distribution
    //this ensures that u1 and u2 are in the range (0,1), avoiding log(0)    
    double u1 = (rand() + 1.0) / (RAND_MAX + 1.0); 
    double u2 = (rand() + 1.0) / (RAND_MAX + 1.0);
    double z0 = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    int64_t result = (int64_t)round(z0 * sigma); 
    return result;
}

