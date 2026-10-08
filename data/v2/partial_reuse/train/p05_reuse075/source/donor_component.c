#include "donor_component.h"
#include <string.h>
#include <math.h>

P02_NOINLINE void p02_f03_heap_sift_down(p02_heap_t *h, size_t idx) {
    if (!h) return;
    while (idx * 2 + 1 < h->size) {
        size_t left = idx * 2 + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (h->data[left] < h->data[smallest]) {
            smallest = left;
        }
        if (right < h->size && h->data[right] < h->data[smallest]) {
            smallest = right;
        }
        if (smallest != idx) {
            int32_t tmp = h->data[idx];
            h->data[idx] = h->data[smallest];
            h->data[smallest] = tmp;
            idx = smallest;
        } else {
            break;
        }
    }
}

P02_NOINLINE bool p02_f05_heap_pop_min(p02_heap_t *h, int32_t *out_val) {
    if (!h || h->size == 0) return false;
    if (out_val) *out_val = h->data[0];
    h->data[0] = h->data[h->size - 1];
    h->size--;
    if (h->size > 0) {
        p02_f03_heap_sift_down(h, 0);
    }
    return true;
}

P02_NOINLINE bool p02_f06_heap_peek(const p02_heap_t *h, int32_t *out_val) {
    if (!h || h->size == 0) return false;
    if (out_val) *out_val = h->data[0];
    return true;
}

P02_NOINLINE bool p02_f09_heap_replace(p02_heap_t *h, int32_t new_val, int32_t *old_val) {
    if (!h || h->size == 0) return false;
    if (old_val) *old_val = h->data[0];
    h->data[0] = new_val;
    p02_f03_heap_sift_down(h, 0);
    return true;
}
