#include "donor_component.h"
#include <string.h>
#include <math.h>

P13_NOINLINE size_t p13_f01_slip_encode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || dst_cap < 2) return 0;
    size_t out = 0;
    dst[out++] = P13_SLIP_END;
    for (size_t i = 0; i < src_len; ++i) {
        if (out >= dst_cap - 2) return 0;
        if (src[i] == P13_SLIP_END) {
            dst[out++] = P13_SLIP_ESC;
            dst[out++] = P13_SLIP_ESC_END;
        } else if (src[i] == P13_SLIP_ESC) {
            dst[out++] = P13_SLIP_ESC;
            dst[out++] = P13_SLIP_ESC_ESC;
        } else {
            dst[out++] = src[i];
        }
    }
    if (out >= dst_cap) return 0;
    dst[out++] = P13_SLIP_END;
    return out;
}

P13_NOINLINE size_t p13_f02_slip_decode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst) return 0;
    size_t out = 0;
    for (size_t i = 0; i < src_len; ++i) {
        if (src[i] == P13_SLIP_END) continue;
        if (out >= dst_cap) return 0;
        if (src[i] == P13_SLIP_ESC) {
            if (++i >= src_len) return 0;
            if (src[i] == P13_SLIP_ESC_END) dst[out++] = P13_SLIP_END;
            else if (src[i] == P13_SLIP_ESC_ESC) dst[out++] = P13_SLIP_ESC;
            else dst[out++] = src[i];
        } else {
            dst[out++] = src[i];
        }
    }
    return out;
}

P13_NOINLINE size_t p13_f03_cobs_encode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || dst_cap < src_len + 2) return 0;
    size_t read_idx = 0;
    size_t write_idx = 1;
    size_t code_idx = 0;
    uint8_t code = 1;

    while (read_idx < src_len) {
        if (src[read_idx] == 0) {
            dst[code_idx] = code;
            code = 1;
            code_idx = write_idx++;
            read_idx++;
        } else {
            dst[write_idx++] = src[read_idx++];
            code++;
            if (code == 0xFF) {
                dst[code_idx] = code;
                code = 1;
                code_idx = write_idx++;
            }
        }
    }
    dst[code_idx] = code;
    return write_idx;
}

P13_NOINLINE uint16_t p13_f05_fcs16_compute(const uint8_t *data, size_t len) {
    uint16_t fcs = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        fcs ^= (uint16_t)data[i];
        for (int b = 0; b < 8; ++b) {
            if (fcs & 1) fcs = (fcs >> 1) ^ 0x8408;
            else fcs >>= 1;
        }
    }
    return fcs ^ 0xFFFF;
}

P13_NOINLINE size_t p13_f10_bit_stuff_decode(const uint8_t *src, size_t stuffed_bits, uint8_t *dst) {
    if (!src || !dst) return 0;
    size_t out_bits = 0;
    size_t ones_count = 0;

    for (size_t b = 0; b < stuffed_bits; ++b) {
        int bit = (src[b / 8] >> (b % 8)) & 1;
        if (bit) {
            dst[out_bits / 8] |= (1 << (out_bits % 8));
            ones_count++;
            out_bits++;
        } else {
            if (ones_count == 5) {
                // stuffed bit, drop it!
                ones_count = 0;
            } else {
                dst[out_bits / 8] &= ~(1 << (out_bits % 8));
                ones_count = 0;
                out_bits++;
            }
        }
    }
    return out_bits;
}

P13_NOINLINE uint16_t p13_f12_byte_swap16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

P13_NOINLINE bool p13_f14_parse_tlv_field(const uint8_t *data, size_t len, uint8_t *tag, uint8_t *v_len, const uint8_t **val) {
    if (!data || len < 2) return false;
    uint8_t t = data[0];
    uint8_t l = data[1];
    if (len < (size_t)(2 + l)) return false;
    if (tag) *tag = t;
    if (v_len) *v_len = l;
    if (val) *val = &data[2];
    return true;
}

P13_NOINLINE bool p13_f15_frame_buf_append(p13_frame_buf_t *fb, const uint8_t *data, size_t len) {
    if (!fb || !data || fb->length + len > fb->capacity) return false;
    memcpy(&fb->buffer[fb->length], data, len);
    fb->length += len;
    return true;
}
