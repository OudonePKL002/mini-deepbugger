#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P06_NOINLINE __attribute__((noinline))
#define P06_MAX_BUF 256

/* Benchmark Function Prototypes */
P06_NOINLINE size_t p06_f02_tlv_encode_uint16(uint8_t tag, uint16_t val, uint8_t *buf, size_t buf_sz);
P06_NOINLINE size_t p06_f04_tlv_encode_bytes(uint8_t tag, const uint8_t *val, size_t len, uint8_t *buf, size_t buf_sz);
P06_NOINLINE bool   p06_f07_tlv_decode_uint16(const uint8_t *buf, size_t len, uint16_t *out_val);
P06_NOINLINE bool   p06_f08_tlv_decode_uint32(const uint8_t *buf, size_t len, uint32_t *out_val);
P06_NOINLINE int    p06_f09_tlv_find_tag(const uint8_t *buf, size_t len, uint8_t target_tag);
P06_NOINLINE size_t p06_f10_tlv_count_tags(const uint8_t *buf, size_t len);
P06_NOINLINE size_t p06_f14_packet_payload_pad(uint8_t *buf, size_t data_len, size_t block_sz, size_t max_buf);
P06_NOINLINE size_t p06_f15_packet_payload_unpad(const uint8_t *buf, size_t padded_len);

#endif /* TARGET_DONOR_COMPONENT_H */
