#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "params.h"
#include "types.h"
#include "serial.h"
#include "keygen.h"
#include "kem.h"


int main(){
    LWEParams params = { .n = 4, .q = 16, .m = 4, .sigma = 3.2 };

    //test params to and from bytes
    uint8_t params_bytes[4 * sizeof(int) + sizeof(double)];
    params_to_bytes(&params, params_bytes);

    LWEParams deserialized_params;
    params_from_bytes(params_bytes, &deserialized_params);

    if (params.n == deserialized_params.n && params.q == deserialized_params.q &&
        params.m == deserialized_params.m && params.sigma == deserialized_params.sigma) {
        printf("LWEParams serialization/deserialization successful!\n");
    } else {
        printf("LWEParams serialization/deserialization failed.\n");
    }

    //test pk to and from bytes
    PublicKey* pk;
    SecretKey* sk;
    pk = malloc(sizeof(PublicKey));
    sk = malloc(sizeof(SecretKey));

    keygen(pk, sk, &params);
    uint8_t pk_bytes[((size_t)params.m) * ((size_t)params.n) * sizeof(int64_t) + ((size_t)params.m) * sizeof(int64_t)];
    pk_to_bytes(pk, &params, pk_bytes);

    PublicKey* deserialized_pk = malloc(sizeof(PublicKey));
    pk_from_bytes(pk_bytes, &params, deserialized_pk);
    
    if (memcmp(pk->A, deserialized_pk->A, ((size_t)params.m) * ((size_t)params.n) * sizeof(int64_t)) == 0 &&
        memcmp(pk->b, deserialized_pk->b, ((size_t)params.m) * sizeof(int64_t)) == 0) {
        printf("PublicKey serialization/deserialization successful!\n");
    } else {
        printf("PublicKey serialization/deserialization failed.\n");
    }

    free(sk->s);
    free(sk);
    free(deserialized_pk->A);
    free(deserialized_pk->b);
    free(deserialized_pk);  


    //test ct to and from bytes
    //test ct to and from bytes
    Ciphertext* ct;
    ct = malloc(sizeof(Ciphertext));

    uint8_t *shared_secret; 
    shared_secret = malloc(((size_t)params.n) * sizeof(uint8_t));

    encaps(pk, ct, shared_secret, &params);

    uint8_t ct_bytes[((size_t)params.n) * ((size_t)params.n) * sizeof(int64_t) + ((size_t)params.n) * sizeof(int64_t)];

    ct_to_bytes(ct, &params, ct_bytes);

    Ciphertext* deserialized_ct;
    deserialized_ct = malloc(sizeof(Ciphertext));

    ct_from_bytes(ct_bytes, &params, deserialized_ct);

    if (memcmp(ct->u, deserialized_ct->u, ((size_t)params.n) * ((size_t)params.n) * sizeof(int64_t)) == 0 &&
        memcmp(ct->v, deserialized_ct->v, ((size_t)params.n) * sizeof(int64_t)) == 0) {
        printf("Ciphertext serialization/deserialization successful!\n");
    } else {
        printf("Ciphertext serialization/deserialization failed.\n");
    }

    free(pk->A);
    free(pk->b);
    free(pk);
    free(ct->u);
    free(ct->v);
    free(ct);
    free(shared_secret);
    free(deserialized_ct->u);
    free(deserialized_ct->v);
    free(deserialized_ct);

    return 0;
}