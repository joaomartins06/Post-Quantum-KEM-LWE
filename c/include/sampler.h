#ifndef LWE_SAMPLER_H
#define LWE_SAMPLER_H

#include <stdint.h>
#include "params.h"

int64_t sample_error(double sigma);

int64_t sample_uniform(int32_t q);

#endif