#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>

#define P01_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P01_NOINLINE uint8_t  p01_f01_crc8_smbus(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f03_crc16_ccitt(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f04_crc16_modbus(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f07_fletcher16(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f08_fletcher32(const uint16_t *data, size_t words);
P01_NOINLINE int      p01_f11_luhn_validate(const char *digits);
P01_NOINLINE uint16_t p01_f13_internet_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint8_t  p01_f15_pearson_hash8(const uint8_t *data, size_t len);

#endif /* TARGET_QUERY_COMPONENT_H */
