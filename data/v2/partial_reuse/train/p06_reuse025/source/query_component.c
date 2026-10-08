#include "query_component.h"
#include <string.h>
#include <math.h>

P06_NOINLINE size_t p06_f02_tlv_encode_uint16(uint8_t tag, uint16_t val, uint8_t *buf, size_t buf_sz) {
    if (!buf || buf_sz < 4) return 0;
    buf[0] = tag;
    buf[1] = 2;
    buf[2] = (uint8_t)((val >> 8) & 0xFF);
    buf[3] = (uint8_t)(val & 0xFF);
    return 4;
}

P06_NOINLINE size_t p06_f03_tlv_encode_uint32(uint8_t tag, uint32_t val, uint8_t *buf, size_t buf_sz) {
    if (!buf || buf_sz < 6) return 0;
    buf[0] = tag;
    buf[1] = 4;
    buf[2] = (uint8_t)((val >> 24) & 0xFF);
    buf[3] = (uint8_t)((val >> 16) & 0xFF);
    buf[4] = (uint8_t)((val >> 8) & 0xFF);
    buf[5] = (uint8_t)(val & 0xFF);
    return 6;
}

P06_NOINLINE bool p06_f05_tlv_decode_header(const uint8_t *buf, size_t len, uint8_t *out_tag, uint8_t *out_len) {
    if (!buf || len < 2) return false;
    if (out_tag) *out_tag = buf[0];
    if (out_len) *out_len = buf[1];
    return true;
}

P06_NOINLINE bool p06_f08_tlv_decode_uint32(const uint8_t *buf, size_t len, uint32_t *out_val) {
    if (!buf || len < 6 || buf[1] != 4) return false;
    if (out_val) {
        *out_val = ((uint32_t)buf[2] << 24) |
                   ((uint32_t)buf[3] << 16) |
                   ((uint32_t)buf[4] << 8)  |
                   (uint32_t)buf[5];
    }
    return true;
}
