#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: rotl64 */
static inline uint64_t rotl64(uint64_t x, int b) {
    return (x << b) | (x >> (64 - b));
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
