#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P15_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P15_NOINLINE uint32_t p15_f02_fnv1a_32(const uint8_t *data, size_t len);
P15_NOINLINE uint64_t p15_f04_fnv1a_64(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f13_ap_hash(const char *str);
P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2);

#endif /* TARGET_DONOR_COMPONENT_H */
