#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p13_slip_framing.h"

int main(void) {
    const uint8_t raw[] = {0x01, 0xC0, 0x02, 0xDB, 0x03};
    uint8_t slip_enc[32];
    size_t enc_sz = p13_f01_slip_encode(raw, sizeof(raw), slip_enc, 32);
    assert(enc_sz > sizeof(raw));

    uint8_t slip_dec[32];
    size_t dec_sz = p13_f02_slip_decode(slip_enc, enc_sz, slip_dec, 32);
    assert(dec_sz == sizeof(raw));
    assert(memcmp(raw, slip_dec, sizeof(raw)) == 0);

    const uint8_t cobs_raw[] = {11, 22, 0, 33, 44};
    uint8_t cobs_enc[32];
    size_t c_enc_sz = p13_f03_cobs_encode(cobs_raw, sizeof(cobs_raw), cobs_enc, 32);
    assert(c_enc_sz > 0);
    uint8_t cobs_dec[32];
    size_t c_dec_sz = p13_f04_cobs_decode(cobs_enc, c_enc_sz, cobs_dec, 32);
    assert(c_dec_sz == sizeof(cobs_raw));
    assert(memcmp(cobs_raw, cobs_dec, sizeof(cobs_raw)) == 0);

    uint16_t fcs = p13_f05_fcs16_compute(raw, sizeof(raw));
    assert(p13_f06_fcs16_verify(raw, sizeof(raw), fcs));

    uint8_t hdr[6];
    assert(p13_f07_pack_header(hdr, 0x05, 100, 50) == 6);
    uint8_t type = 0; uint16_t seq = 0, plen = 0;
    assert(p13_f08_unpack_header(hdr, &type, &seq, &plen));
    assert(type == 0x05 && seq == 100 && plen == 50);

    assert(p13_f12_byte_swap16(0x1234) == 0x3412);
    assert(p13_f13_byte_swap32(0x12345678) == 0x78563412);

    p13_frame_buf_t fb;
    p13_f16_frame_buf_reset(&fb);
    assert(p13_f15_frame_buf_append(&fb, raw, sizeof(raw)));
    assert(fb.length == sizeof(raw));

    printf("PASS: p13_slip_framing unit tests passed.\n");
    return 0;
}
