#include "query_component.h"
#include <string.h>
#include <math.h>

P02_NOINLINE void p02_f02_heap_sift_up(p02_heap_t *h, size_t idx) {
    if (!h || idx >= h->size) return;
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (h->data[idx] < h->data[parent]) {
            int32_t tmp = h->data[idx];
            h->data[idx] = h->data[parent];
            h->data[parent] = tmp;
            idx = parent;
        } else {
            break;
        }
    }
}

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

P02_NOINLINE bool p02_f11_heap_increase_key(p02_heap_t *h, size_t idx, int32_t delta) {
    if (!h || idx >= h->size || delta < 0) return false;
    h->data[idx] += delta;
    p02_f03_heap_sift_down(h, idx);
    return true;
}

P02_NOINLINE bool p02_f14_heap_is_valid(const p02_heap_t *h) {
    if (!h || h->size > h->capacity) return false;
    for (size_t i = 0; i < h->size; ++i) {
        size_t l = 2 * i + 1;
        size_t r = 2 * i + 2;
        if (l < h->size && h->data[i] > h->data[l]) return false;
        if (r < h->size && h->data[i] > h->data[r]) return false;
    }
    return true;
}
