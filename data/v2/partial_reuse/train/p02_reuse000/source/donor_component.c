#include "donor_component.h"
#include <string.h>
#include <math.h>

P06_NOINLINE size_t p06_f01_tlv_encode_uint8(uint8_t tag, uint8_t val, uint8_t *buf, size_t buf_sz) {
    if (!buf || buf_sz < 3) return 0;
    buf[0] = tag;
    buf[1] = 1;
    buf[2] = val;
    return 3;
}

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

P06_NOINLINE size_t p06_f04_tlv_encode_bytes(uint8_t tag, const uint8_t *val, size_t len, uint8_t *buf, size_t buf_sz) {
    if (!buf || !val || len > 255 || buf_sz < len + 2) return 0;
    buf[0] = tag;
    buf[1] = (uint8_t)len;
    for (size_t i = 0; i < len; ++i) {
        buf[2 + i] = val[i];
    }
    return len + 2;
}

P06_NOINLINE bool p06_f05_tlv_decode_header(const uint8_t *buf, size_t len, uint8_t *out_tag, uint8_t *out_len) {
    if (!buf || len < 2) return false;
    if (out_tag) *out_tag = buf[0];
    if (out_len) *out_len = buf[1];
    return true;
}

P06_NOINLINE bool p06_f06_tlv_decode_uint8(const uint8_t *buf, size_t len, uint8_t *out_val) {
    if (!buf || len < 3 || buf[1] != 1) return false;
    if (out_val) *out_val = buf[2];
    return true;
}

P06_NOINLINE bool p06_f07_tlv_decode_uint16(const uint8_t *buf, size_t len, uint16_t *out_val) {
    if (!buf || len < 4 || buf[1] != 2) return false;
    if (out_val) *out_val = (uint16_t)(((uint16_t)buf[2] << 8) | buf[3]);
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

P06_NOINLINE int p06_f09_tlv_find_tag(const uint8_t *buf, size_t len, uint8_t target_tag) {
    if (!buf) return -1;
    size_t offset = 0;
    while (offset + 2 <= len) {
        uint8_t tag = buf[offset];
        uint8_t l = buf[offset + 1];
        if (tag == target_tag) return (int)offset;
        offset += (2 + (size_t)l);
    }
    return -1;
}

P06_NOINLINE size_t p06_f10_tlv_count_tags(const uint8_t *buf, size_t len) {
    if (!buf) return 0;
    size_t count = 0;
    size_t offset = 0;
    while (offset + 2 <= len) {
        uint8_t l = buf[offset + 1];
        if (offset + 2 + l > len) break;
        count++;
        offset += (2 + (size_t)l);
    }
    return count;
}

P06_NOINLINE bool p06_f11_tlv_validate_buffer(const uint8_t *buf, size_t len) {
    if (!buf || len < 2) return false;
    size_t offset = 0;
    while (offset < len) {
        if (offset + 2 > len) return false;
        uint8_t l = buf[offset + 1];
        offset += (2 + (size_t)l);
    }
    return offset == len;
}

P06_NOINLINE size_t p06_f12_packet_header_build(uint16_t seq, uint16_t payload_len, uint8_t *hdr_out, size_t hdr_sz) {
    if (!hdr_out || hdr_sz < 6) return 0;
    hdr_out[0] = 0xAA; // Magic 1
    hdr_out[1] = 0x55; // Magic 2
    hdr_out[2] = (uint8_t)((seq >> 8) & 0xFF);
    hdr_out[3] = (uint8_t)(seq & 0xFF);
    hdr_out[4] = (uint8_t)((payload_len >> 8) & 0xFF);
    hdr_out[5] = (uint8_t)(payload_len & 0xFF);
    return 6;
}

P06_NOINLINE bool p06_f13_packet_header_parse(const uint8_t *hdr_in, size_t hdr_len, uint16_t *out_seq, uint16_t *out_payload_len) {
    if (!hdr_in || hdr_len < 6) return false;
    if (hdr_in[0] != 0xAA || hdr_in[1] != 0x55) return false;
    if (out_seq) *out_seq = (uint16_t)(((uint16_t)hdr_in[2] << 8) | hdr_in[3]);
    if (out_payload_len) *out_payload_len = (uint16_t)(((uint16_t)hdr_in[4] << 8) | hdr_in[5]);
    return true;
}

P06_NOINLINE size_t p06_f14_packet_payload_pad(uint8_t *buf, size_t data_len, size_t block_sz, size_t max_buf) {
    if (!buf || block_sz == 0 || block_sz > 32) return 0;
    size_t pad = block_sz - (data_len % block_sz);
    if (data_len + pad > max_buf) return 0;
    for (size_t i = 0; i < pad; ++i) {
        buf[data_len + i] = (uint8_t)pad;
    }
    return data_len + pad;
}

P06_NOINLINE size_t p06_f15_packet_payload_unpad(const uint8_t *buf, size_t padded_len) {
    if (!buf || padded_len == 0) return 0;
    uint8_t pad = buf[padded_len - 1];
    if (pad == 0 || (size_t)pad > padded_len) return 0;
    for (size_t i = 0; i < pad; ++i) {
        if (buf[padded_len - 1 - i] != pad) return 0;
    }
    return padded_len - pad;
}

P06_NOINLINE size_t p06_f16_packet_serialize(uint16_t seq, const uint8_t *payload, size_t p_len, uint8_t *wire_out, size_t max_out) {
    if (!wire_out || max_out < p_len + 7) return 0;
    size_t hdr_sz = p06_f12_packet_header_build(seq, (uint16_t)p_len, wire_out, max_out);
    if (hdr_sz == 0) return 0;
    for (size_t i = 0; i < p_len; ++i) {
        wire_out[hdr_sz + i] = payload[i];
    }
    uint8_t csum = 0;
    for (size_t i = 0; i < hdr_sz + p_len; ++i) {
        csum ^= wire_out[i];
    }
    wire_out[hdr_sz + p_len] = csum;
    return hdr_sz + p_len + 1;
}
