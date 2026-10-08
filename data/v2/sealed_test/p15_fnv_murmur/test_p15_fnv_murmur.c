#include <stdio.h>
#include <assert.h>
#include "p15_fnv_murmur.h"

int main(void) {
    const uint8_t msg[] = "SealedTestBenchmark2026";
    size_t len = sizeof(msg) - 1;

    uint32_t f1 = p15_f01_fnv1_32(msg, len);
    uint32_t f1a = p15_f02_fnv1a_32(msg, len);
    assert(f1 != 0 && f1a != 0 && f1 != f1a);

    uint64_t f1_64 = p15_f03_fnv1_64(msg, len);
    uint64_t f1a_64 = p15_f04_fnv1a_64(msg, len);
    assert(f1_64 != 0 && f1a_64 != 0 && f1_64 != f1a_64);

    uint32_t m3 = p15_f07_murmur3_32(msg, len, 42);
    assert(m3 != 0);

    uint32_t jen = p15_f08_jenkins_one_at_a_time(msg, len);
    assert(jen != 0);

    uint32_t sf = p15_f09_super_fast_hash(msg, len);
    assert(sf != 0);

    assert(p15_f10_elf_hash("TestString") != 0);
    assert(p15_f11_dek_hash("TestString") != 0);
    assert(p15_f12_bp_hash("TestString") != 0);
    assert(p15_f13_ap_hash("TestString") != 0);

    uint32_t ble = p15_f14_crc24_ble(msg, len, 0x555555);
    assert(ble != 0);

    uint64_t comb = p15_f15_hash_combine64(f1_64, f1a_64);
    assert(comb != 0);

    uint8_t par = p15_f16_checksum_parity_byte(msg, len);
    assert(par != 0);

    printf("PASS: p15_fnv_murmur unit tests passed.\n");
    return 0;
}
