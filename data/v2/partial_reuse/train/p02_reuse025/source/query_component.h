#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

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
P02_NOINLINE void p02_f02_heap_sift_up(p02_heap_t *h, size_t idx);
P02_NOINLINE void p02_f03_heap_sift_down(p02_heap_t *h, size_t idx);
P02_NOINLINE bool p02_f11_heap_increase_key(p02_heap_t *h, size_t idx, int32_t delta);
P02_NOINLINE bool p02_f14_heap_is_valid(const p02_heap_t *h);

#endif /* TARGET_QUERY_COMPONENT_H */
