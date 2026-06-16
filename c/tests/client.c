#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "params.h"
#include "types.h"
#include "serial.h"
#include "keygen.h"
#include "kem.h"
#include "kdf.h"
#include "wire.h"


int main() {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9999);

    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
        }

    printf("Client: Connected to server.\n");

    uint32_t params_len;
    uint8_t *params_bytes = recv_msg(sockfd, &params_len);
    if (!params_bytes) {
        printf("Client: Failed to receive parameters.\n");
        close(sockfd);
        return 1;
    }

    LWEParams *params;
    params = malloc(sizeof(LWEParams));
    params_from_bytes(params_bytes, params);

    printf("Client: Parameters received.\n");

    uint32_t pk_len;
    uint8_t *pk_bytes = recv_msg(sockfd, &pk_len);
    if (!pk_bytes) {
        printf("Client: Failed to receive public key.\n");
        free(params_bytes);
        close(sockfd);
        return 1;
    }
    PublicKey *pk = malloc(sizeof(PublicKey));
    pk_from_bytes(pk_bytes, params, pk);

    printf("Client: Public key received.\n");

    Ciphertext *ct;
    ct = malloc(sizeof(Ciphertext));

    uint8_t *shared_secret = malloc((size_t)params->n * sizeof(uint8_t));

    encaps(pk, ct, shared_secret, params);
    printf("Client: Shared secret generated.\n");

    uint8_t ct_bytes[((size_t)params->n) * ((size_t)params->n) * sizeof(int64_t) + ((size_t)params->n) * sizeof(int64_t)];
    ct_to_bytes(ct, params, ct_bytes);

    send_msg(sockfd, ct_bytes, (uint32_t)sizeof(ct_bytes));
    printf("Client: Ciphertext sent.\n");

    uint8_t key[32];
    derive_key(shared_secret, (size_t)params->n, (const uint8_t*)"lwe-kem-v1", strlen("lwe-kem-v1"), key);

    printf("Client: Derived key: ");
    for (int i = 0; i < 32; i++) printf("%02x", key[i]);
    printf("\n");

    free(params);
    free(params_bytes);
    free(pk_bytes);
    free(pk->A);
    free(pk->b);
    free(pk);
    free(ct->u);
    free(ct->v);
    free(ct);
    free(shared_secret);
    close(sockfd);

    return 0;
}