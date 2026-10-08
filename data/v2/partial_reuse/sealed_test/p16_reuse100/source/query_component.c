#include "query_component.h"
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

P16_NOINLINE void p16_f06_btree_insert_nonfull(p16_btree_t *bt, int32_t node_idx, int32_t key) {
    if (!bt || node_idx < 0) return;
    p16_node_t *n = &bt->pool[node_idx];
    int i = (int)n->num_keys - 1;

    if (n->is_leaf) {
        while (i >= 0 && n->keys[i] > key) {
            n->keys[i + 1] = n->keys[i];
            i--;
        }
        n->keys[i + 1] = key;
        n->num_keys++;
    } else {
        while (i >= 0 && n->keys[i] > key) {
            i--;
        }
        i++;
        int32_t child_idx = n->children[i];
        if (p16_f03_btree_node_is_full(&bt->pool[child_idx])) {
            p16_f05_btree_split_child(bt, node_idx, (size_t)i, child_idx);
            n = &bt->pool[node_idx];
            if (n->keys[i] < key) i++;
        }
        p16_f06_btree_insert_nonfull(bt, n->children[i], key);
    }
}

P16_NOINLINE bool p16_f07_btree_insert(p16_btree_t *bt, int32_t key) {
    if (!bt) return false;
    if (bt->root < 0) {
        int32_t r = p16_f02_btree_alloc_node(bt, true);
        if (r < 0) return false;
        bt->pool[r].keys[0] = key;
        bt->pool[r].num_keys = 1;
        bt->root = r;
        return true;
    }
    int32_t r = bt->root;
    if (p16_f03_btree_node_is_full(&bt->pool[r])) {
        int32_t s = p16_f02_btree_alloc_node(bt, false);
        if (s < 0) return false;
        bt->root = s;
        bt->pool[s].children[0] = r;
        p16_f05_btree_split_child(bt, s, 0, r);
        p16_f06_btree_insert_nonfull(bt, s, key);
    } else {
        p16_f06_btree_insert_nonfull(bt, r, key);
    }
    return true;
}

P16_NOINLINE bool p16_f08_btree_search(const p16_btree_t *bt, int32_t node_idx, int32_t key) {
    if (!bt || node_idx < 0) return false;
    const p16_node_t *n = &bt->pool[node_idx];
    size_t i = 0;
    while (i < n->num_keys && key > n->keys[i]) i++;
    if (i < n->num_keys && key == n->keys[i]) return true;
    if (n->is_leaf) return false;
    return p16_f08_btree_search(bt, n->children[i], key);
}

P16_NOINLINE size_t p16_f09_btree_traverse(const p16_btree_t *bt, int32_t node_idx, int32_t *out_keys, size_t max_keys) {
    if (!bt || !out_keys || max_keys == 0) return 0;
    return traverse_rec(bt, node_idx, out_keys, max_keys, 0);
}

P16_NOINLINE size_t p16_f10_btree_count_keys(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    size_t total = n->num_keys;
    if (!n->is_leaf) {
        for (size_t i = 0; i <= n->num_keys; ++i) {
            total += p16_f10_btree_count_keys(bt, n->children[i]);
        }
    }
    return total;
}

P16_NOINLINE size_t p16_f11_btree_depth(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return 1;
    return 1 + p16_f11_btree_depth(bt, n->children[0]);
}

P16_NOINLINE int32_t p16_f12_btree_min_key(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return n->keys[0];
    return p16_f12_btree_min_key(bt, n->children[0]);
}

P16_NOINLINE int32_t p16_f13_btree_max_key(const p16_btree_t *bt, int32_t node_idx) {
    if (!bt || node_idx < 0) return 0;
    const p16_node_t *n = &bt->pool[node_idx];
    if (n->is_leaf) return n->keys[n->num_keys - 1];
    return p16_f13_btree_max_key(bt, n->children[n->num_keys]);
}

P16_NOINLINE bool p16_f14_btree_contains(const p16_btree_t *bt, int32_t key) {
    if (!bt || bt->root < 0) return false;
    return p16_f08_btree_search(bt, bt->root, key);
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

P16_NOINLINE void p16_f16_btree_clear(p16_btree_t *bt) {
    if (!bt) return;
    bt->pool_size = 0;
    bt->root = -1;
}
