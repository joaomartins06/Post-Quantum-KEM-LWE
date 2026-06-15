#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "params.h"
#include "types.h"
#include "serial.h"
#include "keygen.h"


int main(){
    LWEParams params = { .n = 4, .q = 16, .m = 4, .sigma = 3.2 };
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

    free(pk->A);
    free(pk->b);
    free(pk);
    free(sk->s);
    free(sk);
    free(deserialized_pk->A);
    free(deserialized_pk->b);
    free(deserialized_pk);  

    return 0;
}