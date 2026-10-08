#include "donor_component.h"
#include <string.h>
#include <math.h>

P21_NOINLINE void p21_f01_log_init(p21_circ_log_t *log) {
    if (!log) return;
    log->head = 0;
    log->count = 0;
    log->next_seq = 1;
}

P21_NOINLINE size_t p21_f06_log_count(const p21_circ_log_t *log) {
    if (!log) return 0;
    return log->count;
}

P21_NOINLINE uint32_t p21_f09_log_checksum(const p21_circ_log_t *log) {
    if (!log) return 0;
    uint32_t csum = 0x55555555U;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    for (size_t i = 0; i < log->count; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        csum = (csum ^ log->entries[idx].seq) * 31 + log->entries[idx].timestamp;
    }
    return csum;
}

P21_NOINLINE size_t p21_f12_rle_decompress(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || src_len < 2 || (src_len % 2) != 0) return 0;
    size_t out = 0;
    for (size_t i = 0; i < src_len; i += 2) {
        uint8_t run = src[i];
        uint8_t byte = src[i + 1];
        if (out + run > dst_cap) return 0;
        for (uint8_t r = 0; r < run; ++r) {
            dst[out++] = byte;
        }
    }
    return out;
}
