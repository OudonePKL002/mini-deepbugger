#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

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
P20_NOINLINE size_t  p20_f08_ring_write_slice(p20_ring_t *r, const uint8_t *src, size_t len);
P20_NOINLINE size_t  p20_f11_ring_discard(p20_ring_t *r, size_t len);
P20_NOINLINE size_t  p20_f14_ring_high_watermark(const p20_ring_t *r);

#endif /* TARGET_DONOR_COMPONENT_H */
