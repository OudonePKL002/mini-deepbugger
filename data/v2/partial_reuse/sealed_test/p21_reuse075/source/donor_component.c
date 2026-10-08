#include "donor_component.h"
#include <string.h>
#include <math.h>

P20_NOINLINE void p20_f01_ring_init(p20_ring_t *r) {
    if (!r) return;
    r->head = 0;
    r->tail = 0;
    r->count = 0;
    r->max_count_seen = 0;
    for (size_t i = 0; i < P20_RING_CAP; ++i) r->buffer[i] = 0;
}

P20_NOINLINE size_t p20_f08_ring_write_slice(p20_ring_t *r, const uint8_t *src, size_t len) {
    if (!r || !src) return 0;
    size_t written = 0;
    while (written < len && r->count < P20_RING_CAP) {
        r->buffer[r->head] = src[written++];
        r->head = (r->head + 1) % P20_RING_CAP;
        r->count++;
    }
    if (r->count > r->max_count_seen) r->max_count_seen = r->count;
    return written;
}

P20_NOINLINE size_t p20_f11_ring_discard(p20_ring_t *r, size_t len) {
    if (!r) return 0;
    size_t to_discard = (len > r->count) ? r->count : len;
    r->tail = (r->tail + to_discard) % P20_RING_CAP;
    r->count -= to_discard;
    return to_discard;
}

P20_NOINLINE size_t p20_f14_ring_high_watermark(const p20_ring_t *r) {
    if (!r) return 0;
    return r->max_count_seen;
}
