#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P13_NOINLINE __attribute__((noinline))

#define P13_SLIP_END     0xC0
#define P13_SLIP_ESC     0xDB
#define P13_SLIP_ESC_END 0xDC
#define P13_SLIP_ESC_ESC 0xDD

typedef struct {
    uint8_t buffer[256];
    size_t length;
    size_t capacity;
} p13_frame_buf_t;

/* Benchmark Function Prototypes */
P13_NOINLINE size_t   p13_f01_slip_encode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap);
P13_NOINLINE size_t   p13_f03_cobs_encode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap);
P13_NOINLINE uint16_t p13_f05_fcs16_compute(const uint8_t *data, size_t len);
P13_NOINLINE bool     p13_f06_fcs16_verify(const uint8_t *data, size_t len, uint16_t expected_fcs);
P13_NOINLINE size_t   p13_f09_bit_stuff_encode(const uint8_t *src, size_t src_bits, uint8_t *dst);
P13_NOINLINE bool     p13_f14_parse_tlv_field(const uint8_t *data, size_t len, uint8_t *tag, uint8_t *v_len, const uint8_t **val);
P13_NOINLINE bool     p13_f15_frame_buf_append(p13_frame_buf_t *fb, const uint8_t *data, size_t len);
P13_NOINLINE void     p13_f16_frame_buf_reset(p13_frame_buf_t *fb);

#endif /* TARGET_QUERY_COMPONENT_H */
