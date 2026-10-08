#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P03_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P03_NOINLINE size_t p03_f03_str_join(const char tokens[][32], size_t count, char sep, char *out_buf, size_t out_sz);
P03_NOINLINE bool   p03_f05_str_ends_with(const char *str, const char *suffix);
P03_NOINLINE size_t p03_f12_str_escape_c(const char *src, char *dst, size_t dst_sz);
P03_NOINLINE size_t p03_f15_str_format_hex(const uint8_t *bytes, size_t len, char *out_buf, size_t buf_sz);

#endif /* TARGET_DONOR_COMPONENT_H */
