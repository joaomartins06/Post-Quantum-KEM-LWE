#ifndef LWE_TYPES_H
#define LWE_TYPES_H

#include <stdint.h>

typedef struct {
    int64_t *A;  
    int64_t *b; 
} PublicKey;


typedef struct {
    int64_t *sk;  
} SecretKey;


typedef struct {
    int64_t *u;  
    int64_t *v; 
} Ciphertext;


#endif