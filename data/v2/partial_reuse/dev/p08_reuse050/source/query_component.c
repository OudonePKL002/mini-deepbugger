#include "query_component.h"
#include <string.h>
#include <math.h>

/* Support helper: rotl64 */
static inline uint64_t rotl64(uint64_t x, int b) {
    return (x << b) | (x >> (64 - b));
}

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

P08_NOINLINE void p08_f02_siphash_round(uint64_t v[4]) {
    v[0] += v[1]; v[1] = rotl64(v[1], 13); v[1] ^= v[0]; v[0] = rotl64(v[0], 32);
    v[2] += v[3]; v[3] = rotl64(v[3], 16); v[3] ^= v[2];
    v[0] += v[3]; v[3] = rotl64(v[3], 21); v[3] ^= v[0];
    v[2] += v[1]; v[1] = rotl64(v[1], 17); v[1] ^= v[2]; v[2] = rotl64(v[2], 32);
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

P08_NOINLINE uint32_t p08_f07_rabin_karp_rolling(uint32_t prev_h, uint8_t old_c, uint8_t new_c, uint32_t mult, uint32_t high_pow) {
    uint32_t h = prev_h - (old_c * high_pow);
    h = h * mult + new_c;
    return h;
}

P08_NOINLINE void p08_f10_rot13_cipher(char *dst, const char *src, size_t len) {
    if (!dst || !src) return;
    for (size_t i = 0; i < len; ++i) {
        char c = src[i];
        if (c >= 'a' && c <= 'z') dst[i] = (char)('a' + (c - 'a' + 13) % 26);
        else if (c >= 'A' && c <= 'Z') dst[i] = (char)('A' + (c - 'A' + 13) % 26);
        else dst[i] = c;
    }
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

P08_NOINLINE uint32_t p08_f14_chash_simple(const uint8_t *data, size_t len, uint32_t seed) {
    uint32_t h = seed ^ 0x12345678U;
    for (size_t i = 0; i < len; ++i) {
        h = (h ^ data[i]) * 0x5bd1e995U;
        h ^= (h >> 15);
    }
    return h;
}
