#include "keygen.h"
#include "modular.h"
#include "sampler.h"

#include <stdlib.h>

void keygen(PublicKey *pk, SecretKey *sk, const LWEParams *params){
    //allocate memory for the secret key and public key
    sk -> s = malloc(((size_t)params -> n) * sizeof(int64_t));
    pk -> A = malloc(((size_t)params -> m) * ((size_t)params -> n) * sizeof(int64_t));
    pk -> b = malloc(((size_t)params -> m) * sizeof(int64_t));

    //NULL check
    if (sk->s == NULL || pk->A == NULL || pk->b == NULL) return;

    //generate the secret key s
    for (int32_t i = 0; i < params -> n; i++){
        sk -> s[i] = sample_uniform(params -> q);
    }

    //generate the public key A
    for (int32_t i = 0; i < params -> m; i++){
        for (int32_t j = 0; j < params -> n; j++){
            pk -> A[i*params -> n + j] = sample_uniform(params -> q);
        }
        //After computing row i, we can compute b[i] = A[i]*s + e
        //right away by generating the error and using s from previous loop
        int64_t sum = 0;
        //compute row i of A times s
        for (int32_t j = 0; j < params -> n; j++){
            sum += (pk -> A[i*(params -> n) + j]) * (sk -> s[j]);
        }
        //add error by sampling it inside the mod_add()
        pk -> b[i] = mod_add(sum, sample_error(params -> sigma), params -> q);
    }


}