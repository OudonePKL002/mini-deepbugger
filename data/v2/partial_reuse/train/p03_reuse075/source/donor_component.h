#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P07_NOINLINE __attribute__((noinline))
#define P07_BLOCK_SZ 32
#define P07_NUM_BLOCKS 32

typedef struct {
    uint8_t memory_pool[P07_NUM_BLOCKS * P07_BLOCK_SZ];
    uint32_t alloc_bitmap;
    size_t free_count;
    size_t total_blocks;
} p07_slab_t;

/* Benchmark Function Prototypes */
P07_NOINLINE void     p07_f06_slab_mark_busy(p07_slab_t *s, size_t idx);
P07_NOINLINE int      p07_f08_slab_find_first_free_bit(uint32_t bitmap);
P07_NOINLINE size_t   p07_f09_slab_defragment_check(const p07_slab_t *s);
P07_NOINLINE void*    p07_f12_slab_index_to_pointer(p07_slab_t *s, size_t idx);

#endif /* TARGET_DONOR_COMPONENT_H */
