#include <stdio.h>
#include <assert.h>
#include "p19_shortest_path.h"

int main(void) {
    p19_graph_t g;
    p19_f01_sp_init(&g, 4);

    p19_f02_sp_add_edge(&g, 0, 1, 1);
    p19_f02_sp_add_edge(&g, 1, 2, 2);
    p19_f02_sp_add_edge(&g, 2, 3, 3);
    p19_f02_sp_add_edge(&g, 0, 3, 10);

    int32_t dist[4], parent[4];
    p19_f03_dijkstra(&g, 0, dist, parent);
    assert(dist[3] == 6); // 0->1->2->3 = 1+2+3 = 6

    size_t path[4];
    size_t plen = p19_f06_reconstruct_path(parent, 3, path, 4);
    assert(plen == 4 && path[0] == 0 && path[3] == 3);

    assert(p19_f04_bellman_ford(&g, 0, dist));
    assert(!p19_f07_has_negative_cycle(&g));

    int32_t fw[P19_MAX_VERTICES][P19_MAX_VERTICES];
    p19_f05_floyd_warshall(&g, fw);
    assert(fw[0][3] == 6);

    assert(p19_f08_bidirectional_dijkstra(&g, 0, 3) == 6);

    p19_f16_sp_clear(&g);
    assert(g.weights[0][1] == P19_INF);

    printf("PASS: p19_shortest_path unit tests passed.\n");
    return 0;
}
