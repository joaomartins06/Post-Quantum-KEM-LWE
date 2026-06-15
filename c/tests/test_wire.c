#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "wire.h"


int main(void){

    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        return 1;
    }

    const char *msg = "Hello, world!";
    send_msg(sv[0], (const uint8_t *)msg, (uint32_t)strlen(msg));

    uint32_t len;
    uint8_t *recv_msg_buf = recv_msg(sv[1], &len);
    if (recv_msg_buf) {
        printf("Received message: %.*s\n", len, recv_msg_buf);
        free(recv_msg_buf);
    } else {
        printf("Failed to receive message\n");
    }

    close(sv[0]);
    close(sv[1]);

    return 0;
}