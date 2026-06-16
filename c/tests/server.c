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

extern const LWEParams DEFAULT_PARAMS;

int main() {

    const LWEParams *params = &DEFAULT_PARAMS;
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(9999);

    bind(socketfd, (struct sockaddr *)&addr, sizeof(addr));
    listen(socketfd, 1);

    printf("Server listening on port 9999...\n");

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connfd = accept(socketfd, (struct sockaddr *)&client_addr, &client_len);

    printf("Server: Connection accepted.\n");

    PublicKey *pk = malloc(sizeof(PublicKey));
    SecretKey *sk = malloc(sizeof(SecretKey));
    Ciphertext *ct = malloc(sizeof(Ciphertext));

    keygen(pk, sk, params);
    printf("Server: Keypair generated.\n");

    uint8_t params_bytes[4 * sizeof(int) + sizeof(double)];
    params_to_bytes(params, params_bytes);
    send_msg(connfd, params_bytes, (uint32_t)sizeof(params_bytes));
    printf("Server: Parameters sent.\n");

    uint8_t pk_bytes[((size_t)params->m) * ((size_t)params->n) * sizeof(int64_t) + ((size_t)params->m) * sizeof(int64_t)];
    pk_to_bytes(pk, params, pk_bytes);
    send_msg(connfd, pk_bytes, (uint32_t)sizeof(pk_bytes));
    printf("Server: Public key sent.\n");

    uint32_t ct_len;
    uint8_t *ct_bytes = recv_msg(connfd, &ct_len);
    ct_from_bytes(ct_bytes, params, ct);
    printf("Server: Ciphertext received.\n");

    uint8_t *shared_secret = malloc((size_t)params->n * sizeof(uint8_t));
    decaps(sk, ct, shared_secret, params);
    printf("Server: Shared secret established.\n");

    uint8_t key[32];
    derive_key(shared_secret, (size_t)params->n, (const uint8_t *)"lwe-kem-v1", strlen("lwe-kem-v1"), key);

    printf("Server: Derived key: ");
    for (int i = 0; i < 32; i++) printf("%02x", key[i]);
    printf("\n");

    free(pk->A); free(pk->b); free(pk);
    free(sk->s); free(sk);
    free(ct->u); free(ct->v); free(ct);
    free(ct_bytes);
    free(shared_secret);
    close(connfd);
    close(socketfd);

    return 0;
}