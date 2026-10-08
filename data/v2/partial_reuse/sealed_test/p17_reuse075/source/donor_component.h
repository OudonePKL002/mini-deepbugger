#ifndef TARGET_DONOR_COMPONENT_H
#define TARGET_DONOR_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P16_NOINLINE __attribute__((noinline))
#define P16_BTREE_T 2           // Minimum degree (2-3-4 tree)
#define P16_MAX_KEYS (2 * P16_BTREE_T - 1)  // 3
#define P16_MAX_CHILDREN (2 * P16_BTREE_T) // 4
#define P16_MAX_NODES 64

typedef struct {
    int32_t keys[P16_MAX_KEYS];
    int32_t children[P16_MAX_CHILDREN]; // node pool indices, -1 if leaf
    size_t num_keys;
    bool is_leaf;
} p16_node_t;

typedef struct {
    p16_node_t pool[P16_MAX_NODES];
    size_t pool_size;
    int32_t root;
} p16_btree_t;

/* Benchmark Function Prototypes */
P16_NOINLINE void    p16_f01_btree_init(p16_btree_t *bt);
P16_NOINLINE int32_t p16_f04_btree_find_key_idx(const p16_node_t *node, int32_t key);
P16_NOINLINE size_t  p16_f11_btree_depth(const p16_btree_t *bt, int32_t node_idx);
P16_NOINLINE bool    p16_f15_btree_is_valid_node(const p16_btree_t *bt, int32_t node_idx);

#endif /* TARGET_DONOR_COMPONENT_H */
