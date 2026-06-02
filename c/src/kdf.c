#include "kdf.h"

#include <openssl/evp.h>
#include <openssl/kdf.h>


static int hkdf_sha256(const uint8_t *ikm, size_t ikm_len,
                const uint8_t *info, size_t info_len,
                uint8_t *out, size_t out_len) {

    EVP_KDF *kdf = EVP_KDF_fetch(NULL, "HKDF", NULL);
    EVP_KDF_CTX *ctx = EVP_KDF_CTX_new(kdf);
    EVP_KDF_free(kdf);

    OSSL_PARAM params[5];
    params[0] = OSSL_PARAM_construct_utf8_string("digest", "SHA256", 0);
    params[1] = OSSL_PARAM_construct_octet_string("key", (void*)ikm, ikm_len);
    params[2] = OSSL_PARAM_construct_octet_string("info", (void*)info, info_len);
    params[3] = OSSL_PARAM_construct_end();

    int ret = EVP_KDF_derive(ctx, out, out_len, params);
    EVP_KDF_CTX_free(ctx);

    return ret; 
}


int derive_key(const uint8_t *mu, size_t mu_len, 
                const uint8_t *info, size_t info_len,
                uint8_t *out) {
    return hkdf_sha256(mu, mu_len, info, info_len, out, 32);
}
