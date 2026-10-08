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
P20_NOINLINE bool    p20_f03_ring_is_full(const p20_ring_t *r);
P20_NOINLINE bool    p20_f06_ring_write_byte(p20_ring_t *r, uint8_t byte);
P20_NOINLINE size_t  p20_f09_ring_read_slice(p20_ring_t *r, uint8_t *dst, size_t max_len);
P20_NOINLINE size_t  p20_f11_ring_discard(p20_ring_t *r, size_t len);

#endif /* TARGET_QUERY_COMPONENT_H */
