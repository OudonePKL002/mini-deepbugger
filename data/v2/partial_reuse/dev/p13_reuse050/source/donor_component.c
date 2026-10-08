#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: rotl32 */
static inline uint32_t rotl32(uint32_t x, int b) {
    return (x << b) | (x >> (32 - b));
}

P08_NOINLINE uint32_t p08_f01_adler32(const uint8_t *data, size_t len) {
    uint32_t a = 1, b = 0;
    const uint32_t mod_adler = 65521;
    for (size_t i = 0; i < len; ++i) {
        a = (a + data[i]) % mod_adler;
        b = (b + a) % mod_adler;
    }
    return (b << 16) | a;
}

P08_NOINLINE uint32_t p08_f04_half_siphash(const uint8_t *data, size_t len, const uint8_t key[8]) {
    uint32_t k0 = 0, k1 = 0;
    for (int i = 0; i < 4; ++i) {
        k0 |= ((uint32_t)key[i]) << (8 * i);
        k1 |= ((uint32_t)key[i + 4]) << (8 * i);
    }
    uint32_t v0 = k0, v1 = k1, v2 = k0 ^ 0x6c796765U, v3 = k1 ^ 0x74656462U;
    for (size_t i = 0; i < len; ++i) {
        uint32_t m = (uint32_t)data[i];
        v3 ^= m;
        v0 += v1; v1 = rotl32(v1, 5);  v1 ^= v0; v0 = rotl32(v0, 16);
        v2 += v3; v3 = rotl32(v3, 8);  v3 ^= v2;
        v0 += v3; v3 = rotl32(v3, 7);  v3 ^= v0;
        v2 += v1; v1 = rotl32(v1, 13); v1 ^= v2; v2 = rotl32(v2, 16);
        v0 ^= m;
    }
    v2 ^= 0xFF;
    v0 += v1; v1 = rotl32(v1, 5);  v1 ^= v0; v0 = rotl32(v0, 16);
    v2 += v3; v3 = rotl32(v3, 8);  v3 ^= v2;
    v0 += v3; v3 = rotl32(v3, 7);  v3 ^= v0;
    v2 += v1; v1 = rotl32(v1, 13); v1 ^= v2; v2 = rotl32(v2, 16);
    return v0 ^ v1 ^ v2 ^ v3;
}

P08_NOINLINE uint32_t p08_f06_sdbm_hash(const char *str) {
    uint32_t hash = 0;
    if (!str) return 0;
    while (*str) {
        hash = (uint8_t)(*str++) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

P08_NOINLINE uint32_t p08_f09_knuth_multiplicative(uint32_t val) {
    return val * 2654435761U;
}

P08_NOINLINE uint32_t p08_f11_rotate_mix32(const uint8_t *data, size_t len) {
    uint32_t acc = 0x47b54721U;
    for (size_t i = 0; i < len; ++i) {
        acc = rotl32(acc, 7) ^ ((uint32_t)data[i] * 0x9e37U);
        acc = acc + 0x1000193U;
    }
    acc ^= (acc >> 11);
    acc += (acc << 3);
    return acc;
}

P08_NOINLINE uint16_t p08_f12_crc16_usb(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 1) crc = (crc >> 1) ^ 0xA001;
            else crc >>= 1;
        }
    }
    return crc ^ 0xFFFF;
}

P08_NOINLINE uint32_t p08_f15_hash_mix32(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6bU;
    h ^= h >> 13;
    h *= 0xc2b2ae35U;
    h ^= h >> 16;
    return h;
}

P08_NOINLINE uint32_t p08_f16_checksum_fold64_to_32(uint64_t val) {
    return (uint32_t)(val ^ (val >> 32));
}
