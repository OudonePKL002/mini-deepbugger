#include "query_component.h"
#include <string.h>
#include <math.h>

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

P13_NOINLINE size_t p13_f04_cobs_decode(const uint8_t *src, size_t src_len, uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || src_len == 0) return 0;
    size_t read_idx = 0;
    size_t write_idx = 0;

    while (read_idx < src_len) {
        uint8_t code = src[read_idx++];
        for (uint8_t i = 1; i < code; ++i) {
            if (read_idx >= src_len || write_idx >= dst_cap) return 0;
            dst[write_idx++] = src[read_idx++];
        }
        if (code < 0xFF && read_idx < src_len) {
            if (write_idx >= dst_cap) return 0;
            dst[write_idx++] = 0;
        }
    }
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

P13_NOINLINE bool p13_f06_fcs16_verify(const uint8_t *data, size_t len, uint16_t expected_fcs) {
    return p13_f05_fcs16_compute(data, len) == expected_fcs;
}

P13_NOINLINE size_t p13_f07_pack_header(uint8_t *dst, uint8_t type, uint16_t seq, uint16_t payload_len) {
    if (!dst) return 0;
    dst[0] = 0xAA; // Sync
    dst[1] = type;
    dst[2] = (uint8_t)(seq >> 8);
    dst[3] = (uint8_t)(seq & 0xFF);
    dst[4] = (uint8_t)(payload_len >> 8);
    dst[5] = (uint8_t)(payload_len & 0xFF);
    return 6;
}

P13_NOINLINE size_t p13_f09_bit_stuff_encode(const uint8_t *src, size_t src_bits, uint8_t *dst) {
    if (!src || !dst) return 0;
    size_t out_bits = 0;
    size_t ones_count = 0;

    for (size_t b = 0; b < src_bits; ++b) {
        int bit = (src[b / 8] >> (b % 8)) & 1;
        // set bit in dst
        if (bit) {
            dst[out_bits / 8] |= (1 << (out_bits % 8));
            ones_count++;
        } else {
            dst[out_bits / 8] &= ~(1 << (out_bits % 8));
            ones_count = 0;
        }
        out_bits++;
        if (ones_count == 5) { // stuff a zero
            dst[out_bits / 8] &= ~(1 << (out_bits % 8));
            out_bits++;
            ones_count = 0;
        }
    }
    return out_bits;
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

P13_NOINLINE bool p13_f11_frame_checksum_valid(const uint8_t *frame, size_t total_len) {
    if (!frame || total_len < 3) return false;
    uint16_t calc = p13_f05_fcs16_compute(frame, total_len - 2);
    uint16_t stored = ((uint16_t)frame[total_len - 2] << 8) | frame[total_len - 1];
    return calc == stored;
}

P13_NOINLINE uint16_t p13_f12_byte_swap16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

P13_NOINLINE uint32_t p13_f13_byte_swap32(uint32_t val) {
    return ((val >> 24) & 0xFF) |
           ((val >> 8)  & 0xFF00) |
           ((val << 8)  & 0xFF0000) |
           ((val << 24) & 0xFF000000);
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

P13_NOINLINE void p13_f16_frame_buf_reset(p13_frame_buf_t *fb) {
    if (!fb) return;
    fb->length = 0;
    fb->capacity = 256;
}
