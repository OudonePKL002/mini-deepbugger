#include "donor_component.h"
#include <string.h>
#include <math.h>

P16_NOINLINE void p16_f01_btree_init(p16_btree_t *bt) {
    if (!bt) return;
    bt->pool_size = 0;
    bt->root = -1;
}

P16_NOINLINE int32_t p16_f04_btree_find_key_idx(const p16_node_t *node, int32_t key) {
    if (!node) return -1;
    for (size_t i = 0; i < node->num_keys; ++i) {
        if (node->keys[i] >= key) return (int32_t)i;
    }
    return (int32_t)node->num_keys;
}

P16_NOINLINE size_t p16_f11_btree_depth(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return 1;
    return 1 + p16_f11_btree_depth(bt, n->children[0]);
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
