#ifndef LWE_KEM_H
#define LWE_KEM_H

#include "params.h"
#include "types.h"

void encaps(PublicKey *pk, Ciphertext *ct, uint8_t *shared_secret, const LWEParams *params);

void decaps(SecretKey *sk, Ciphertext *ct, uint8_t *shared_secret, const LWEParams *params);

#endif