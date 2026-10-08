#include <stdio.h>
#include <assert.h>
#include "p12_topological_dag.h"

int main(void) {
    p12_dag_t g;
    p12_f01_dag_init(&g, 5);

    p12_f02_dag_add_edge(&g, 0, 1, 10);
    p12_f02_dag_add_edge(&g, 0, 2, 5);
    p12_f02_dag_add_edge(&g, 1, 3, 20);
    p12_f02_dag_add_edge(&g, 2, 3, 15);
    p12_f02_dag_add_edge(&g, 3, 4, 10);

    assert(!p12_f06_dag_has_cycle(&g));

    size_t order[5];
    size_t len = 0;
    assert(p12_f05_dag_kahn_toposort(&g, order, &len) && len == 5);

    int32_t lp = p12_f07_dag_longest_path(&g, 0, 4);
    assert(lp == 40); // 0->1->3->4 = 10+20+10 = 40

    int32_t sp = p12_f08_dag_shortest_path(&g, 0, 4);
    assert(sp == 30); // 0->2->3->4 = 5+15+10 = 30

    assert(p12_f09_dag_count_ancestors(&g, 3) == 3);
    assert(p12_f10_dag_count_descendants(&g, 1) == 2);

    size_t levels[5];
    p12_f11_dag_assign_levels(&g, levels);
    assert(levels[4] == 3);

    size_t sources[5];
    assert(p12_f13_dag_find_sources(&g, sources, 5) == 1 && sources[0] == 0);

    size_t sinks[5];
    assert(p12_f14_dag_find_sinks(&g, sinks, 5) == 1 && sinks[0] == 4);

    assert(p12_f15_dag_is_reachable(&g, 0, 4));
    assert(!p12_f15_dag_is_reachable(&g, 4, 0));

    p12_f16_dag_clear(&g);
    assert(g.adj[0][1] == 0);

    printf("PASS: p12_topological_dag unit tests passed.\n");
    return 0;
}
