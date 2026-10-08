#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P08_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P08_NOINLINE uint32_t p08_f01_adler32(const uint8_t *data, size_t len);
P08_NOINLINE uint32_t p08_f04_half_siphash(const uint8_t *data, size_t len, const uint8_t key[8]);
P08_NOINLINE uint32_t p08_f06_sdbm_hash(const char *str);
P08_NOINLINE uint32_t p08_f09_knuth_multiplicative(uint32_t val);
P08_NOINLINE uint32_t p08_f11_rotate_mix32(const uint8_t *data, size_t len);
P08_NOINLINE uint16_t p08_f12_crc16_usb(const uint8_t *data, size_t len);
P08_NOINLINE uint32_t p08_f15_hash_mix32(uint32_t h);
P08_NOINLINE uint32_t p08_f16_checksum_fold64_to_32(uint64_t val);

#endif /* TARGET_DONOR_COMPONENT_H */
