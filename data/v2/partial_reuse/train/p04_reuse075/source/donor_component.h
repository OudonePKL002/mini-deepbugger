#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>

#define P01_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P01_NOINLINE uint32_t p01_f06_crc32_table_driven(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f07_fletcher16(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f10_sysv_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f13_internet_checksum(const uint8_t *data, size_t len);

#endif /* TARGET_DONOR_COMPONENT_H */
