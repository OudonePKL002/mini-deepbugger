#include "p08_adler_siphash.h"

static inline uint64_t rotl64(uint64_t x, int b) {
    return (x << b) | (x >> (64 - b));
}

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

P08_NOINLINE uint64_t p08_f03_siphash24(const uint8_t *data, size_t len, const uint8_t key[16]) {
    uint64_t k0 = 0, k1 = 0;
    for (int i = 0; i < 8; ++i) {
        k0 |= ((uint64_t)key[i]) << (8 * i);
        k1 |= ((uint64_t)key[i + 8]) << (8 * i);
    }
    uint64_t v[4] = {
        k0 ^ 0x736f6d6570736575ULL,
        k1 ^ 0x646f72616e646f6dULL,
        k0 ^ 0x6c7967656e657261ULL,
        k1 ^ 0x7465646279746573ULL
    };
    size_t left = len & 7;
    const uint8_t *end = data + len - left;
    for (const uint8_t *p = data; p < end; p += 8) {
        uint64_t m = 0;
        for (int i = 0; i < 8; ++i) m |= ((uint64_t)p[i]) << (8 * i);
        v[3] ^= m;
        p08_f02_siphash_round(v);
        p08_f02_siphash_round(v);
        v[0] ^= m;
    }
    uint64_t b = ((uint64_t)len) << 56;
    for (size_t i = 0; i < left; ++i) {
        b |= ((uint64_t)end[i]) << (8 * i);
    }
    v[3] ^= b;
    p08_f02_siphash_round(v);
    p08_f02_siphash_round(v);
    v[0] ^= b;
    v[2] ^= 0xff;
    p08_f02_siphash_round(v);
    p08_f02_siphash_round(v);
    p08_f02_siphash_round(v);
    p08_f02_siphash_round(v);
    return v[0] ^ v[1] ^ v[2] ^ v[3];
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

P08_NOINLINE uint32_t p08_f05_djb2_hash(const char *str) {
    uint32_t hash = 5381;
    if (!str) return 0;
    while (*str) {
        hash = ((hash << 5) + hash) + (uint8_t)(*str++);
    }
    return hash;
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

P08_NOINLINE uint32_t p08_f08_jenkins_lookup2(const uint8_t *k, size_t length, uint32_t initval) {
    uint32_t a = 0x9e3779b9U, b = 0x9e3779b9U, c = initval;
    size_t len = length;
    while (len >= 12) {
        a += (k[0] +((uint32_t)k[1]<<8) +((uint32_t)k[2]<<16) +((uint32_t)k[3]<<24));
        b += (k[4] +((uint32_t)k[5]<<8) +((uint32_t)k[6]<<16) +((uint32_t)k[7]<<24));
        c += (k[8] +((uint32_t)k[9]<<8) +((uint32_t)k[10]<<16)+((uint32_t)k[11]<<24));
        a -= b; a -= c; a ^= (c >> 13);
        b -= c; b -= a; b ^= (a << 8);
        c -= a; c -= b; c ^= (b >> 13);
        a -= b; a -= c; a ^= (c >> 12);
        b -= c; b -= a; b ^= (a << 16);
        c -= a; c -= b; c ^= (b >> 5);
        a -= b; a -= c; a ^= (c >> 3);
        b -= c; b -= a; b ^= (a << 10);
        c -= a; c -= b; c ^= (b >> 15);
        k += 12; len -= 12;
    }
    c += (uint32_t)length;
    switch(len) {
        case 11: c += ((uint32_t)k[10] << 24); /* fallthrough */
        case 10: c += ((uint32_t)k[9] << 16);  /* fallthrough */
        case 9 : c += ((uint32_t)k[8] << 8);   /* fallthrough */
        case 8 : b += ((uint32_t)k[7] << 24);  /* fallthrough */
        case 7 : b += ((uint32_t)k[6] << 16);  /* fallthrough */
        case 6 : b += ((uint32_t)k[5] << 8);   /* fallthrough */
        case 5 : b += k[4];                    /* fallthrough */
        case 4 : a += ((uint32_t)k[3] << 24);  /* fallthrough */
        case 3 : a += ((uint32_t)k[2] << 16);  /* fallthrough */
        case 2 : a += ((uint32_t)k[1] << 8);   /* fallthrough */
        case 1 : a += k[0];
        default: break;
    }
    a -= b; a -= c; a ^= (c >> 13);
    b -= c; b -= a; b ^= (a << 8);
    c -= a; c -= b; c ^= (b >> 13);
    a -= b; a -= c; a ^= (c >> 12);
    b -= c; b -= a; b ^= (a << 16);
    c -= a; c -= b; c ^= (b >> 5);
    a -= b; a -= c; a ^= (c >> 3);
    b -= c; b -= a; b ^= (a << 10);
    c -= a; c -= b; c ^= (b >> 15);
    return c;
}

P08_NOINLINE uint32_t p08_f09_knuth_multiplicative(uint32_t val) {
    return val * 2654435761U;
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

P08_NOINLINE void p08_f13_poly1305_clamp(uint8_t r[16]) {
    if (!r) return;
    r[3] &= 15;
    r[7] &= 15;
    r[11] &= 15;
    r[15] &= 15;
    r[4] &= 252;
    r[8] &= 252;
    r[12] &= 252;
}

P08_NOINLINE uint32_t p08_f14_chash_simple(const uint8_t *data, size_t len, uint32_t seed) {
    uint32_t h = seed ^ 0x12345678U;
    for (size_t i = 0; i < len; ++i) {
        h = (h ^ data[i]) * 0x5bd1e995U;
        h ^= (h >> 15);
    }
    return h;
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
