#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p01_crc_fletcher.h"

int main(void) {
    const uint8_t sample[] = "MiniDeepBugger2026";
    size_t len = sizeof(sample) - 1;

    uint8_t c8_1 = p01_f01_crc8_smbus(sample, len);
    uint8_t c8_2 = p01_f02_crc8_cdma(sample, len);
    assert(c8_1 != 0 || c8_2 != 0);

    uint16_t c16_1 = p01_f03_crc16_ccitt(sample, len);
    uint16_t c16_2 = p01_f04_crc16_modbus(sample, len);
    assert(c16_1 != c16_2);

    uint32_t c32_1 = p01_f05_crc32_ieee(sample, len);
    uint32_t c32_2 = p01_f06_crc32_table_driven(sample, len);
    assert(c32_1 == c32_2);

    uint16_t fl16 = p01_f07_fletcher16(sample, len);
    assert(fl16 != 0);

    uint16_t wbuf[8] = { 0x1234, 0x5678, 0x9ABC, 0xDEF0, 0x1357, 0x2468, 0x3579, 0x4680 };
    uint32_t fl32 = p01_f08_fletcher32(wbuf, 8);
    assert(fl32 != 0);

    uint16_t bsd = p01_f09_bsd_checksum(sample, len);
    uint32_t sysv = p01_f10_sysv_checksum(sample, len);
    assert(bsd != 0 && sysv != 0);

    assert(p01_f11_luhn_validate("79927398713") == 1);
    assert(p01_f11_luhn_validate("79927398714") == 0);

    uint8_t xor_res = p01_f12_xor8_block(sample, len, 0x55);
    uint16_t inet = p01_f13_internet_checksum(sample, len);
    uint16_t adl = p01_f14_adler16_simple(sample, len);
    uint8_t pear = p01_f15_pearson_hash8(sample, len);
    assert(xor_res != 0 && inet != 0 && adl != 0);

    uint32_t combo = p01_f16_checksum_combine(c16_1, c32_1, pear);
    assert(combo != 0);

    printf("PASS: p01_crc_fletcher unit tests passed.\n");
    return 0;
}
