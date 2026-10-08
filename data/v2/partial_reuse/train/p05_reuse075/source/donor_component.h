#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P02_NOINLINE __attribute__((noinline))
#define P02_MAX_CAP 128

typedef struct {
    int32_t data[P02_MAX_CAP];
    size_t size;
    size_t capacity;
} p02_heap_t;

/* Benchmark Function Prototypes */
P02_NOINLINE void p02_f03_heap_sift_down(p02_heap_t *h, size_t idx);
P02_NOINLINE bool p02_f05_heap_pop_min(p02_heap_t *h, int32_t *out_val);
P02_NOINLINE bool p02_f06_heap_peek(const p02_heap_t *h, int32_t *out_val);
P02_NOINLINE bool p02_f09_heap_replace(p02_heap_t *h, int32_t new_val, int32_t *old_val);

#endif /* TARGET_DONOR_COMPONENT_H */
