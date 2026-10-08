#include "donor_component.h"
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

P02_NOINLINE void p02_f07_heapify_array(p02_heap_t *h, const int32_t *arr, size_t n) {
    if (!h || !arr) return;
    p02_f01_heap_init(h);
    size_t count = (n > h->capacity) ? h->capacity : n;
    for (size_t i = 0; i < count; ++i) {
        h->data[i] = arr[i];
    }
    h->size = count;
    if (count > 1) {
        for (int p = (int)(count / 2) - 1; p >= 0; --p) {
            p02_f03_heap_sift_down(h, (size_t)p);
        }
    }
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

P02_NOINLINE void p02_f16_heap_clear(p02_heap_t *h) {
    if (!h) return;
    h->size = 0;
}
