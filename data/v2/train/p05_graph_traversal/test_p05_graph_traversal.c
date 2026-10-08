#include <stdio.h>
#include <assert.h>
#include "p05_graph_traversal.h"

int main(void) {
    p05_graph_t g;
    p05_f01_adj_init(&g, 5);
    assert(g.num_vertices == 5);

    p05_f02_adj_add_edge(&g, 0, 1);
    p05_f02_adj_add_edge(&g, 1, 2);
    p05_f02_adj_add_edge(&g, 2, 3);
    p05_f02_adj_add_edge(&g, 3, 4);
    assert(p05_f03_adj_has_edge(&g, 0, 1));
    assert(!p05_f03_adj_has_edge(&g, 1, 0));

    assert(p05_f04_adj_in_degree(&g, 1) == 1);
    assert(p05_f05_adj_out_degree(&g, 1) == 1);

    size_t order[5];
    assert(p05_f06_graph_bfs(&g, 0, order, 5) == 5);
    assert(order[0] == 0 && order[4] == 4);

    assert(p05_f07_graph_dfs_iterative(&g, 0, order, 5) == 5);

    assert(p05_f08_graph_has_cycle_directed(&g) == false);
    p05_f02_adj_add_edge(&g, 4, 1);
    assert(p05_f08_graph_has_cycle_directed(&g) == true);

    int comp[5];
    assert(p05_f09_graph_connected_components(&g, comp) == 1);

    uint8_t reach[P05_MAX_V][P05_MAX_V];
    p05_f11_graph_transitive_closure(&g, reach);
    assert(reach[0][4] == 1);

    size_t dens = p05_f12_graph_density(&g);
    assert(dens > 0);

    p05_graph_t g2;
    p05_f01_adj_init(&g2, 3);
    assert(p05_f13_graph_isolate_count(&g2) == 3);

    assert(p05_f14_graph_eulerian_path_check(&g2) == true);

    bool mask[5] = {true, true, false, false, false};
    p05_graph_t sub;
    p05_f15_graph_subgraph_induced(&g, mask, &sub);
    assert(p05_f03_adj_has_edge(&sub, 0, 1));
    assert(!p05_f03_adj_has_edge(&sub, 1, 2));

    int ecc = p05_f16_graph_vertex_eccentricity(&g, 0);
    assert(ecc >= 0);

    printf("PASS: p05_graph_traversal unit tests passed.\n");
    return 0;
}
