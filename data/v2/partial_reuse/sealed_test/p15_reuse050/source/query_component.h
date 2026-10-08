#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P15_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P15_NOINLINE uint32_t p15_f01_fnv1_32(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f02_fnv1a_32(const uint8_t *data, size_t len);
P15_NOINLINE uint64_t p15_f03_fnv1_64(const uint8_t *data, size_t len);
P15_NOINLINE uint64_t p15_f04_fnv1a_64(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f09_super_fast_hash(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f14_crc24_ble(const uint8_t *data, size_t len, uint32_t init_state);
P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2);
P15_NOINLINE uint8_t  p15_f16_checksum_parity_byte(const uint8_t *data, size_t len);

#endif /* TARGET_QUERY_COMPONENT_H */
