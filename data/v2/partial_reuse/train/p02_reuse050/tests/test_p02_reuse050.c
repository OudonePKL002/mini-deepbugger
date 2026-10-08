#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "query_component.h"
#include "donor_component.h"

static void test_p02_f01_heap_init(void) {
    p02_heap_t h; p02_f01_heap_init(&h); assert(h.size == 0 && h.capacity == P02_MAX_CAP);
}

static void test_p02_f02_heap_sift_up(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f02_heap_sift_up(&h, 1); assert(h.data[0] == 5);
}

static void test_p02_f03_heap_sift_down(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 2; h.data[0] = 10; h.data[1] = 5; p02_f03_heap_sift_down(&h, 0); assert(h.data[0] == 5);
}

static void test_p02_f04_heap_push(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 0; assert(p02_f04_heap_push(&h, 42) && h.size == 1);
}

static void test_p02_f06_heap_peek(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 1; h.data[0] = 99; int32_t val = 0; assert(p02_f06_heap_peek(&h, &val) && val == 99);
}

static void test_p02_f10_heap_delete_at(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 3; h.data[0] = 10; h.data[1] = 20; h.data[2] = 30; assert(p02_f10_heap_delete_at(&h, 1) && h.size == 2);
}

static void test_p02_f13_heap_merge(void) {
    p02_heap_t h1, h2; memset(&h1, 0, sizeof(h1)); memset(&h2, 0, sizeof(h2)); h1.capacity = P02_MAX_CAP; h2.capacity = P02_MAX_CAP; h1.size = 1; h1.data[0] = 10; h2.size = 1; h2.data[0] = 5; assert(p02_f13_heap_merge(&h1, &h2) && h1.size == 2 && h1.data[0] == 5);
}

static void test_p02_f16_heap_clear(void) {
    p02_heap_t h; memset(&h, 0, sizeof(h)); h.capacity = P02_MAX_CAP; h.size = 5; p02_f16_heap_clear(&h); assert(h.size == 0);
}

static void test_p06_f02_tlv_encode_uint16(void) {
    uint8_t buf[16]; assert(p06_f02_tlv_encode_uint16(2, 0x1234, buf, 16) == 4);
}

static void test_p06_f04_tlv_encode_bytes(void) {
    uint8_t raw[2] = {1, 2}; uint8_t buf[16]; assert(p06_f04_tlv_encode_bytes(4, raw, 2, buf, 16) == 4);
}

static void test_p06_f07_tlv_decode_uint16(void) {
    uint8_t buf[4] = {2, 2, 0x12, 0x34}; uint16_t val; assert(p06_f07_tlv_decode_uint16(buf, 4, &val) && val == 0x1234);
}

static void test_p06_f08_tlv_decode_uint32(void) {
    uint8_t buf[6] = {3, 4, 0x12, 0x34, 0x56, 0x78}; uint32_t val; assert(p06_f08_tlv_decode_uint32(buf, 6, &val) && val == 0x12345678);
}

static void test_p06_f09_tlv_find_tag(void) {
    uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f09_tlv_find_tag(buf, 6, 2) == 3);
}

static void test_p06_f10_tlv_count_tags(void) {
    uint8_t buf[6] = {1, 1, 0xAA, 2, 1, 0xBB}; assert(p06_f10_tlv_count_tags(buf, 6) == 2);
}

static void test_p06_f14_packet_payload_pad(void) {
    uint8_t buf[16] = {'a', 'b'}; assert(p06_f14_packet_payload_pad(buf, 2, 4, 16) == 4);
}

static void test_p06_f15_packet_payload_unpad(void) {
    const uint8_t buf[4] = {'a', 'b', 2, 2}; assert(p06_f15_packet_payload_unpad(buf, 4) == 2);
}

int main(void) {
    test_p02_f01_heap_init();
    test_p02_f02_heap_sift_up();
    test_p02_f03_heap_sift_down();
    test_p02_f04_heap_push();
    test_p02_f06_heap_peek();
    test_p02_f10_heap_delete_at();
    test_p02_f13_heap_merge();
    test_p02_f16_heap_clear();
    test_p06_f02_tlv_encode_uint16();
    test_p06_f04_tlv_encode_bytes();
    test_p06_f07_tlv_decode_uint16();
    test_p06_f08_tlv_decode_uint32();
    test_p06_f09_tlv_find_tag();
    test_p06_f10_tlv_count_tags();
    test_p06_f14_packet_payload_pad();
    test_p06_f15_packet_payload_unpad();
    printf("PASS: test_p02_reuse050 passed.\n");
    return 0;
}
