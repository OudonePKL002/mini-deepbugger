#include <stdio.h>
#include <assert.h>
#include "p07_slab_allocator.h"

int main(void) {
    p07_slab_t slab;
    p07_f01_slab_init(&slab);
    assert(p07_f04_slab_available_count(&slab) == 32);

    void *b1 = p07_f02_slab_alloc_block(&slab);
    assert(b1 != NULL);
    assert(p07_f04_slab_available_count(&slab) == 31);
    assert(p07_f05_slab_is_pointer_valid(&slab, b1) == true);

    void *b2 = p07_f02_slab_alloc_block(&slab);
    assert(b2 != NULL && b2 != b1);

    assert(p07_f14_slab_audit_integrity(&slab) == true);
    assert(p07_f13_slab_stats_utilization(&slab) == (2 * 100 / 32));

    p07_f15_slab_scrub_pattern(&slab, 0, 0xAA);

    assert(p07_f03_slab_free_block(&slab, b1) == true);
    assert(p07_f04_slab_available_count(&slab) == 31);

    size_t longest = 0;
    size_t spans = p07_f16_slab_compact_scan(&slab, &longest);
    assert(spans > 0 && longest > 0);

    size_t frag = p07_f09_slab_defragment_check(&slab);
    assert(frag > 0);

    p07_f10_slab_reset(&slab);
    assert(p07_f04_slab_available_count(&slab) == 32);

    printf("PASS: p07_slab_allocator unit tests passed.\n");
    return 0;
}
