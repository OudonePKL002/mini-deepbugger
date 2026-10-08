#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P20_NOINLINE __attribute__((noinline))
#define P20_RING_CAP 128

typedef struct {
    uint8_t buffer[P20_RING_CAP];
    size_t head;
    size_t tail;
    size_t count;
    size_t max_count_seen;
} p20_ring_t;

/* Benchmark Function Prototypes */
P20_NOINLINE void    p20_f01_ring_init(p20_ring_t *r);
P20_NOINLINE bool    p20_f02_ring_is_empty(const p20_ring_t *r);
P20_NOINLINE bool    p20_f03_ring_is_full(const p20_ring_t *r);
P20_NOINLINE size_t  p20_f04_ring_available(const p20_ring_t *r);
P20_NOINLINE size_t  p20_f08_ring_write_slice(p20_ring_t *r, const uint8_t *src, size_t len);
P20_NOINLINE size_t  p20_f09_ring_read_slice(p20_ring_t *r, uint8_t *dst, size_t max_len);
P20_NOINLINE bool    p20_f10_ring_peek_byte(const p20_ring_t *r, size_t offset, uint8_t *out_byte);
P20_NOINLINE size_t  p20_f11_ring_discard(p20_ring_t *r, size_t len);
P20_NOINLINE int64_t p20_f12_ring_find_marker(const p20_ring_t *r, const uint8_t *marker, size_t marker_len);
P20_NOINLINE size_t  p20_f13_ring_linearize(const p20_ring_t *r, uint8_t *dst, size_t max_len);
P20_NOINLINE uint8_t p20_f15_ring_checksum(const p20_ring_t *r);
P20_NOINLINE void    p20_f16_ring_clear(p20_ring_t *r);

#endif /* TARGET_QUERY_COMPONENT_H */
