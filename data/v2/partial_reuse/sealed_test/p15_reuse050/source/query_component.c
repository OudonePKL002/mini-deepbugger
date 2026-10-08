#include "query_component.h"
#include <string.h>
#include <math.h>

P15_NOINLINE uint32_t p15_f01_fnv1_32(const uint8_t *data, size_t len) {
    uint32_t h = 2166136261U;
    for (size_t i = 0; i < len; ++i) {
        h = (h * 16777619U) ^ data[i];
    }
    return h;
}

P15_NOINLINE uint32_t p15_f02_fnv1a_32(const uint8_t *data, size_t len) {
    uint32_t h = 2166136261U;
    for (size_t i = 0; i < len; ++i) {
        h = (h ^ data[i]) * 16777619U;
    }
    return h;
}

P15_NOINLINE uint64_t p15_f03_fnv1_64(const uint8_t *data, size_t len) {
    uint64_t h = 14695981039346656037ULL;
    for (size_t i = 0; i < len; ++i) {
        h = (h * 1099511628211ULL) ^ data[i];
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

P15_NOINLINE uint32_t p15_f14_crc24_ble(const uint8_t *data, size_t len, uint32_t init_state) {
    uint32_t crc = init_state & 0x00FFFFFF;
    for (size_t i = 0; i < len; ++i) {
        uint8_t byte = data[i];
        for (int b = 0; b < 8; ++b) {
            uint32_t bit = (byte >> b) & 1;
            uint32_t top = (crc >> 23) & 1;
            crc = (crc << 1) & 0x00FFFFFF;
            if (bit ^ top) {
                crc ^= 0x0000065B; // BLE poly: x^24 + x^10 + x^9 + x^6 + x^4 + x^3 + x + 1
            }
        }
    }
    return crc;
}

P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2) {
    return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
}

P15_NOINLINE uint8_t p15_f16_checksum_parity_byte(const uint8_t *data, size_t len) {
    uint8_t p = 0;
    for (size_t i = 0; i < len; ++i) p ^= data[i];
    return p;
}
