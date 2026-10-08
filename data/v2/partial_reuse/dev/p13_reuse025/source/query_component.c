#include "query_component.h"
#include <string.h>
#include <math.h>

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
