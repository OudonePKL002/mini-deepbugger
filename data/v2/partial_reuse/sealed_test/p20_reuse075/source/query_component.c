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

P20_NOINLINE bool p20_f02_ring_is_empty(const p20_ring_t *r) {
    if (!r) return true;
    return r->count == 0;
}

P20_NOINLINE bool p20_f03_ring_is_full(const p20_ring_t *r) {
    if (!r) return false;
    return r->count == P20_RING_CAP;
}

P20_NOINLINE size_t p20_f04_ring_available(const p20_ring_t *r) {
    if (!r) return 0;
    return r->count;
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

P20_NOINLINE bool p20_f10_ring_peek_byte(const p20_ring_t *r, size_t offset, uint8_t *out_byte) {
    if (!r || offset >= r->count) return false;
    size_t idx = (r->tail + offset) % P20_RING_CAP;
    if (out_byte) *out_byte = r->buffer[idx];
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

P20_NOINLINE uint8_t p20_f15_ring_checksum(const p20_ring_t *r) {
    if (!r) return 0;
    uint8_t c = 0;
    for (size_t i = 0; i < r->count; ++i) {
        c ^= r->buffer[(r->tail + i) % P20_RING_CAP];
    }
    return c;
}

P20_NOINLINE void p20_f16_ring_clear(p20_ring_t *r) {
    if (!r) return;
    p20_f01_ring_init(r);
}
