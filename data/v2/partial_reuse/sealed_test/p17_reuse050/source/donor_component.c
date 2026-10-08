#include "donor_component.h"
#include <string.h>
#include <math.h>

/* Support helper: traverse_rec */
static size_t traverse_rec(const p16_btree_t *bt, int32_t node_idx, int32_t *out_keys, size_t max_keys, size_t count) {
    if (!bt || node_idx < 0 || count >= max_keys) return count;
    const p16_node_t *n = &bt->pool[node_idx];
    for (size_t i = 0; i < n->num_keys; ++i) {
        if (!n->is_leaf) {
            count = traverse_rec(bt, n->children[i], out_keys, max_keys, count);
        }
        if (count < max_keys) out_keys[count++] = n->keys[i];
    }
    if (!n->is_leaf) {
        count = traverse_rec(bt, n->children[n->num_keys], out_keys, max_keys, count);
    }
    return count;
}

P16_NOINLINE void p16_f01_btree_init(p16_btree_t *bt) {
    if (!bt) return;
    bt->pool_size = 0;
    bt->root = -1;
}

P16_NOINLINE int32_t p16_f02_btree_alloc_node(p16_btree_t *bt, bool is_leaf) {
    if (!bt || bt->pool_size >= P16_MAX_NODES) return -1;
    int32_t idx = (int32_t)bt->pool_size++;
    bt->pool[idx].num_keys = 0;
    bt->pool[idx].is_leaf = is_leaf;
    for (size_t i = 0; i < P16_MAX_CHILDREN; ++i) {
        bt->pool[idx].children[i] = -1;
    }
    return idx;
}

P16_NOINLINE bool p16_f03_btree_node_is_full(const p16_node_t *node) {
    if (!node) return false;
    return node->num_keys == P16_MAX_KEYS;
}

P16_NOINLINE int32_t p16_f04_btree_find_key_idx(const p16_node_t *node, int32_t key) {
    if (!node) return -1;
    for (size_t i = 0; i < node->num_keys; ++i) {
        if (node->keys[i] >= key) return (int32_t)i;
    }
    return (int32_t)node->num_keys;
}

P16_NOINLINE void p16_f05_btree_split_child(p16_btree_t *bt, int32_t parent_idx, size_t child_pos, int32_t child_idx) {
    if (!bt || parent_idx < 0 || child_idx < 0) return;
    p16_node_t *y = &bt->pool[child_idx];
    int32_t z_idx = p16_f02_btree_alloc_node(bt, y->is_leaf);
    if (z_idx < 0) return;
    p16_node_t *z = &bt->pool[z_idx];
    y = &bt->pool[child_idx]; // re-fetch in case pointer moved
    p16_node_t *p = &bt->pool[parent_idx];

    z->num_keys = P16_BTREE_T - 1;
    for (size_t j = 0; j < P16_BTREE_T - 1; ++j) {
        z->keys[j] = y->keys[j + P16_BTREE_T];
    }
    if (!y->is_leaf) {
        for (size_t j = 0; j < P16_BTREE_T; ++j) {
            z->children[j] = y->children[j + P16_BTREE_T];
        }
    }
    y->num_keys = P16_BTREE_T - 1;

    for (int j = (int)p->num_keys; j >= (int)child_pos + 1; --j) {
        p->children[j + 1] = p->children[j];
    }
    p->children[child_pos + 1] = z_idx;

    for (int j = (int)p->num_keys - 1; j >= (int)child_pos; --j) {
        p->keys[j + 1] = p->keys[j];
    }
    p->keys[child_pos] = y->keys[P16_BTREE_T - 1];
    p->num_keys++;
}

P16_NOINLINE size_t p16_f09_btree_traverse(const p16_btree_t *bt, int32_t node_idx, int32_t *out_keys, size_t max_keys) {
    if (!bt || !out_keys || max_keys == 0) return 0;
    return traverse_rec(bt, node_idx, out_keys, max_keys, 0);
}

P16_NOINLINE int32_t p16_f13_btree_max_key(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return n->keys[n->num_keys - 1];
    return p16_f13_btree_max_key(bt, n->children[n->num_keys]);
}

P16_NOINLINE bool p16_f15_btree_is_valid_node(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0 || (size_t)node_idx >= bt->pool_size) return false;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->num_keys > P16_MAX_KEYS) return false;
    for (size_t i = 1; i < n->num_keys; ++i) {
        if (n->keys[i] <= n->keys[i - 1]) return false;
    }
    return true;
}
