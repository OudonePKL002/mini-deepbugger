#include "query_component.h"
#include <string.h>
#include <math.h>

P15_NOINLINE uint64_t p15_f03_fnv1_64(const uint8_t *data, size_t len) {
    uint64_t h = 14695981039346656037ULL;
    for (size_t i = 0; i < len; ++i) {
        h = (h * 1099511628211ULL) ^ data[i];
    }
    return h;
}

P15_NOINLINE uint32_t p15_f09_super_fast_hash(const uint8_t *data, size_t len) {
    uint32_t hash = (uint32_t)len;
    uint32_t tmp;
    int rem;
    if (len == 0 || !data) return 0;
    rem = len & 3;
    len >>= 2;
    for (; len > 0; len--) {
        hash += (uint32_t)data[0] | ((uint32_t)data[1] << 8);
        tmp = ((uint32_t)data[2] | ((uint32_t)data[3] << 8)) << 11;
        hash = (hash << 16) ^ (hash ^ tmp);
        data += 4;
        hash += hash >> 11;
    }
    switch (rem) {
        case 3: hash += (uint32_t)data[0] | ((uint32_t)data[1] << 8);
                hash ^= hash << 16;
                hash ^= ((uint32_t)data[2]) << 18;
                hash += hash >> 11;
                break;
        case 2: hash += (uint32_t)data[0] | ((uint32_t)data[1] << 8);
                hash ^= hash << 11;
                hash += hash >> 17;
                break;
        case 1: hash += (uint32_t)data[0];
                hash ^= hash << 10;
                hash += hash >> 1;
                break;
        default: break;
    }
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;
    return hash;
}

P15_NOINLINE uint32_t p15_f11_dek_hash(const char *str) {
    uint32_t hash = 5381;
    if (!str) return 0;
    while (*str) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (uint8_t)(*str++);
    }
    return hash;
}

P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2) {
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
}
