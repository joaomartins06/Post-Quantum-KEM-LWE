#include "params.h"
#include "types.h"
#include "serial.h"
#include <stdio.h>
#include <string.h>


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
    
    return 0;
}