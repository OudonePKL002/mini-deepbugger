#include "donor_component.h"
#include <string.h>
#include <math.h>

P07_NOINLINE void p07_f01_slab_init(p07_slab_t *s) {
    if (!s) return;
    s->alloc_bitmap = 0;
    s->total_blocks = P07_NUM_BLOCKS;
    s->free_count = P07_NUM_BLOCKS;
    for (size_t i = 0; i < sizeof(s->memory_pool); ++i) {
        s->memory_pool[i] = 0;
    }
}

P07_NOINLINE void* p07_f02_slab_alloc_block(p07_slab_t *s) {
    if (!s || s->free_count == 0) return NULL;
    int idx = p07_f08_slab_find_first_free_bit(s->alloc_bitmap);
    if (idx < 0 || (size_t)idx >= s->total_blocks) return NULL;
    p07_f06_slab_mark_busy(s, (size_t)idx);
    s->free_count--;
    return p07_f12_slab_index_to_pointer(s, (size_t)idx);
}

P07_NOINLINE bool p07_f03_slab_free_block(p07_slab_t *s, void *ptr) {
    if (!s || !ptr) return false;
    int idx = p07_f11_slab_block_index(s, ptr);
    if (idx < 0) return false;
    if (!(s->alloc_bitmap & (1U << idx))) return false; // double free
    p07_f07_slab_mark_free(s, (size_t)idx);
    s->free_count++;
    return true;
}

P07_NOINLINE size_t p07_f04_slab_available_count(const p07_slab_t *s) {
    return s ? s->free_count : 0;
}

P07_NOINLINE bool p07_f05_slab_is_pointer_valid(const p07_slab_t *s, const void *ptr) {
    return p07_f11_slab_block_index(s, ptr) >= 0;
}

P07_NOINLINE void p07_f06_slab_mark_busy(p07_slab_t *s, size_t idx) {
    if (!s || idx >= 32) return;
    s->alloc_bitmap |= (1U << idx);
}

P07_NOINLINE void p07_f07_slab_mark_free(p07_slab_t *s, size_t idx) {
    if (!s || idx >= 32) return;
    s->alloc_bitmap &= ~(1U << idx);
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

P07_NOINLINE void p07_f10_slab_reset(p07_slab_t *s) {
    p07_f01_slab_init(s);
}

P07_NOINLINE int p07_f11_slab_block_index(const p07_slab_t *s, const void *ptr) {
    if (!s || !ptr) return -1;
    const uint8_t *base = s->memory_pool;
    const uint8_t *p = (const uint8_t*)ptr;
    if (p < base || p >= base + (P07_NUM_BLOCKS * P07_BLOCK_SZ)) return -1;
    size_t diff = (size_t)(p - base);
    if (diff % P07_BLOCK_SZ != 0) return -1;
    return (int)(diff / P07_BLOCK_SZ);
}

P07_NOINLINE void* p07_f12_slab_index_to_pointer(p07_slab_t *s, size_t idx) {
    if (!s || idx >= P07_NUM_BLOCKS) return NULL;
    return (void*)&(s->memory_pool[idx * P07_BLOCK_SZ]);
}

P07_NOINLINE uint32_t p07_f13_slab_stats_utilization(const p07_slab_t *s) {
    if (!s || s->total_blocks == 0) return 0;
    size_t used = s->total_blocks - s->free_count;
    return (uint32_t)((used * 100U) / s->total_blocks);
}

P07_NOINLINE bool p07_f14_slab_audit_integrity(const p07_slab_t *s) {
    if (!s) return false;
    size_t set_bits = 0;
    for (int i = 0; i < 32; ++i) {
        if (s->alloc_bitmap & (1U << i)) set_bits++;
    }
    return (s->free_count + set_bits) == s->total_blocks;
}

P07_NOINLINE void p07_f15_slab_scrub_pattern(p07_slab_t *s, size_t idx, uint8_t pattern) {
    void *ptr = p07_f12_slab_index_to_pointer(s, idx);
    if (!ptr) return;
    uint8_t *b = (uint8_t*)ptr;
    for (size_t i = 0; i < P07_BLOCK_SZ; ++i) {
        b[i] = pattern;
    }
}

P07_NOINLINE size_t p07_f16_slab_compact_scan(const p07_slab_t *s, size_t *out_longest_free_run) {
    if (!s) return 0;
    size_t max_run = 0;
    size_t curr_run = 0;
    size_t free_spans = 0;
    for (int i = 0; i < 32; ++i) {
        if (!(s->alloc_bitmap & (1U << i))) {
            curr_run++;
            if (curr_run == 1) free_spans++;
            if (curr_run > max_run) max_run = curr_run;
        } else {
            curr_run = 0;
        }
    }
    if (out_longest_free_run) *out_longest_free_run = max_run;
    return free_spans;
}
