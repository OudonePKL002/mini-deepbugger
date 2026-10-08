#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p21_circular_log.h"

int main(void) {
    p21_circ_log_t log;
    p21_f01_log_init(&log);
    assert(p21_f06_log_count(&log) == 0);

    assert(p21_f02_log_append(&log, 1000, P21_LOG_INFO, "System booting"));
    assert(p21_f02_log_append(&log, 1005, P21_LOG_WARN, "Memory high"));
    assert(p21_f02_log_append(&log, 1010, P21_LOG_ERROR, "Disk full"));

    assert(p21_f06_log_count(&log) == 3);

    p21_log_entry_t ent;
    assert(p21_f04_log_get_latest(&log, &ent) && ent.level == P21_LOG_ERROR);
    assert(p21_f05_log_get_oldest(&log, &ent) && ent.level == P21_LOG_INFO);
    assert(p21_f03_log_get_by_seq(&log, 2, &ent) && ent.level == P21_LOG_WARN);

    assert(p21_f08_log_search_substring(&log, "Disk") == 3);

    p21_log_entry_t errs[2];
    assert(p21_f07_log_filter_level(&log, P21_LOG_ERROR, errs, 2) == 1);

    size_t stats[4];
    p21_f15_log_export_stats(&log, stats);
    assert(stats[P21_LOG_INFO] == 1 && stats[P21_LOG_WARN] == 1 && stats[P21_LOG_ERROR] == 1);

    const uint8_t raw[] = {1, 1, 1, 2, 2, 3};
    uint8_t comp[16];
    size_t c_sz = p21_f11_rle_compress(raw, 6, comp, 16);
    assert(c_sz == 6);
    uint8_t decomp[16];
    size_t d_sz = p21_f12_rle_decompress(comp, c_sz, decomp, 16);
    assert(d_sz == 6 && memcmp(raw, decomp, 6) == 0);

    p21_f14_log_truncate_older(&log, 1005);
    assert(p21_f06_log_count(&log) == 2);

    p21_f16_log_clear(&log);
    assert(p21_f06_log_count(&log) == 0);

    printf("PASS: p21_circular_log unit tests passed.\n");
    return 0;
}
