#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P15_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P15_NOINLINE uint32_t p15_f01_fnv1_32(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f02_fnv1a_32(const uint8_t *data, size_t len);
P15_NOINLINE uint64_t p15_f03_fnv1_64(const uint8_t *data, size_t len);
P15_NOINLINE uint64_t p15_f04_fnv1a_64(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f05_murmur3_32_scramble(uint32_t k);
P15_NOINLINE uint32_t p15_f06_murmur3_32_fmix(uint32_t h);
P15_NOINLINE uint32_t p15_f08_jenkins_one_at_a_time(const uint8_t *key, size_t len);
P15_NOINLINE uint32_t p15_f09_super_fast_hash(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f10_elf_hash(const char *str);
P15_NOINLINE uint32_t p15_f13_ap_hash(const char *str);
P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2);
P15_NOINLINE uint8_t  p15_f16_checksum_parity_byte(const uint8_t *data, size_t len);

#endif /* TARGET_DONOR_COMPONENT_H */
