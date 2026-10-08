#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P08_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P08_NOINLINE uint32_t p08_f01_adler32(const uint8_t *data, size_t len);
P08_NOINLINE void     p08_f02_siphash_round(uint64_t v[4]);
P08_NOINLINE uint32_t p08_f08_jenkins_lookup2(const uint8_t *k, size_t length, uint32_t initval);
P08_NOINLINE void     p08_f13_poly1305_clamp(uint8_t r[16]);

#endif /* TARGET_DONOR_COMPONENT_H */
