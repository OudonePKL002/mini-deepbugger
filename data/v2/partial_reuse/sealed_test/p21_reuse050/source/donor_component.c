#include "donor_component.h"
#include <string.h>
#include <math.h>

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

P20_NOINLINE bool p20_f10_ring_peek_byte(const p20_ring_t *r, size_t offset, uint8_t *out_byte) {
    if (!r || offset >= r->count) return false;
    size_t idx = (r->tail + offset) % P20_RING_CAP;
    if (out_byte) *out_byte = r->buffer[idx];
    return true;
}

P20_NOINLINE size_t p20_f13_ring_linearize(const p20_ring_t *r, uint8_t *dst, size_t max_len) {
    if (!r || !dst) return 0;
    size_t to_copy = (r->count > max_len) ? max_len : r->count;
    for (size_t i = 0; i < to_copy; ++i) {
        dst[i] = r->buffer[(r->tail + i) % P20_RING_CAP];
    }
    return to_copy;
}

P20_NOINLINE size_t p20_f14_ring_high_watermark(const p20_ring_t *r) {
    if (!r) return 0;
    return r->max_count_seen;
}

P20_NOINLINE uint8_t p20_f15_ring_checksum(const p20_ring_t *r) {
    if (!r) return 0;
    uint8_t c = 0;
    for (size_t i = 0; i < r->count; ++i) {
        c ^= r->buffer[(r->tail + i) % P20_RING_CAP];
    }
    return c;
}
