#ifndef TARGET_QUERY_COMPONENT_H
#define TARGET_QUERY_COMPONENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define P09_NOINLINE __attribute__((noinline))
#define P09_MAX_NODES 128

typedef struct p09_node {
    int32_t key;
    int32_t value;
    int32_t height;
    int32_t left;   // index into node pool, -1 if NULL
    int32_t right;  // index into node pool, -1 if NULL
} p09_node_t;

typedef struct {
    p09_node_t pool[P09_MAX_NODES];
    size_t pool_size;
    int32_t root;
} p09_tree_t;

/* Benchmark Function Prototypes */
P09_NOINLINE void    p09_f01_avl_init(p09_tree_t *t);
P09_NOINLINE int32_t p09_f02_avl_height(const p09_tree_t *t, int32_t idx);
P09_NOINLINE int32_t p09_f03_avl_balance_factor(const p09_tree_t *t, int32_t idx);
P09_NOINLINE int32_t p09_f04_avl_rotate_right(p09_tree_t *t, int32_t y);
P09_NOINLINE int32_t p09_f05_avl_rotate_left(p09_tree_t *t, int32_t x);
P09_NOINLINE int32_t p09_f06_avl_insert_internal(p09_tree_t *t, int32_t node, int32_t key, int32_t val);
P09_NOINLINE bool    p09_f07_avl_insert(p09_tree_t *t, int32_t key, int32_t val);
P09_NOINLINE int32_t p09_f08_avl_min_node(const p09_tree_t *t, int32_t node);
P09_NOINLINE int32_t p09_f09_avl_delete_internal(p09_tree_t *t, int32_t node, int32_t key);
P09_NOINLINE bool    p09_f10_avl_delete(p09_tree_t *t, int32_t key);
P09_NOINLINE bool    p09_f11_avl_search(const p09_tree_t *t, int32_t key, int32_t *out_val);
P09_NOINLINE size_t  p09_f12_avl_inorder(const p09_tree_t *t, int32_t *out_keys, size_t max_keys);
P09_NOINLINE size_t  p09_f13_avl_preorder(const p09_tree_t *t, int32_t *out_keys, size_t max_keys);
P09_NOINLINE size_t  p09_f14_avl_count(const p09_tree_t *t, int32_t node);
P09_NOINLINE bool    p09_f15_avl_is_balanced(const p09_tree_t *t, int32_t node);
P09_NOINLINE void    p09_f16_avl_clear(p09_tree_t *t);

#endif /* TARGET_QUERY_COMPONENT_H */
