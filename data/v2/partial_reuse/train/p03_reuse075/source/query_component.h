#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P03_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P03_NOINLINE size_t p03_f01_str_trim(char *s);
P03_NOINLINE size_t p03_f02_str_split_delim(const char *s, char delim, char tokens[][32], size_t max_tok);
P03_NOINLINE size_t p03_f03_str_join(const char tokens[][32], size_t count, char sep, char *out_buf, size_t out_sz);
P03_NOINLINE bool   p03_f04_str_starts_with(const char *str, const char *prefix);
P03_NOINLINE bool   p03_f05_str_ends_with(const char *str, const char *suffix);
P03_NOINLINE size_t p03_f06_str_replace_char(char *s, char old_c, char new_c);
P03_NOINLINE size_t p03_f07_str_count_substr(const char *haystack, const char *needle);
P03_NOINLINE void   p03_f08_str_to_lower_ascii(char *s);
P03_NOINLINE void   p03_f09_str_to_upper_ascii(char *s);
P03_NOINLINE void   p03_f10_str_reverse(char *s);
P03_NOINLINE size_t p03_f15_str_format_hex(const uint8_t *bytes, size_t len, char *out_buf, size_t buf_sz);
P03_NOINLINE size_t p03_f16_str_parse_hex(const char *hex_str, uint8_t *out_bytes, size_t max_bytes);

#endif /* TARGET_QUERY_COMPONENT_H */
