#include "query_component.h"
#include <string.h>
#include <math.h>

P08_NOINLINE uint32_t p08_f05_djb2_hash(const char *str) {
    uint32_t hash = 5381;
    if (!str) return 0;
    while (*str) {
        hash = ((hash << 5) + hash) + (uint8_t)(*str++);
    }
    return hash;
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

P08_NOINLINE uint32_t p08_f16_checksum_fold64_to_32(uint64_t val) {
    return (uint32_t)(val ^ (val >> 32));
}
