#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>

#define P01_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P01_NOINLINE uint8_t  p01_f02_crc8_cdma(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f04_crc16_modbus(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f05_crc32_ieee(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f06_crc32_table_driven(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f07_fletcher16(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f09_bsd_checksum(const uint8_t *data, size_t len);
P01_NOINLINE int      p01_f11_luhn_validate(const char *digits);
P01_NOINLINE uint8_t  p01_f12_xor8_block(const uint8_t *data, size_t len, uint8_t key);
P01_NOINLINE uint16_t p01_f13_internet_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f14_adler16_simple(const uint8_t *data, size_t len);
P01_NOINLINE uint8_t  p01_f15_pearson_hash8(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f16_checksum_combine(uint16_t c16, uint32_t c32, uint8_t c8);

#endif /* TARGET_QUERY_COMPONENT_H */
