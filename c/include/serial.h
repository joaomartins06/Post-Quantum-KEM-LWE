#ifndef LWE_SERIAL_H
#define LWE_SERIAL_H

#include <stdint.h>

#include "types.h"
#include "params.h"


void params_to_bytes(const LWEParams *params, uint8_t *out);

void params_from_bytes(const uint8_t *data, LWEParams *params);

void pk_to_bytes(const PublicKey *pk, const LWEParams *params, uint8_t *out);

void pk_from_bytes(const uint8_t *data, const LWEParams *params, PublicKey *pk);

void ct_to_bytes(const Ciphertext *ct, const LWEParams *params, uint8_t *out);

void ct_from_bytes(const uint8_t *data, const LWEParams *params, Ciphertext *ct);




#endif