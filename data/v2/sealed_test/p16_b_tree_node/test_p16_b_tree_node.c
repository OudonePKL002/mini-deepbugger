#include <stdio.h>
#include <assert.h>
#include "p16_b_tree_node.h"

int main(void) {
    p16_btree_t bt;
    p16_f01_btree_init(&bt);

    assert(p16_f07_btree_insert(&bt, 10));
    assert(p16_f07_btree_insert(&bt, 20));
    assert(p16_f07_btree_insert(&bt, 5));
    assert(p16_f07_btree_insert(&bt, 6));
    assert(p16_f07_btree_insert(&bt, 12));
    assert(p16_f07_btree_insert(&bt, 30));
    assert(p16_f07_btree_insert(&bt, 7));
    assert(p16_f07_btree_insert(&bt, 17));

    assert(p16_f14_btree_contains(&bt, 6));
    assert(p16_f14_btree_contains(&bt, 12));
    assert(!p16_f14_btree_contains(&bt, 99));

    assert(p16_f10_btree_count_keys(&bt, bt.root) == 8);
    assert(p16_f11_btree_depth(&bt, bt.root) >= 2);
    assert(p16_f12_btree_min_key(&bt, bt.root) == 5);
    assert(p16_f13_btree_max_key(&bt, bt.root) == 30);

    int32_t keys[10];
    size_t k_cnt = p16_f09_btree_traverse(&bt, bt.root, keys, 10);
    assert(k_cnt == 8);
    for (size_t i = 1; i < k_cnt; ++i) {
        assert(keys[i - 1] < keys[i]);
    }

    assert(p16_f15_btree_is_valid_node(&bt, bt.root));

    p16_f16_btree_clear(&bt);
    assert(bt.pool_size == 0 && bt.root == -1);

    printf("PASS: p16_b_tree_node unit tests passed.\n");
    return 0;
}
