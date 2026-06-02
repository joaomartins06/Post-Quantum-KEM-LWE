#ifndef LWE_KEYGEN_H
#define LWE_KEYGEN_H

#include "params.h"
#include "types.h"

void keygen(PublicKey *pk, SecretKey *sk, const LWEParams *params);
    
#endif