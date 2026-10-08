#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P21_NOINLINE __attribute__((noinline))
#define P21_MAX_ENTRIES 32
#define P21_MSG_LEN 48

typedef enum {
    P21_LOG_DEBUG = 0,
    P21_LOG_INFO,
    P21_LOG_WARN,
    P21_LOG_ERROR
} p21_log_level_t;

typedef struct {
    uint32_t seq;
    uint32_t timestamp;
    p21_log_level_t level;
    char msg[P21_MSG_LEN];
} p21_log_entry_t;

typedef struct {
    p21_log_entry_t entries[P21_MAX_ENTRIES];
    size_t head;
    size_t count;
    uint32_t next_seq;
} p21_circ_log_t;

/* Benchmark Function Prototypes */
P21_NOINLINE void    p21_f01_log_init(p21_circ_log_t *log);
P21_NOINLINE bool    p21_f03_log_get_by_seq(const p21_circ_log_t *log, uint32_t seq, p21_log_entry_t *out_entry);
P21_NOINLINE bool    p21_f04_log_get_latest(const p21_circ_log_t *log, p21_log_entry_t *out_entry);
P21_NOINLINE bool    p21_f05_log_get_oldest(const p21_circ_log_t *log, p21_log_entry_t *out_entry);
P21_NOINLINE size_t  p21_f07_log_filter_level(const p21_circ_log_t *log, p21_log_level_t min_lvl, p21_log_entry_t *out_entries, size_t max_out);
P21_NOINLINE uint32_t p21_f09_log_checksum(const p21_circ_log_t *log);
P21_NOINLINE size_t  p21_f11_rle_compress(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap);
P21_NOINLINE size_t  p21_f12_rle_decompress(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap);

#endif /* TARGET_QUERY_COMPONENT_H */
