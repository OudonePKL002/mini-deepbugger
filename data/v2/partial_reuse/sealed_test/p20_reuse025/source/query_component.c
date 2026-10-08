#include "query_component.h"
#include <string.h>
#include <math.h>

P20_NOINLINE bool p20_f03_ring_is_full(const p20_ring_t *r) {
    if (!r) return false;
    return r->count == P20_RING_CAP;
}

P20_NOINLINE bool p20_f06_ring_write_byte(p20_ring_t *r, uint8_t byte) {
    if (!r || r->count >= P20_RING_CAP) return false;
    r->buffer[r->head] = byte;
    r->head = (r->head + 1) % P20_RING_CAP;
    r->count++;
    if (r->count > r->max_count_seen) r->max_count_seen = r->count;
    return true;
}

P20_NOINLINE size_t p20_f09_ring_read_slice(p20_ring_t *r, uint8_t *dst, size_t max_len) {
    if (!r || !dst) return 0;
    size_t read = 0;
    while (read < max_len && r->count > 0) {
        dst[read++] = r->buffer[r->tail];
        r->tail = (r->tail + 1) % P20_RING_CAP;
        r->count--;
    }
    return read;
}

P20_NOINLINE size_t p20_f11_ring_discard(p20_ring_t *r, size_t len) {
    if (!r) return 0;
    size_t to_discard = (len > r->count) ? r->count : len;
    r->tail = (r->tail + to_discard) % P20_RING_CAP;
    r->count -= to_discard;
    return to_discard;
}
