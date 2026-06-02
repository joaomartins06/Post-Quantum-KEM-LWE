#ifndef LWE_KDF_H
#define LWE_KDF_H

#include <stdint.h>
#include <stddef.h>

int derive_key(const uint8_t *mu, size_t mu_len, const uint8_t *info, size_t info_len,
                uint8_t *out);

#endif