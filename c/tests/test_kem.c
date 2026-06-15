#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keygen.h"
#include "kem.h"
#include "kdf.h"
#include "params.h"
#include "types.h"

extern const LWEParams DEFAULT_PARAMS;

int main(void){

    const LWEParams *params = &DEFAULT_PARAMS;
    
    PublicKey pk;
    SecretKey sk;

    //key generation
    keygen(&pk, &sk, params);

    uint8_t shared_secret_enc[256];
    uint8_t shared_secret_dec[256];

    Ciphertext ct;
    int fails = 0;
    int tries = 100;
    for (int i = 0; i < tries; i++){
        encaps(&pk, &ct, shared_secret_enc, params);
        decaps(&sk, &ct, shared_secret_dec, params);
        int match = (memcmp(shared_secret_enc, shared_secret_dec, (size_t)params->n) == 0);
        if (!match) fails++;
        free(ct.u);
        free(ct.v);
    }

    double failure_rate = (double)fails / tries;
    printf("Failure rate: %f\n", failure_rate);

    free(sk.s);
    free(pk.A);
    free(pk.b);

    return 0;
}