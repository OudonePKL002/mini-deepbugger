#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P15_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P15_NOINLINE uint64_t p15_f03_fnv1_64(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f09_super_fast_hash(const uint8_t *data, size_t len);
P15_NOINLINE uint32_t p15_f11_dek_hash(const char *str);
P15_NOINLINE uint64_t p15_f15_hash_combine64(uint64_t h1, uint64_t h2);

#endif /* TARGET_QUERY_COMPONENT_H */
