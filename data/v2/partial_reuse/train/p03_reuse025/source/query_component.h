#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P03_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P03_NOINLINE size_t p03_f01_str_trim(char *s);
P03_NOINLINE bool   p03_f05_str_ends_with(const char *str, const char *suffix);
P03_NOINLINE void   p03_f08_str_to_lower_ascii(char *s);
P03_NOINLINE size_t p03_f13_str_unescape_c(const char *src, char *dst, size_t dst_sz);

#endif /* TARGET_QUERY_COMPONENT_H */
