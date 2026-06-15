#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "types.h"


void params_to_bytes(const LWEParams *params, uint8_t *out){
    memcpy(out, &params->n, sizeof(int));
    memcpy(out + sizeof(int), &params->q, sizeof(int));
    memcpy(out + 2 * sizeof(int), &params->m, sizeof(int));
    memcpy(out + 3 * sizeof(int), &params->sigma, sizeof(double));
}


void params_from_bytes(const uint8_t *data, LWEParams *params){
    memcpy(&params->n, data, sizeof(int));
    memcpy(&params->q, data + sizeof(int), sizeof(int));
    memcpy(&params->m, data + 2 * sizeof(int), sizeof(int));
    memcpy(&params->sigma, data + 3 * sizeof(int), sizeof(double));
}


void pk_to_bytes(const PublicKey *pk, const LWEParams *params, uint8_t *out){
    size_t A_size = ((size_t)params->m) * ((size_t)params->n) * sizeof(int64_t);
    size_t b_size = ((size_t)params->m) * sizeof(int64_t);

    memcpy(out, pk->A, A_size);
    memcpy(out + A_size, pk->b, b_size);
}


void pk_from_bytes(const uint8_t *data, const LWEParams *params, PublicKey *pk){
    size_t A_size = ((size_t)params->m) * ((size_t)params->n) * sizeof(int64_t);
    size_t b_size = ((size_t)params->m) * sizeof(int64_t);

    pk->A = malloc(A_size);
    pk->b = malloc(b_size);

    if (!pk->A || !pk->b) return;

    memcpy(pk->A, data, A_size);
    memcpy(pk->b, data + A_size, b_size);
}


void ct_to_bytes(const Ciphertext *ct, const LWEParams *params, uint8_t *out) {
    size_t u_size = ((size_t)params->n) * ((size_t)params->n) * sizeof(int64_t);
    size_t v_size = ((size_t)params->n) * sizeof(int64_t);

    memcpy(out, ct->u, u_size);
    memcpy(out + u_size, ct->v, v_size);
}


void ct_from_bytes(const uint8_t *data, const LWEParams *params, Ciphertext *ct) {
    size_t u_size = ((size_t)params->n) * ((size_t)params->n) * sizeof(int64_t);
    size_t v_size = ((size_t)params->n) * sizeof(int64_t);

    ct->u = malloc(u_size);
    ct->v = malloc(v_size);

    if (!ct->u || !ct->v) return;

    memcpy(ct->u, data, u_size);
    memcpy(ct->v, data + u_size, v_size);
}