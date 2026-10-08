#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: max_i32 */
static inline int32_t max_i32(int32_t a, int32_t b) {
    return (a > b) ? a : b;
}

/* Support helper: inorder_rec */
static size_t inorder_rec(const p09_tree_t *t, int32_t node, int32_t *out_keys, size_t max_keys, size_t count) {
    if (node < 0 || count >= max_keys) return count;
    count = inorder_rec(t, t->pool[node].left, out_keys, max_keys, count);
    if (count < max_keys) {
        out_keys[count++] = t->pool[node].key;
    }
    return inorder_rec(t, t->pool[node].right, out_keys, max_keys, count);
}

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

P09_NOINLINE int32_t p09_f08_avl_min_node(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return -1;
    int32_t curr = node;
    while (t->pool[curr].left >= 0) {
        curr = t->pool[curr].left;
    }
    return curr;
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

P09_NOINLINE size_t p09_f12_avl_inorder(const p09_tree_t *t, int32_t *out_keys, size_t max_keys) {
    if (!t || !out_keys || max_keys == 0) return 0;
    return inorder_rec(t, t->root, out_keys, max_keys, 0);
}

P09_NOINLINE size_t p09_f13_avl_preorder(const p09_tree_t *t, int32_t *out_keys, size_t max_keys) {
    if (!t || !out_keys || max_keys == 0) return 0;
    return preorder_rec(t, t->root, out_keys, max_keys, 0);
}

P09_NOINLINE size_t p09_f14_avl_count(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return 0;
    return 1 + p09_f14_avl_count(t, t->pool[node].left) + p09_f14_avl_count(t, t->pool[node].right);
}
