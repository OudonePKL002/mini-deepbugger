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
P06_NOINLINE bool   p06_f08_tlv_decode_uint32(const uint8_t *buf, size_t len, uint32_t *out_val);

#endif /* TARGET_QUERY_COMPONENT_H */
