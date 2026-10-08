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
P02_NOINLINE void p02_f01_heap_init(p02_heap_t *h);
P02_NOINLINE void p02_f02_heap_sift_up(p02_heap_t *h, size_t idx);
P02_NOINLINE void p02_f03_heap_sift_down(p02_heap_t *h, size_t idx);
P02_NOINLINE bool p02_f04_heap_push(p02_heap_t *h, int32_t val);
P02_NOINLINE bool p02_f06_heap_peek(const p02_heap_t *h, int32_t *out_val);
P02_NOINLINE bool p02_f10_heap_delete_at(p02_heap_t *h, size_t idx);
P02_NOINLINE bool p02_f13_heap_merge(p02_heap_t *dst, const p02_heap_t *src);
P02_NOINLINE void p02_f16_heap_clear(p02_heap_t *h);

#endif /* TARGET_QUERY_COMPONENT_H */
