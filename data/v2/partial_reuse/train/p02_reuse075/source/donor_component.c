#include "donor_component.h"
#include <string.h>
#include <math.h>

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
