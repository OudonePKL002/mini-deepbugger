#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P08_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P08_NOINLINE uint32_t p08_f05_djb2_hash(const char *str);
P08_NOINLINE void     p08_f10_rot13_cipher(char *dst, const char *src, size_t len);
P08_NOINLINE void     p08_f13_poly1305_clamp(uint8_t r[16]);
P08_NOINLINE uint32_t p08_f16_checksum_fold64_to_32(uint64_t val);

#endif /* TARGET_QUERY_COMPONENT_H */
