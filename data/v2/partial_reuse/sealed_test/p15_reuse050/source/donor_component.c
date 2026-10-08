#include "donor_component.h"
#include <string.h>
#include <math.h>

P21_NOINLINE void p21_f01_log_init(p21_circ_log_t *log) {
    if (!log) return;
    log->head = 0;
    log->count = 0;
    log->next_seq = 1;
}

P21_NOINLINE bool p21_f02_log_append(p21_circ_log_t *log, uint32_t ts, p21_log_level_t level, const char *msg) {
    if (!log || !msg) return false;
    p21_log_entry_t *e = &log->entries[log->head];
    e->seq = log->next_seq++;
    e->timestamp = ts;
    e->level = level;
    strncpy(e->msg, msg, P21_MSG_LEN - 1);
    e->msg[P21_MSG_LEN - 1] = '\0';

    log->head = (log->head + 1) % P21_MAX_ENTRIES;
    if (log->count < P21_MAX_ENTRIES) log->count++;
    return true;
}

P21_NOINLINE bool p21_f04_log_get_latest(const p21_circ_log_t *log, p21_log_entry_t *out_entry) {
    if (!log || log->count == 0) return false;
    size_t last = (log->head + P21_MAX_ENTRIES - 1) % P21_MAX_ENTRIES;
    if (out_entry) *out_entry = log->entries[last];
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

P21_NOINLINE size_t p21_f13_log_filter_timestamp(const p21_circ_log_t *log, uint32_t start_ts, uint32_t end_ts, p21_log_entry_t *out_entries, size_t max_out) {
    if (!log || !out_entries || max_out == 0) return 0;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    size_t out_cnt = 0;
    for (size_t i = 0; i < log->count && out_cnt < max_out; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        if (log->entries[idx].timestamp >= start_ts && log->entries[idx].timestamp <= end_ts) {
            out_entries[out_cnt++] = log->entries[idx];
        }
    }
    return out_cnt;
}

P21_NOINLINE void p21_f15_log_export_stats(const p21_circ_log_t *log, size_t stats[4]) {
    if (!log || !stats) return;
    stats[0] = stats[1] = stats[2] = stats[3] = 0;
    size_t start = (log->head + P21_MAX_ENTRIES - log->count) % P21_MAX_ENTRIES;
    for (size_t i = 0; i < log->count; ++i) {
        size_t idx = (start + i) % P21_MAX_ENTRIES;
        if (log->entries[idx].level <= P21_LOG_ERROR) {
            stats[log->entries[idx].level]++;
        }
    }
}

P21_NOINLINE void p21_f16_log_clear(p21_circ_log_t *log) {
    if (!log) return;
    p21_f01_log_init(log);
}
