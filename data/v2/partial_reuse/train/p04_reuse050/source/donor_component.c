#include "donor_component.h"
#include <string.h>
#include <math.h>

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
