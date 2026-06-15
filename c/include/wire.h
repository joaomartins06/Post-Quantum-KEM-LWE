#ifndef LWE_WIRE_H
#define LWE_WIRE_H

#include <stdint.h>

void send_msg(int sockfd, const uint8_t *data, uint32_t len);

uint8_t *recv_msg(int sockfd, uint32_t *out_len);

#endif