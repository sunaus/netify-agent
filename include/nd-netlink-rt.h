#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nd_ct_flow {
    uint32_t id;

    uint8_t l3_proto;
    uint8_t l4_proto;

    uint8_t src_ip[16];
    uint8_t dst_ip[16];

    uint16_t src_port;
    uint16_t dst_port;

    uint64_t packets_src;
    uint64_t packets_dst;
    uint64_t bytes_src;
    uint64_t bytes_dst;

    uint32_t mark;
} nd_ct_flow;

#ifdef __cplusplus
}
#endif
