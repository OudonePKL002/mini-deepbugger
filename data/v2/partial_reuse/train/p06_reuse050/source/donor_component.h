#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P03_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P03_NOINLINE size_t p03_f01_str_trim(char *s);
P03_NOINLINE size_t p03_f03_str_join(const char tokens[][32], size_t count, char sep, char *out_buf, size_t out_sz);
P03_NOINLINE size_t p03_f06_str_replace_char(char *s, char old_c, char new_c);
P03_NOINLINE size_t p03_f07_str_count_substr(const char *haystack, const char *needle);
P03_NOINLINE void   p03_f09_str_to_upper_ascii(char *s);
P03_NOINLINE void   p03_f10_str_reverse(char *s);
P03_NOINLINE size_t p03_f11_str_levenshtein(const char *s1, const char *s2);
P03_NOINLINE bool   p03_f14_str_parse_int(const char *s, int32_t *out_val);

#endif /* TARGET_DONOR_COMPONENT_H */
