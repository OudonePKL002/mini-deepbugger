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

static void test_p15_f02_fnv1a_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f02_fnv1a_32(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f03_fnv1_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f03_fnv1_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f04_fnv1a_64(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f04_fnv1a_64(msg, sizeof(msg)-1) != 0);
}

static void test_p15_f05_murmur3_32_scramble(void) {
    assert(p15_f05_murmur3_32_scramble(0x12345678) != 0);
}

static void test_p15_f06_murmur3_32_fmix(void) {
    assert(p15_f06_murmur3_32_fmix(0x12345678) != 0);
}

static void test_p15_f07_murmur3_32(void) {
    const uint8_t msg[] = "MiniDeepBuggerV2"; assert(p15_f07_murmur3_32(msg, sizeof(msg)-1, 42) != 0);
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

static void test_p15_f12_bp_hash(void) {
    assert(p15_f12_bp_hash("TestString") != 0);
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

int main(void) {
    test_p15_f01_fnv1_32();
    test_p15_f02_fnv1a_32();
    test_p15_f03_fnv1_64();
    test_p15_f04_fnv1a_64();
    test_p15_f05_murmur3_32_scramble();
    test_p15_f06_murmur3_32_fmix();
    test_p15_f07_murmur3_32();
    test_p15_f08_jenkins_one_at_a_time();
    test_p15_f09_super_fast_hash();
    test_p15_f10_elf_hash();
    test_p15_f11_dek_hash();
    test_p15_f12_bp_hash();
    test_p15_f13_ap_hash();
    test_p15_f14_crc24_ble();
    test_p15_f15_hash_combine64();
    test_p15_f16_checksum_parity_byte();
    printf("PASS: test_p15_reuse100 passed.\n");
    return 0;
}
