#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: max_i32 */
static inline int32_t max_i32(int32_t a, int32_t b) {
    return (a > b) ? a : b;
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

P09_NOINLINE int32_t p09_f05_avl_rotate_left(p09_tree_t *t, int32_t x) {
    if (!t || x < 0) return -1;
    int32_t y = t->pool[x].right;
    if (y < 0) return x;
    int32_t t2 = t->pool[y].left;

    t->pool[y].left = x;
    t->pool[x].right = t2;

    t->pool[x].height = max_i32(p09_f02_avl_height(t, t->pool[x].left), p09_f02_avl_height(t, t->pool[x].right)) + 1;
    t->pool[y].height = max_i32(p09_f02_avl_height(t, t->pool[y].left), p09_f02_avl_height(t, t->pool[y].right)) + 1;

    return y;
}

P09_NOINLINE int32_t p09_f08_avl_min_node(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return -1;
    int32_t curr = node;
    while (t->pool[curr].left >= 0) {
        curr = t->pool[curr].left;
    }
    return curr;
}

P09_NOINLINE int32_t p09_f09_avl_delete_internal(p09_tree_t *t, int32_t node, int32_t key) {
    if (node < 0) return -1;
    if (key < t->pool[node].key) {
        t->pool[node].left = p09_f09_avl_delete_internal(t, t->pool[node].left, key);
    } else if (key > t->pool[node].key) {
        t->pool[node].right = p09_f09_avl_delete_internal(t, t->pool[node].right, key);
    } else {
        if (t->pool[node].left < 0 || t->pool[node].right < 0) {
            int32_t temp = (t->pool[node].left >= 0) ? t->pool[node].left : t->pool[node].right;
            if (temp < 0) {
                node = -1;
            } else {
                t->pool[node] = t->pool[temp];
            }
        } else {
            int32_t temp = p09_f08_avl_min_node(t, t->pool[node].right);
            t->pool[node].key = t->pool[temp].key;
            t->pool[node].value = t->pool[temp].value;
            t->pool[node].right = p09_f09_avl_delete_internal(t, t->pool[node].right, t->pool[temp].key);
        }
    }
    if (node < 0) return -1;

    t->pool[node].height = 1 + max_i32(p09_f02_avl_height(t, t->pool[node].left), p09_f02_avl_height(t, t->pool[node].right));
    int32_t balance = p09_f03_avl_balance_factor(t, node);

    if (balance > 1 && p09_f03_avl_balance_factor(t, t->pool[node].left) >= 0)
        return p09_f04_avl_rotate_right(t, node);
    if (balance > 1 && p09_f03_avl_balance_factor(t, t->pool[node].left) < 0) {
        t->pool[node].left = p09_f05_avl_rotate_left(t, t->pool[node].left);
        return p09_f04_avl_rotate_right(t, node);
    }
    if (balance < -1 && p09_f03_avl_balance_factor(t, t->pool[node].right) <= 0)
        return p09_f05_avl_rotate_left(t, node);
    if (balance < -1 && p09_f03_avl_balance_factor(t, t->pool[node].right) > 0) {
        t->pool[node].right = p09_f04_avl_rotate_right(t, t->pool[node].right);
        return p09_f05_avl_rotate_left(t, node);
    }
    return node;
}

P09_NOINLINE bool p09_f10_avl_delete(p09_tree_t *t, int32_t key) {
    if (!t || t->root < 0) return false;
    t->root = p09_f09_avl_delete_internal(t, t->root, key);
    return true;
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

P09_NOINLINE size_t p09_f14_avl_count(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return 0;
    return 1 + p09_f14_avl_count(t, t->pool[node].left) + p09_f14_avl_count(t, t->pool[node].right);
}

P09_NOINLINE bool p09_f15_avl_is_balanced(const p09_tree_t *t, int32_t node) {
    if (!t || node < 0) return true;
    int32_t bf = p09_f03_avl_balance_factor(t, node);
    if (bf < -1 || bf > 1) return false;
    return p09_f15_avl_is_balanced(t, t->pool[node].left) && p09_f15_avl_is_balanced(t, t->pool[node].right);
}

P09_NOINLINE void p09_f16_avl_clear(p09_tree_t *t) {
    if (!t) return;
    t->pool_size = 0;
    t->root = -1;
}
