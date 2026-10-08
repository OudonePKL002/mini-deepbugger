#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P06_NOINLINE __attribute__((noinline))
#define P06_MAX_BUF 256

/* Benchmark Function Prototypes */
P06_NOINLINE size_t p06_f02_tlv_encode_uint16(uint8_t tag, uint16_t val, uint8_t *buf, size_t buf_sz);
P06_NOINLINE size_t p06_f03_tlv_encode_uint32(uint8_t tag, uint32_t val, uint8_t *buf, size_t buf_sz);
P06_NOINLINE bool   p06_f05_tlv_decode_header(const uint8_t *buf, size_t len, uint8_t *out_tag, uint8_t *out_len);
P06_NOINLINE bool   p06_f06_tlv_decode_uint8(const uint8_t *buf, size_t len, uint8_t *out_val);
P06_NOINLINE size_t p06_f12_packet_header_build(uint16_t seq, uint16_t payload_len, uint8_t *hdr_out, size_t hdr_sz);
P06_NOINLINE bool   p06_f13_packet_header_parse(const uint8_t *hdr_in, size_t hdr_len, uint16_t *out_seq, uint16_t *out_payload_len);
P06_NOINLINE size_t p06_f14_packet_payload_pad(uint8_t *buf, size_t data_len, size_t block_sz, size_t max_buf);
P06_NOINLINE size_t p06_f15_packet_payload_unpad(const uint8_t *buf, size_t padded_len);

#endif /* TARGET_QUERY_COMPONENT_H */
