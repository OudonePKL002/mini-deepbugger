#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P06_NOINLINE __attribute__((noinline))
#define P06_MAX_BUF 256

/* Benchmark Function Prototypes */
P06_NOINLINE bool   p06_f08_tlv_decode_uint32(const uint8_t *buf, size_t len, uint32_t *out_val);
P06_NOINLINE size_t p06_f10_tlv_count_tags(const uint8_t *buf, size_t len);
P06_NOINLINE size_t p06_f12_packet_header_build(uint16_t seq, uint16_t payload_len, uint8_t *hdr_out, size_t hdr_sz);
P06_NOINLINE size_t p06_f16_packet_serialize(uint16_t seq, const uint8_t *payload, size_t p_len, uint8_t *wire_out, size_t max_out);

#endif /* TARGET_DONOR_COMPONENT_H */
