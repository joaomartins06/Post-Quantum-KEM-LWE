#ifndef LWE_PARAMS_H
#define LWE_PARAMS_H

typedef struct {
    int n;  //length of the secret key     
    int q;  //modulus
    int m;  //number of samples/cols on A
    double sigma;  //standard deviation of the error distribution
} LWEParams;

#endif
