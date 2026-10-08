#include "query_component.h"
#include <string.h>
#include <math.h>

P21_NOINLINE void p21_f01_log_init(p21_circ_log_t *log) {
    if (!log) return;
    log->head = 0;
    log->count = 0;
    log->next_seq = 1;
}

P21_NOINLINE bool p21_f03_log_get_by_seq(const p21_circ_log_t *log, uint32_t seq, p21_log_entry_t *out_entry) {
    if (!log || log->count == 0) return false;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    for (size_t i = 0; i < log->count; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        if (log->entries[idx].seq == seq) {
            if (out_entry) *out_entry = log->entries[idx];
            return true;
        }
    }
    return false;
}

P21_NOINLINE bool p21_f04_log_get_latest(const p21_circ_log_t *log, p21_log_entry_t *out_entry) {
    if (!log || log->count == 0) return false;
    size_t last = (log->head + P21_MAX_ENTRIES - 1) % P21_MAX_ENTRIES;
    if (out_entry) *out_entry = log->entries[last];
    return true;
}

P21_NOINLINE bool p21_f05_log_get_oldest(const p21_circ_log_t *log, p21_log_entry_t *out_entry) {
    if (!log || log->count == 0) return false;
    size_t first = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    if (out_entry) *out_entry = log->entries[first];
    return true;
}

P21_NOINLINE size_t p21_f07_log_filter_level(const p21_circ_log_t *log, p21_log_level_t min_lvl, p21_log_entry_t *out_entries, size_t max_out) {
    if (!log || !out_entries || max_out == 0) return 0;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    size_t out_cnt = 0;
    for (size_t i = 0; i < log->count && out_cnt < max_out; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        if (log->entries[idx].level >= min_lvl) {
            out_entries[out_cnt++] = log->entries[idx];
        }
    }
    return out_cnt;
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

P21_NOINLINE size_t p21_f11_rle_compress(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || src_len == 0) return 0;
    size_t out = 0;
    size_t i = 0;
    while (i < src_len) {
        uint8_t byte = src[i];
        uint8_t run = 1;
        while (i + 1 < src_len && src[i + 1] == byte && run < 255) {
            run++;
            i++;
        }
        if (out + 2 > dst_cap) return 0;
        dst[out++] = run;
        dst[out++] = byte;
        i++;
    }
    return out;
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
