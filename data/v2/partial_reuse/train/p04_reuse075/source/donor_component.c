#include "donor_component.h"
#include <string.h>
#include <math.h>

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

P01_NOINLINE uint32_t p01_f10_sysv_checksum(const uint8_t *data, size_t len) {
    uint32_t s = 0;
    for (size_t i = 0; i < len; ++i) {
        s += data[i];
    }
    uint32_t r = (s & 0xFFFF) + ((s >> 16) & 0xFFFF);
    return (r & 0xFFFF) + (r >> 16);
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
