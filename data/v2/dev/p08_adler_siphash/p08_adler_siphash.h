#ifndef P08_ADLER_SIPHASH_H
#define P08_ADLER_SIPHASH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P08_NOINLINE __attribute__((noinline))

P08_NOINLINE uint32_t p08_f01_adler32(const uint8_t *data, size_t len);
P08_NOINLINE void     p08_f02_siphash_round(uint64_t v[4]);
P08_NOINLINE uint64_t p08_f03_siphash24(const uint8_t *data, size_t len, const uint8_t key[16]);
P08_NOINLINE uint32_t p08_f04_half_siphash(const uint8_t *data, size_t len, const uint8_t key[8]);
P08_NOINLINE uint32_t p08_f05_djb2_hash(const char *str);
P08_NOINLINE uint32_t p08_f06_sdbm_hash(const char *str);
P08_NOINLINE uint32_t p08_f07_rabin_karp_rolling(uint32_t prev_h, uint8_t old_c, uint8_t new_c, uint32_t mult, uint32_t high_pow);
P08_NOINLINE uint32_t p08_f08_jenkins_lookup2(const uint8_t *k, size_t length, uint32_t initval);
P08_NOINLINE uint32_t p08_f09_knuth_multiplicative(uint32_t val);
P08_NOINLINE void     p08_f10_rot13_cipher(char *dst, const char *src, size_t len);
P08_NOINLINE uint32_t p08_f11_rotate_mix32(const uint8_t *data, size_t len);
P08_NOINLINE uint16_t p08_f12_crc16_usb(const uint8_t *data, size_t len);
P08_NOINLINE void     p08_f13_poly1305_clamp(uint8_t r[16]);
P08_NOINLINE uint32_t p08_f14_chash_simple(const uint8_t *data, size_t len, uint32_t seed);
P08_NOINLINE uint32_t p08_f15_hash_mix32(uint32_t h);
P08_NOINLINE uint32_t p08_f16_checksum_fold64_to_32(uint64_t val);

#endif
