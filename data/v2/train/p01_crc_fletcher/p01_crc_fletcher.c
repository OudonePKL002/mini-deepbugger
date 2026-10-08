#include "p01_crc_fletcher.h"

P01_NOINLINE uint8_t p01_f01_crc8_smbus(const uint8_t *data, size_t len) {
    uint8_t crc = 0x00;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x80) {
                crc = (uint8_t)((crc << 1) ^ 0x07);
            } else {
                crc = (uint8_t)(crc << 1);
            }
        }
    }
    return crc;
}

P01_NOINLINE uint8_t p01_f02_crc8_cdma(const uint8_t *data, size_t len) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x80) {
                crc = (uint8_t)((crc << 1) ^ 0x9B);
            } else {
                crc = (uint8_t)(crc << 1);
            }
        }
    }
    return (uint8_t)(crc ^ 0xFF);
}

P01_NOINLINE uint16_t p01_f03_crc16_ccitt(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)((uint16_t)data[i] << 8);
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x8000) {
                crc = (uint16_t)((crc << 1) ^ 0x1021);
            } else {
                crc = (uint16_t)(crc << 1);
            }
        }
    }
    return crc;
}

P01_NOINLINE uint16_t p01_f04_crc16_modbus(const uint8_t *data, size_t len) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint16_t)data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 0x0001) {
                crc = (uint16_t)((crc >> 1) ^ 0xA001);
            } else {
                crc = (uint16_t)(crc >> 1);
            }
        }
    }
    return crc;
}

P01_NOINLINE uint32_t p01_f05_crc32_ieee(const uint8_t *data, size_t len) {
    uint32_t crc = 0xFFFFFFFFU;
    for (size_t i = 0; i < len; ++i) {
        crc ^= (uint32_t)data[i];
        for (int b = 0; b < 8; ++b) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320U;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc ^ 0xFFFFFFFFU;
}

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

P01_NOINLINE uint16_t p01_f07_fletcher16(const uint8_t *data, size_t len) {
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    for (size_t i = 0; i < len; ++i) {
        sum1 = (uint16_t)((sum1 + data[i]) % 255);
        sum2 = (uint16_t)((sum2 + sum1) % 255);
    }
    return (uint16_t)((sum2 << 8) | sum1);
}

P01_NOINLINE uint32_t p01_f08_fletcher32(const uint16_t *data, size_t words) {
    uint32_t c0 = 0;
    uint32_t c1 = 0;
    for (size_t i = 0; i < words; ++i) {
        c0 = (c0 + data[i]) % 65535U;
        c1 = (c1 + c0) % 65535U;
    }
    return (c1 << 16) | c0;
}

P01_NOINLINE uint16_t p01_f09_bsd_checksum(const uint8_t *data, size_t len) {
    uint16_t checksum = 0;
    for (size_t i = 0; i < len; ++i) {
        checksum = (uint16_t)((checksum >> 1) + ((checksum & 1) << 15));
        checksum = (uint16_t)(checksum + data[i]);
    }
    return checksum;
}

P01_NOINLINE uint32_t p01_f10_sysv_checksum(const uint8_t *data, size_t len) {
    uint32_t s = 0;
    for (size_t i = 0; i < len; ++i) {
        s += data[i];
    }
    uint32_t r = (s & 0xFFFF) + ((s >> 16) & 0xFFFF);
    return (r & 0xFFFF) + (r >> 16);
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

P01_NOINLINE uint8_t p01_f12_xor8_block(const uint8_t *data, size_t len, uint8_t key) {
    uint8_t acc = key;
    for (size_t i = 0; i < len; ++i) {
        acc = (uint8_t)(acc ^ (data[i] + (uint8_t)i));
    }
    return acc;
}

P01_NOINLINE uint16_t p01_f13_internet_checksum(const uint8_t *data, size_t len) {
    uint32_t sum = 0;
    size_t i = 0;
    while (i + 1 < len) {
        uint16_t word = (uint16_t)(((uint16_t)data[i] << 8) | data[i + 1]);
        sum += word;
        i += 2;
    }
    if (i < len) {
        sum += (uint16_t)((uint16_t)data[i] << 8);
    }
    while (sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

P01_NOINLINE uint16_t p01_f14_adler16_simple(const uint8_t *data, size_t len) {
    uint16_t a = 1;
    uint16_t b = 0;
    for (size_t i = 0; i < len; ++i) {
        a = (uint16_t)((a + data[i]) % 251);
        b = (uint16_t)((b + a) % 251);
    }
    return (uint16_t)((b << 8) | a);
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
