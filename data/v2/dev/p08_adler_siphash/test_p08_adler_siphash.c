#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p08_adler_siphash.h"

int main(void) {
    const uint8_t sample[] = "DevBenchmarkCorpus2026";
    size_t len = sizeof(sample) - 1;

    uint32_t adler = p08_f01_adler32(sample, len);
    assert(adler != 0);

    uint8_t key16[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    uint64_t sip = p08_f03_siphash24(sample, len, key16);
    assert(sip != 0);

    uint8_t key8[8] = {1,2,3,4,5,6,7,8};
    uint32_t hsip = p08_f04_half_siphash(sample, len, key8);
    assert(hsip != 0);

    uint32_t djb = p08_f05_djb2_hash("test_string");
    uint32_t sdbm = p08_f06_sdbm_hash("test_string");
    assert(djb != 0 && sdbm != 0 && djb != sdbm);

    uint32_t rk = p08_f07_rabin_karp_rolling(100, 'a', 'b', 31, 961);
    assert(rk != 0);

    uint32_t jl = p08_f08_jenkins_lookup2(sample, len, 42);
    assert(jl != 0);

    uint32_t km = p08_f09_knuth_multiplicative(12345);
    assert(km != 0);

    char rot_buf[16];
    p08_f10_rot13_cipher(rot_buf, "Hello", 5);
    rot_buf[5] = '\0';
    assert(strcmp(rot_buf, "Uryyb") == 0);

    uint32_t rot_mix = p08_f11_rotate_mix32(sample, len);
    assert(rot_mix != 0);
    assert(p08_f11_rotate_mix32(sample, len) == rot_mix);

    uint16_t crc_usb = p08_f12_crc16_usb(sample, len);
    assert(crc_usb != 0);

    uint8_t poly_key[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    p08_f13_poly1305_clamp(poly_key);
    assert((poly_key[3] & 15) == poly_key[3]);

    uint32_t ch = p08_f14_chash_simple(sample, len, 999);
    uint32_t mx = p08_f15_hash_mix32(ch);
    assert(mx != 0);

    uint32_t fld = p08_f16_checksum_fold64_to_32(sip);
    assert(fld != 0);

    printf("PASS: p08_adler_siphash unit tests passed.\n");
    return 0;
}
