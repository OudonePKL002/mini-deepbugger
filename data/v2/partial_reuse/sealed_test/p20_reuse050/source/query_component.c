#include "query_component.h"
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

P20_NOINLINE size_t p20_f04_ring_available(const p20_ring_t *r) {
    if (!r) return 0;
    return r->count;
}

P20_NOINLINE bool p20_f06_ring_write_byte(p20_ring_t *r, uint8_t byte) {
    if (!r || r->count >= P20_RING_CAP) return false;
    r->buffer[r->head] = byte;
    r->head = (r->head + 1) % P20_RING_CAP;
    r->count++;
    if (r->count > r->max_count_seen) r->max_count_seen = r->count;
    return true;
}

P20_NOINLINE bool p20_f07_ring_read_byte(p20_ring_t *r, uint8_t *out_byte) {
    if (!r || r->count == 0) return false;
    if (out_byte) *out_byte = r->buffer[r->tail];
    r->tail = (r->tail + 1) % P20_RING_CAP;
    r->count--;
    return true;
}

P20_NOINLINE size_t p20_f11_ring_discard(p20_ring_t *r, size_t len) {
    if (!r) return 0;
    size_t to_discard = (len > r->count) ? r->count : len;
    r->tail = (r->tail + to_discard) % P20_RING_CAP;
    r->count -= to_discard;
    return to_discard;
}

P20_NOINLINE int64_t p20_f12_ring_find_marker(const p20_ring_t *r, const uint8_t *marker, size_t marker_len) {
    if (!r || !marker || marker_len == 0 || marker_len > r->count) return -1;
    for (size_t i = 0; i <= r->count - marker_len; ++i) {
        bool match = true;
        for (size_t j = 0; j < marker_len; ++j) {
            size_t idx = (r->tail + i + j) % P20_RING_CAP;
            if (r->buffer[idx] != marker[j]) { match = false; break; }
        }
        if (match) return (int64_t)i;
    }
    return -1;
}

P20_NOINLINE size_t p20_f13_ring_linearize(const p20_ring_t *r, uint8_t *dst, size_t max_len) {
    if (!r || !dst) return 0;
    size_t to_copy = (r->count > max_len) ? max_len : r->count;
    for (size_t i = 0; i < to_copy; ++i) {
        dst[i] = r->buffer[(r->tail + i) % P20_RING_CAP];
    }
    return to_copy;
}

P20_NOINLINE void p20_f16_ring_clear(p20_ring_t *r) {
    if (!r) return;
    p20_f01_ring_init(r);
}
