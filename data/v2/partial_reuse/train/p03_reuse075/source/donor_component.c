#include "donor_component.h"
#include <string.h>
#include <math.h>

P07_NOINLINE void p07_f06_slab_mark_busy(p07_slab_t *s, size_t idx) {
    if (!s || idx >= 32) return;
    s->alloc_bitmap |= (1U << idx);
}

P07_NOINLINE int p07_f08_slab_find_first_free_bit(uint32_t bitmap) {
    for (int i = 0; i < 32; ++i) {
        if (!(bitmap & (1U << i))) return i;
    }
    return -1;
}

P07_NOINLINE size_t p07_f09_slab_defragment_check(const p07_slab_t *s) {
    if (!s) return 0;
    size_t transitions = 0;
    uint32_t b = s->alloc_bitmap;
    for (int i = 0; i < 31; ++i) {
        if (((b >> i) & 1) != ((b >> (i + 1)) & 1)) transitions++;
    }
    return transitions;
}

P07_NOINLINE void* p07_f12_slab_index_to_pointer(p07_slab_t *s, size_t idx) {
    if (!s || idx >= P07_NUM_BLOCKS) return NULL;
    return (void*)&(s->memory_pool[idx * P07_BLOCK_SZ]);
}
