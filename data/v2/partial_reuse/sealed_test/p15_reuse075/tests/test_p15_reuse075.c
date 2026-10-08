#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p15_f01_fnv1_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f01_fnv1_32(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f03_fnv1_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f03_fnv1_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f04_fnv1a_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f04_fnv1a_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f06_murmur3_32_fmix(void) {
    assert(p15_f06_murmur3_32_fmix(0x12345678) != 0);
}

static void test_p15_f08_jenkins_one_at_a_time(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f08_jenkins_one_at_a_time(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f09_super_fast_hash(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f09_super_fast_hash(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f10_elf_hash(void) {
    assert(p15_f10_elf_hash("TestString") != 0);
}

static void test_p15_f11_dek_hash(void) {
    assert(p15_f11_dek_hash("TestString") != 0);
}

static void test_p15_f13_ap_hash(void) {
    assert(p15_f13_ap_hash("TestString") != 0);
}

static void test_p15_f14_crc24_ble(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f14_crc24_ble(msg, sizeof(msg)-1, 0x555555) != 0);
}

static void test_p15_f15_hash_combine64(void) {
    assert(p15_f15_hash_combine64(10, 20) != 0);
}

static void test_p15_f16_checksum_parity_byte(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f16_checksum_parity_byte(msg, sizeof(msg)-1) != 0);
}

static void test_p21_f01_log_init(void) {
    p21_circ_log_t log; log.count = 5; p21_f01_log_init(&log); assert(log.count == 0 && log.next_seq == 1);
}

static void test_p21_f06_log_count(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); log.count = 1; assert(p21_f06_log_count(&log) == 1);
}

static void test_p21_f09_log_checksum(void) {
    p21_circ_log_t log; memset(&log, 0, sizeof(log)); assert(p21_f09_log_checksum(&log) == 0x55555555U);
}

static void test_p21_f12_rle_decompress(void) {
    const uint8_t comp[4] = {2, 1, 1, 2}; uint8_t dec[4]; assert(p21_f12_rle_decompress(comp, 4, dec, 4) == 3);
}

int main(void) {
    test_p15_f01_fnv1_32();
    test_p15_f03_fnv1_64();
    test_p15_f04_fnv1a_64();
    test_p15_f06_murmur3_32_fmix();
    test_p15_f08_jenkins_one_at_a_time();
    test_p15_f09_super_fast_hash();
    test_p15_f10_elf_hash();
    test_p15_f11_dek_hash();
    test_p15_f13_ap_hash();
    test_p15_f14_crc24_ble();
    test_p15_f15_hash_combine64();
    test_p15_f16_checksum_parity_byte();
    test_p21_f01_log_init();
    test_p21_f06_log_count();
    test_p21_f09_log_checksum();
    test_p21_f12_rle_decompress();
    printf("PASS: test_p15_reuse075 passed.\n");
    return 0;
}
