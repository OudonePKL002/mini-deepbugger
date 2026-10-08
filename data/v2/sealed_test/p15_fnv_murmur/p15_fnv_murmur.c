#include "p15_fnv_murmur.h"

static inline uint32_t rotl32(uint32_t x, int b) {
    return (x << b) | (x >> (32 - b));
}

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

P15_NOINLINE uint32_t p15_f05_murmur3_32_scramble(uint32_t k) {
    k *= 0xcc9e2d51;
    k = rotl32(k, 15);
    k *= 0x1b873593;
    return k;
}

P15_NOINLINE uint32_t p15_f06_murmur3_32_fmix(uint32_t h) {
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

P15_NOINLINE uint32_t p15_f07_murmur3_32(const uint8_t *data, size_t len, uint32_t seed) {
    uint32_t h = seed;
    size_t nblocks = len / 4;
    for (size_t i = 0; i < nblocks; ++i) {
        uint32_t k = (uint32_t)data[i*4] |
                     ((uint32_t)data[i*4+1] << 8) |
                     ((uint32_t)data[i*4+2] << 16) |
                     ((uint32_t)data[i*4+3] << 24);
        k = p15_f05_murmur3_32_scramble(k);
        h ^= k;
        h = rotl32(h, 13);
        h = h * 5 + 0xe6546b64;
    }
    const uint8_t *tail = data + nblocks * 4;
    uint32_t k1 = 0;
    switch (len & 3) {
        case 3: k1 ^= ((uint32_t)tail[2]) << 16; /* fallthrough */
        case 2: k1 ^= ((uint32_t)tail[1]) << 8;  /* fallthrough */
        case 1: k1 ^= (uint32_t)tail[0];
                k1 = p15_f05_murmur3_32_scramble(k1);
                h ^= k1;
        default: break;
    }
    h ^= (uint32_t)len;
    return p15_f06_murmur3_32_fmix(h);
}

P15_NOINLINE uint32_t p15_f08_jenkins_one_at_a_time(const uint8_t *key, size_t len) {
    uint32_t hash = 0;
    for (size_t i = 0; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
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

P15_NOINLINE uint32_t p15_f10_elf_hash(const char *str) {
    uint32_t h = 0, g;
    if (!str) return 0;
    while (*str) {
        h = (h << 4) + (uint8_t)(*str++);
        g = h & 0xf0000000L;
        if (g != 0) h ^= (g >> 24);
        h &= ~g;
    }
    return h;
}

P15_NOINLINE uint32_t p15_f11_dek_hash(const char *str) {
    uint32_t hash = 5381;
    if (!str) return 0;
    while (*str) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (uint8_t)(*str++);
    }
    return hash;
}

P15_NOINLINE uint32_t p15_f12_bp_hash(const char *str) {
    uint32_t hash = 0;
    if (!str) return 0;
    while (*str) {
        hash = (hash << 7) ^ (uint8_t)(*str++);
    }
    return hash;
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
