#ifndef LWE_MODULAR_H
#define LWE_MODULAR_H

#include <stdint.h>
#include "params.h"

int64_t mod_add(int64_t a, int64_t b, int32_t q);

void mod_matmul(int64_t *out, const int64_t *A, const int64_t *B,
                int32_t rows, int32_t inner, int32_t cols, int32_t q);

#endif

