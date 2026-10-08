#include "donor_component.h"
#include <string.h>
#include <math.h>

P15_NOINLINE uint32_t p15_f02_fnv1a_32(const uint8_t *data, size_t len) {
    uint32_t h = 2166136261U;
    for (size_t i = 0; i < len; ++i) {
        h = (h ^ data[i]) * 16777619U;
    }
    return h;
}

P15_NOINLINE uint64_t p15_f04_fnv1a_64(const uint8_t *data, size_t len) {
    uint64_t h = 14695981039346656037ULL;
    for (size_t i = 0; i < len; ++i) {
        h = (h ^ data[i]) * 1099511628211ULL;
    }
    return h;
}

P15_NOINLINE uint32_t p15_f13_ap_hash(const char *str) {
    uint32_t hash = 0xAAAAAAAA;
    if (!str) return 0;
    for (size_t i = 0; *str; ++i, ++str) {
        if ((i & 1) == 0) {
            hash ^= ((hash << 7) ^ (uint8_t)(*str) * (hash >> 3));
        } else {
            hash ^= (~((hash << 11) + ((uint8_t)(*str) ^ (hash >> 5))));
        }
    }
    return hash;
}

P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2) {
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
}
