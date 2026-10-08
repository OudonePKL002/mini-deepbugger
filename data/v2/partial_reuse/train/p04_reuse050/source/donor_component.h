#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>

#define P01_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P01_NOINLINE uint8_t  p01_f02_crc8_cdma(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f05_crc32_ieee(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f07_fletcher16(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f08_fletcher32(const uint16_t *data, size_t words);
P01_NOINLINE uint16_t p01_f09_bsd_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint32_t p01_f10_sysv_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f13_internet_checksum(const uint8_t *data, size_t len);
P01_NOINLINE uint16_t p01_f14_adler16_simple(const uint8_t *data, size_t len);

#endif /* TARGET_DONOR_COMPONENT_H */
