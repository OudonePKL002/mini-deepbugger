#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P08_NOINLINE __attribute__((noinline))

/* Benchmark Function Prototypes */
P08_NOINLINE uint32_t p08_f01_adler32(const uint8_t *data, size_t len);
P08_NOINLINE void     p08_f02_siphash_round(uint64_t v[4]);
P08_NOINLINE uint32_t p08_f04_half_siphash(const uint8_t *data, size_t len, const uint8_t key[8]);
P08_NOINLINE uint32_t p08_f06_sdbm_hash(const char *str);
P08_NOINLINE uint32_t p08_f07_rabin_karp_rolling(uint32_t prev_h, uint8_t old_c, uint8_t new_c, uint32_t mult, uint32_t high_pow);
P08_NOINLINE void     p08_f10_rot13_cipher(char *dst, const char *src, size_t len);
P08_NOINLINE uint16_t p08_f12_crc16_usb(const uint8_t *data, size_t len);
P08_NOINLINE uint32_t p08_f14_chash_simple(const uint8_t *data, size_t len, uint32_t seed);

#endif /* TARGET_QUERY_COMPONENT_H */
