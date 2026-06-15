#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>


static int send_all(int sockfd, const uint8_t *data, uint32_t len) {
    uint32_t total = 0;
    while (total < len){
        ssize_t n = send(sockfd, data + total, len - total, 0);
        if (n <= 0) return -1;
        total += (uint32_t)n;
    }
    return 0;
}


static int recv_all(int sockfd, uint8_t *buf, uint32_t len) {
    uint32_t total = 0;
    while (total < len) {
        ssize_t n = recv(sockfd, buf + total, len - total, 0);
        if (n <= 0) return -1;
        total += (uint32_t)n;
    }
    return 0;
}


void send_msg(int sockfd, const uint8_t *data, uint32_t len){
    uint32_t len_net = htonl(len);
    send_all(sockfd, (uint8_t *)&len_net, sizeof(len_net));
    send_all(sockfd, data, len);
}


uint8_t *recv_msg(int sockfd, uint32_t *out_len){
    uint32_t len_net;
    if (recv_all(sockfd, (uint8_t *)&len_net, sizeof(len_net)) != 0)
        return NULL;

    uint32_t msg_len = ntohl(len_net);

    uint8_t *data = malloc(msg_len);
    if (!data) return NULL;

    if (recv_all(sockfd, data, msg_len) != 0) {
        free(data);
        return NULL;
    }

    *out_len = msg_len;
    return data;
}