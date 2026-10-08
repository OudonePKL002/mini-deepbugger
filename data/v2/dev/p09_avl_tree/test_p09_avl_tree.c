#include <stdio.h>
#include <assert.h>
#include "p09_avl_tree.h"

int main(void) {
    p09_tree_t t;
    p09_f01_avl_init(&t);

    assert(p09_f07_avl_insert(&t, 50, 500));
    assert(p09_f07_avl_insert(&t, 25, 250));
    assert(p09_f07_avl_insert(&t, 75, 750));
    assert(p09_f07_avl_insert(&t, 10, 100));
    assert(p09_f07_avl_insert(&t, 30, 300));
    assert(p09_f07_avl_insert(&t, 60, 600));
    assert(p09_f07_avl_insert(&t, 80, 800));

    assert(p09_f15_avl_is_balanced(&t, t.root));
    assert(p09_f14_avl_count(&t, t.root) == 7);

    int32_t val = 0;
    assert(p09_f11_avl_search(&t, 30, &val) && val == 300);
    assert(!p09_f11_avl_search(&t, 999, &val));

    int32_t in_keys[10];
    size_t in_n = p09_f12_avl_inorder(&t, in_keys, 10);
    assert(in_n == 7);
    for (size_t i = 1; i < in_n; ++i) {
        assert(in_keys[i-1] < in_keys[i]);
    }

    int32_t pre_keys[10];
    size_t pre_n = p09_f13_avl_preorder(&t, pre_keys, 10);
    assert(pre_n == 7);

    assert(p09_f10_avl_delete(&t, 25));
    assert(p09_f15_avl_is_balanced(&t, t.root));

    p09_f16_avl_clear(&t);
    assert(t.pool_size == 0 && t.root == -1);

    printf("PASS: p09_avl_tree unit tests passed.\n");
    return 0;
}
