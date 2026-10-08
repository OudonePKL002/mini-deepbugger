#include "query_component.h"
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

P07_NOINLINE void p07_f06_slab_mark_busy(p07_slab_t *s, size_t idx) {
    if (!s || idx >= 32) return;
    s->alloc_bitmap |= (1U << idx);
}

P07_NOINLINE uint32_t p07_f13_slab_stats_utilization(const p07_slab_t *s) {
    if (!s || s->total_blocks == 0) return 0;
    size_t used = s->total_blocks - s->free_count;
    return (uint32_t)((used * 100U) / s->total_blocks);
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
