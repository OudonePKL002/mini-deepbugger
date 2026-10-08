#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "p20_ring_frame.h"

int main(void) {
    p20_ring_t r;
    p20_f01_ring_init(&r);
    assert(p20_f02_ring_is_empty(&r));

    const uint8_t payload[] = "FrameHeaderData123";
    size_t sz = sizeof(payload) - 1;

    assert(p20_f08_ring_write_slice(&r, payload, sz) == sz);
    assert(p20_f04_ring_available(&r) == sz);
    assert(p20_f14_ring_high_watermark(&r) == sz);

    uint8_t b = 0;
    assert(p20_f10_ring_peek_byte(&r, 0, &b) && b == 'F');

    const uint8_t marker[] = "Data";
    int64_t mpos = p20_f12_ring_find_marker(&r, marker, 4);
    assert(mpos >= 0);

    uint8_t out[32];
    assert(p20_f13_ring_linearize(&r, out, 32) == sz);
    assert(memcmp(out, payload, sz) == 0);

    assert(p20_f07_ring_read_byte(&r, &b) && b == 'F');
    assert(p20_f04_ring_available(&r) == sz - 1);

    assert(p20_f11_ring_discard(&r, 5) == 5);
    p20_f16_ring_clear(&r);
    assert(p20_f02_ring_is_empty(&r));

    printf("PASS: p20_ring_frame unit tests passed.\n");
    return 0;
}
