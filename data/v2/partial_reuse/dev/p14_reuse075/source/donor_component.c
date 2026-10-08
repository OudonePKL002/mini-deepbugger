#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: preorder_rec */
static size_t preorder_rec(const p09_tree_t *t, int32_t node, int32_t *out_keys, size_t max_keys, size_t count) {
    if (node < 0 || count >= max_keys) return count;
    if (count < max_keys) out_keys[count++] = t->pool[node].key;
    count = preorder_rec(t, t->pool[node].left, out_keys, max_keys, count);
    return preorder_rec(t, t->pool[node].right, out_keys, max_keys, count);
}

P09_NOINLINE void p09_f01_avl_init(p09_tree_t *t) {
    if (!t) return;
    t->pool_size = 0;
    t->root = -1;
}

P09_NOINLINE int32_t p09_f02_avl_height(const p09_tree_t *t, int32_t idx) {
    if (!t || idx < 0 || (size_t)idx >= t->pool_size) return 0;
    return t->pool[idx].height;
}

P09_NOINLINE bool p09_f11_avl_search(const p09_tree_t *t, int32_t key, int32_t *out_val) {
    if (!t) return false;
    int32_t curr = t->root;
    while (curr >= 0) {
        if (key == t->pool[curr].key) {
            if (out_val) *out_val = t->pool[curr].value;
            return true;
        } else if (key < t->pool[curr].key) {
            curr = t->pool[curr].left;
        } else {
            curr = t->pool[curr].right;
        }
    }
    return false;
}

P09_NOINLINE size_t p09_f13_avl_preorder(const p09_tree_t *t, int32_t *out_keys, size_t max_keys) {
    if (!t || !out_keys || max_keys == 0) return 0;
    return preorder_rec(t, t->root, out_keys, max_keys, 0);
}
