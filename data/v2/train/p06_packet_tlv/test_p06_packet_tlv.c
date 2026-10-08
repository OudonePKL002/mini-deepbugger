#include <stdio.h>
#include <assert.h>
#include "p06_packet_tlv.h"

int main(void) {
    uint8_t buf[128];
    size_t s1 = p06_f01_tlv_encode_uint8(0x01, 0xFE, buf, sizeof(buf));
    assert(s1 == 3);

    size_t s2 = p06_f02_tlv_encode_uint16(0x02, 0x1234, buf + s1, sizeof(buf) - s1);
    assert(s2 == 4);

    size_t s3 = p06_f03_tlv_encode_uint32(0x03, 0xDEADBEEF, buf + s1 + s2, sizeof(buf) - s1 - s2);
    assert(s3 == 6);

    uint8_t raw[3] = {0xAA, 0xBB, 0xCC};
    size_t s4 = p06_f04_tlv_encode_bytes(0x04, raw, 3, buf + s1 + s2 + s3, sizeof(buf) - s1 - s2 - s3);
    assert(s4 == 5);

    size_t total = s1 + s2 + s3 + s4;
    assert(p06_f11_tlv_validate_buffer(buf, total) == true);
    assert(p06_f10_tlv_count_tags(buf, total) == 4);

    int idx = p06_f09_tlv_find_tag(buf, total, 0x03);
    assert(idx == (int)(s1 + s2));

    uint8_t tag, len;
    assert(p06_f05_tlv_decode_header(buf, total, &tag, &len) && tag == 0x01 && len == 1);

    uint8_t v8 = 0;
    assert(p06_f06_tlv_decode_uint8(buf, total, &v8) && v8 == 0xFE);

    uint16_t v16 = 0;
    assert(p06_f07_tlv_decode_uint16(buf + s1, total - s1, &v16) && v16 == 0x1234);

    uint32_t v32 = 0;
    assert(p06_f08_tlv_decode_uint32(buf + s1 + s2, total - s1 - s2, &v32) && v32 == 0xDEADBEEF);

    uint8_t hdr[8];
    assert(p06_f12_packet_header_build(101, 20, hdr, sizeof(hdr)) == 6);
    uint16_t q_seq, q_len;
    assert(p06_f13_packet_header_parse(hdr, 6, &q_seq, &q_len) && q_seq == 101 && q_len == 20);

    uint8_t pbuf[32] = "abcdefgh";
    size_t padded = p06_f14_packet_payload_pad(pbuf, 8, 8, sizeof(pbuf));
    assert(padded == 16);
    assert(p06_f15_packet_payload_unpad(pbuf, padded) == 8);

    uint8_t wire[64];
    size_t w_sz = p06_f16_packet_serialize(42, (const uint8_t *)"hello", 5, wire, sizeof(wire));
    assert(w_sz == 6 + 5 + 1);

    printf("PASS: p06_packet_tlv unit tests passed.\n");
    return 0;
}
