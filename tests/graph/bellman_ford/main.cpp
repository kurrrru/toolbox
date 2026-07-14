#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "toolbox/graph/shortest_path/bellman_ford.hpp"
#include "toolbox/graph/shortest_path/dijkstra.hpp"

#include "utils/test_util.hpp"

namespace {

const long long INF = 1LL << 60;

typedef std::vector<std::vector<std::pair<long long, long long>>> Graph;

Graph make_graph(int n) { return Graph(n); }

void add_directed_edge(Graph &g, long long u, long long v, long long w) { g[u].push_back({v, w}); }

// ---- basic cases -----------------------------------------------------------

bool test_basic() {
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 1);
    add_directed_edge(g, 1, 2, 2);
    add_directed_edge(g, 2, 3, 1);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(), "bellman_ford basic: no negative cycle");
    ok &= toolbox::test_utils::check(dist[0] == 0, "bellman_ford: dist[0]==0");
    ok &= toolbox::test_utils::check(dist[1] == 1, "bellman_ford: dist[1]==1");
    ok &= toolbox::test_utils::check(dist[2] == 3, "bellman_ford: dist[2]==3");
    ok &= toolbox::test_utils::check(dist[3] == 4, "bellman_ford: dist[3]==4");
    return ok;
}

bool test_negative_edges() {
    // 0 -1-> 1 -(-3)-> 2 -2-> 3  : path cost = 1 + (-3) + 2 = 0
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 1);
    add_directed_edge(g, 1, 2, -3);
    add_directed_edge(g, 2, 3, 2);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(), "bellman_ford negative: no negative cycle");
    ok &= toolbox::test_utils::check(dist[3] == 0, "bellman_ford negative edges: dist[3]==0");
    return ok;
}

bool test_negative_edge_preferred() {
    // A negative edge should make a longer-hop path cheaper than a direct positive edge.
    // 0->1 direct cost 10; 0->2->1 cost 4 + (-8) = -4.
    Graph g = make_graph(3);
    add_directed_edge(g, 0, 1, 10);
    add_directed_edge(g, 0, 2, 4);
    add_directed_edge(g, 2, 1, -8);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(), "bellman_ford neg preferred: no neg cycle");
    ok &= toolbox::test_utils::check(dist[1] == -4, "bellman_ford neg preferred: dist[1]==-4");
    return ok;
}

// ---- negative cycle detection ----------------------------------------------

bool test_negative_cycle() {
    // 0 -> 1 -> 2 -> 1 (negative cycle: 1->2->1 with total cost -2)
    Graph g = make_graph(3);
    add_directed_edge(g, 0, 1, 1);
    add_directed_edge(g, 1, 2, 2);
    add_directed_edge(g, 2, 1, -4);  // forms negative cycle
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    return toolbox::test_utils::check(dist.empty(), "bellman_ford detects negative cycle");
}

bool test_negative_self_loop() {
    // A negative self-loop reachable from the source is itself a negative cycle.
    Graph g = make_graph(2);
    add_directed_edge(g, 0, 1, 1);
    add_directed_edge(g, 1, 1, -1);  // negative self-loop
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    return toolbox::test_utils::check(dist.empty(), "bellman_ford detects negative self-loop");
}

bool test_negative_cycle_unreachable() {
    // Negative cycle among {1,2} is NOT reachable from source 0, so no detection expected.
    Graph g = make_graph(3);
    add_directed_edge(g, 1, 2, 1);
    add_directed_edge(g, 2, 1, -3);  // negative cycle, but unreachable from 0
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(),
                                     "bellman_ford: unreachable neg cycle not reported");
    ok &= toolbox::test_utils::check(dist[0] == 0, "bellman_ford: dist[0]==0");
    return ok;
}

bool test_negative_cycle_helper() {
    // The negative_cycle() convenience wrapper should agree with the empty-vector convention.
    Graph with_cycle = make_graph(2);
    add_directed_edge(with_cycle, 0, 1, 1);
    add_directed_edge(with_cycle, 1, 0, -3);
    Graph without_cycle = make_graph(2);
    add_directed_edge(without_cycle, 0, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::graph::negative_cycle(0LL, with_cycle),
                                     "negative_cycle: true when present");
    ok &= toolbox::test_utils::check(!toolbox::graph::negative_cycle(0LL, without_cycle),
                                     "negative_cycle: false when absent");
    return ok;
}

// ---- structural cases ------------------------------------------------------

bool test_disconnected() {
    Graph g = make_graph(3);
    add_directed_edge(g, 0, 1, 5);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(), "bellman_ford disconnected: no negative cycle");
    ok &=
        toolbox::test_utils::check(dist[2] == INF, "bellman_ford disconnected: unreachable is INF");
    return ok;
}

bool test_single_node() {
    Graph g = make_graph(1);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(!dist.empty(), "bellman_ford single node: no negative cycle");
    ok &= toolbox::test_utils::check(dist[0] == 0, "bellman_ford single node: dist[0]==0");
    return ok;
}

// ---- randomized cross-check against dijkstra (non-negative weights) --------

bool test_randomized_vs_dijkstra() {
    std::mt19937 rng(19260817);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 12)(rng);
        Graph g = make_graph(n);
        const int edge_count = std::uniform_int_distribution<int>(0, n * n)(rng);
        std::uniform_int_distribution<int> vtx(0, n - 1);
        std::uniform_int_distribution<int> weight(0, 30);  // non-negative for dijkstra parity
        for (int i = 0; i < edge_count; ++i) {
            add_directed_edge(g, vtx(rng), vtx(rng), weight(rng));
        }
        const long long s = vtx(rng);
        auto bf = toolbox::graph::bellman_ford(s, g);
        auto dj = toolbox::graph::dijkstra(s, g);
        ok &= toolbox::test_utils::check(!bf.empty(), "bellman_ford: no false negative cycle");
        ok &= toolbox::test_utils::check(
            bf == dj, "bellman_ford == dijkstra (non-negative) trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"basic", test_basic},
        {"negative_edges", test_negative_edges},
        {"negative_edge_preferred", test_negative_edge_preferred},
        {"negative_cycle", test_negative_cycle},
        {"negative_self_loop", test_negative_self_loop},
        {"negative_cycle_unreachable", test_negative_cycle_unreachable},
        {"negative_cycle_helper", test_negative_cycle_helper},
        {"disconnected", test_disconnected},
        {"single_node", test_single_node},
        {"randomized_vs_dijkstra", test_randomized_vs_dijkstra},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
