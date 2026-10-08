#include "query_component.h"
#include <string.h>
#include <math.h>

/* Support helper: max_i32 */
static inline int32_t max_i32(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}

P09_NOINLINE int32_t p09_f02_avl_height(const p09_tree_t *t, int32_t idx) {
    if (!t || idx < 0 || (size_t)idx >= t->pool_size) return 0;
    return t->pool[idx].height;
}

P09_NOINLINE int32_t p09_f03_avl_balance_factor(const p09_tree_t *t, int32_t idx) {
    if (!t || idx < 0 || (size_t)idx >= t->pool_size) return 0;
    return p09_f02_avl_height(t, t->pool[idx].left) - p09_f02_avl_height(t, t->pool[idx].right);
}

P09_NOINLINE int32_t p09_f04_avl_rotate_right(p09_tree_t *t, int32_t y) {
    if (!t || y < 0) return -1;
    int32_t x = t->pool[y].left;
    if (x < 0) return y;
    int32_t t2 = t->pool[x].right;

    t->pool[x].right = y;
    t->pool[y].left = t2;

    t->pool[y].height = max_i32(p09_f02_avl_height(t, t->pool[y].left), p09_f02_avl_height(t, t->pool[y].right)) + 1;
    t->pool[x].height = max_i32(p09_f02_avl_height(t, t->pool[x].left), p09_f02_avl_height(t, t->pool[x].right)) + 1;

    return x;
}

P09_NOINLINE size_t p09_f14_avl_count(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return 0;
    return 1 + p09_f14_avl_count(t, t->pool[node].left) + p09_f14_avl_count(t, t->pool[node].right);
}
