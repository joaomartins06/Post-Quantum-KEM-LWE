#include "kem.h"
#include "sampler.h"
#include "modular.h"

#include <stdlib.h>

void encaps(PublicKey *pk, Ciphertext *ct, uint8_t *shared_secret, const LWEParams *params){
    
    ct -> u = malloc(((size_t)params -> n) * ((size_t)params -> n) * sizeof(int64_t)); 
    ct -> v = malloc(((size_t)params -> n) * sizeof(int64_t));
    
    int64_t *R = malloc(((size_t)params -> m) * ((size_t)params -> n) * sizeof(int64_t)); 

    // generate mu (length n)
    for (int32_t i = 0; i < params->n; i++)
        shared_secret[i] = (uint8_t)sample_uniform(2);

    //generate the random matrix R
    for (int32_t i = 0; i < params -> m; i++){
        for (int32_t j = 0; j < params -> n; j++){
            R[i*(params -> n) + j] = sample_uniform(2); //it is a binary matrix(m,n)
        }
    }

    //compute u = A^T * R (mod q)
    for (int32_t i = 0; i < params -> n; i++){
        for (int32_t j = 0; j < params -> n; j++){
            int64_t sum = 0;
            for (int32_t k = 0; k < params -> m; k++) {
                sum += (pk -> A[k*(params -> n)+i]) * R[k*(params -> n)+j];
            }
            ct->u[i*params->n+j] = sum % (params -> q);
            //again, if negative, add q to get the positive equivalent
            if (ct->u[i*params->n+j] < 0) ct->u[i*params->n+j] += params->q; 
        }
    }

    int64_t *aux = malloc(((size_t)params -> n) * sizeof(int64_t));

    //compute aux = b^T * R (mod q)
    for (int32_t i = 0; i < params -> n; i++){
        int64_t sum = 0;
        for (int32_t k = 0; k < params -> m; k++) {
            sum += (pk -> b[k]) * R[k*(params -> n)+i];
        }
        aux[i] = sum % (params -> q);
        if (aux[i] < 0) aux[i] += params->q; 
    }

    //compute v = aux + (q/2)*mu (mod q)
    for (int32_t i = 0; i < params -> n; i++){
        ct -> v[i] = mod_add(aux[i], ((params -> q) / 2) * shared_secret[i], params -> q);
    }

    free(R);
    free(aux);
}


void decaps(SecretKey *sk, Ciphertext *ct, uint8_t *shared_secret, const LWEParams *params){
     
    //compute aux = u^T * s mod q
    int64_t *aux = malloc(((size_t)params -> n) * sizeof(int64_t));

    for (int32_t i = 0; i < params -> n; i++){
        int64_t sum = 0;
        for (int32_t j = 0; j < params -> n; j++){
            sum += (ct -> u[j*(params -> n) + i]) * (sk -> s[j]);
        }
        aux[i] = sum % (params -> q);
        if (aux[i] < 0) aux[i] += params->q; 
    }

    //compute w = v - aux (mod q)
    for (int32_t i = 0; i < params -> n; i++){
        int64_t w = ((ct -> v[i]) - aux[i]) % (params -> q);
        if (w < 0) w += params->q;
        shared_secret[i] = (w >= params->q / 4 && w <= 3 * params->q / 4) ? 1 : 0;
    }

    free(aux);
}