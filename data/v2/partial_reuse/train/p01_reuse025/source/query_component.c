#include "query_component.h"
#include <string.h>
#include <math.h>

P01_NOINLINE uint32_t p01_f06_crc32_table_driven(const uint8_t *data, size_t len) {
    uint32_t table[256];
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t c = i;
        for (int k = 0; k < 8; ++k) {
            c = (c & 1) ? (0xEDB88320U ^ (c >> 1)) : (c >> 1);
        }
        table[i] = c;
    }
    uint32_t crc = 0xFFFFFFFFU;
    for (size_t i = 0; i < len; ++i) {
        crc = table[(crc ^ data[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc ^ 0xFFFFFFFFU;
}

P01_NOINLINE int p01_f11_luhn_validate(const char *digits) {
    if (!digits) return 0;
    int sum = 0;
    int alt = 0;
    size_t len = 0;
    while (digits[len]) len++;
    if (len < 2) return 0;
    for (int i = (int)len - 1; i >= 0; --i) {
        char c = digits[i];
        if (c < '0' || c > '9') return 0;
        int d = c - '0';
        if (alt) {
            d *= 2;
            if (d > 9) d -= 9;
        }
        sum += d;
        alt = !alt;
    }
    return (sum % 10) == 0;
}

P01_NOINLINE uint8_t p01_f15_pearson_hash8(const uint8_t *data, size_t len) {
    static const uint8_t perm[16] = {
        0x3, 0xF, 0x1, 0x9, 0xC, 0x0, 0x7, 0xA,
        0x5, 0xE, 0x2, 0x8, 0xD, 0xB, 0x4, 0x6
    };
    uint8_t h = 0x07;
    for (size_t i = 0; i < len; ++i) {
        h = perm[(h ^ data[i]) & 0x0F];
    }
    return h;
}

P01_NOINLINE uint32_t p01_f16_checksum_combine(uint16_t c16, uint32_t c32, uint8_t c8) {
    uint32_t mix = c32 ^ ((uint32_t)c16 << 16) ^ ((uint32_t)c8 << 8);
    mix ^= (mix >> 13);
    mix *= 0x5BD1E995U;
    mix ^= (mix >> 15);
    return mix;
}
