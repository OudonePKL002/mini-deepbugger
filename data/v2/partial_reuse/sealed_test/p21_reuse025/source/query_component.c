#include "query_component.h"
#include <string.h>
#include <math.h>

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

P21_NOINLINE int64_t p21_f08_log_search_substring(const p21_circ_log_t *log, const char *substr) {
    if (!log || !substr || log->count == 0) return -1;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    for (size_t i = 0; i < log->count; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        if (strstr(log->entries[idx].msg, substr)) {
            return (int64_t)log->entries[idx].seq;
        }
    }
    return -1;
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

P21_NOINLINE void p21_f14_log_truncate_older(p21_circ_log_t *log, uint32_t min_ts) {
    if (!log || log->count == 0) return;
    while (log->count > 0) {
        size_t oldest = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
        if (log->entries[oldest].timestamp < min_ts) {
            log->count--;
        } else {
            break;
        }
    }
}
