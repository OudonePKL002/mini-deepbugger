#include "query_component.h"
#include <string.h>
#include <math.h>

P02_NOINLINE void p02_f01_heap_init(p02_heap_t *h) {
    if (!h) return;
    h->size = 0;
    h->capacity = P02_MAX_CAP;
    for (size_t i = 0; i < P02_MAX_CAP; ++i) {
        h->data[i] = 0;
    }
}

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

P02_NOINLINE bool p02_f04_heap_push(p02_heap_t *h, int32_t val) {
    if (!h || h->size >= h->capacity) return false;
    h->data[h->size] = val;
    h->size++;
    p02_f02_heap_sift_up(h, h->size - 1);
    return true;
}

P02_NOINLINE bool p02_f06_heap_peek(const p02_heap_t *h, int32_t *out_val) {
    if (!h || h->size == 0) return false;
    if (out_val) *out_val = h->data[0];
    return true;
}

P02_NOINLINE bool p02_f10_heap_delete_at(p02_heap_t *h, size_t idx) {
    if (!h || idx >= h->size) return false;
    h->data[idx] = h->data[h->size - 1];
    h->size--;
    if (idx < h->size) {
        p02_f03_heap_sift_down(h, idx);
        p02_f02_heap_sift_up(h, idx);
    }
    return true;
}

P02_NOINLINE bool p02_f13_heap_merge(p02_heap_t *dst, const p02_heap_t *src) {
    if (!dst || !src) return false;
    if (dst->size + src->size > dst->capacity) return false;
    for (size_t i = 0; i < src->size; ++i) {
        dst->data[dst->size++] = src->data[i];
    }
    for (int p = (int)(dst->size / 2) - 1; p >= 0; --p) {
        p02_f03_heap_sift_down(dst, (size_t)p);
    }
    return true;
}

P02_NOINLINE void p02_f16_heap_clear(p02_heap_t *h) {
    if (!h) return;
    h->size = 0;
}
