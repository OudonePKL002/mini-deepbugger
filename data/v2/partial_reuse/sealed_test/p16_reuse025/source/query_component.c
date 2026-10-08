#include "query_component.h"
#include <string.h>
#include <math.h>

P16_NOINLINE void p16_f01_btree_init(p16_btree_t *bt) {
    if (!bt) return;
    bt->pool_size = 0;
    bt->root = -1;
}

P16_NOINLINE bool p16_f03_btree_node_is_full(const p16_node_t *node) {
    if (!node) return false;
    return node->num_keys == P16_MAX_KEYS;
}

P16_NOINLINE size_t p16_f11_btree_depth(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return 1;
    return 1 + p16_f11_btree_depth(bt, n->children[0]);
}

P16_NOINLINE int32_t p16_f13_btree_max_key(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return n->keys[n->num_keys - 1];
    return p16_f13_btree_max_key(bt, n->children[n->num_keys]);
}
