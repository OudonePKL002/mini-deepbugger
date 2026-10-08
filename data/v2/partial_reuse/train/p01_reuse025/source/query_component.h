#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>

#define P01_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P01_NOINLINE uint32_t p01_f06_crc32_table_driven(const uint8_t *data, size_t len);
P01_NOINLINE int      p01_f11_luhn_validate(const char *digits);
P01_NOINLINE uint8_t  p01_f15_pearson_hash8(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f16_checksum_combine(uint16_t c16, uint32_t c32, uint8_t c8);

#endif /* TARGET_QUERY_COMPONENT_H */
