#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "toolbox/graph/shortest_path/dijkstra.hpp"

#include "utils/test_util.hpp"

namespace {

const long long INF = 1LL << 60;

typedef std::vector<std::vector<std::pair<long long, long long>>> Graph;

Graph make_graph(int n) { return Graph(n); }

void add_edge(Graph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

void add_directed_edge(Graph &g, long long u, long long v, long long w) { g[u].push_back({v, w}); }

// Reference single-source shortest path via naive O(V*E) relaxation (Bellman-Ford style).
// Valid for non-negative weights and self-contained for cross-checking dijkstra.
std::vector<long long> brute_sssp(long long s, const Graph &g) {
    const int n = static_cast<int>(g.size());
    std::vector<long long> dist(n, INF);
    dist[s] = 0;
    for (int iter = 0; iter < n; ++iter) {
        for (int u = 0; u < n; ++u) {
            if (dist[u] == INF) {
                continue;
            }
            for (const auto &[v, w] : g[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                }
            }
        }
    }
    return dist;
}

// ---- basic cases -----------------------------------------------------------

bool test_basic() {
    // 0 -1- 1 -2- 2 -1- 3
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    add_edge(g, 2, 3, 1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(dist[0] == 0, "dijkstra: dist[0]==0");
    ok &= toolbox::test_utils::check(dist[1] == 1, "dijkstra: dist[1]==1");
    ok &= toolbox::test_utils::check(dist[2] == 3, "dijkstra: dist[2]==3");
    ok &= toolbox::test_utils::check(dist[3] == 4, "dijkstra: dist[3]==4");
    return ok;
}

bool test_disconnected() {
    // 0 -1- 1    2 (disconnected)
    Graph g = make_graph(3);
    add_edge(g, 0, 1, 1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(dist[0] == 0, "dijkstra disconnected: dist[0]==0");
    ok &= toolbox::test_utils::check(dist[1] == 1, "dijkstra disconnected: dist[1]==1");
    ok &= toolbox::test_utils::check(dist[2] == INF, "dijkstra disconnected: dist[2]==INF");
    return ok;
}

bool test_single_node() {
    Graph g = make_graph(1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    return toolbox::test_utils::check(dist[0] == 0, "dijkstra single node: dist[0]==0");
}

bool test_multiple_paths() {
    // Two paths from 0 to 3: 0->1->3 (cost 5) vs 0->2->3 (cost 3)
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 3);
    add_directed_edge(g, 0, 2, 1);
    add_directed_edge(g, 1, 3, 2);
    add_directed_edge(g, 2, 3, 2);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    return toolbox::test_utils::check(dist[3] == 3, "dijkstra multiple paths: shortest is 3");
}

// ---- edge cases ------------------------------------------------------------

bool test_zero_weight_edges() {
    // Zero-weight edges form a "free" path; dijkstra must handle 0 costs.
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 0);
    add_directed_edge(g, 1, 2, 0);
    add_directed_edge(g, 2, 3, 5);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(dist[1] == 0, "dijkstra zero weight: dist[1]==0");
    ok &= toolbox::test_utils::check(dist[2] == 0, "dijkstra zero weight: dist[2]==0");
    ok &= toolbox::test_utils::check(dist[3] == 5, "dijkstra zero weight: dist[3]==5");
    return ok;
}

bool test_self_loop() {
    // A self-loop must never make a shortest distance worse or loop forever.
    Graph g = make_graph(2);
    add_directed_edge(g, 0, 0, 3);  // self loop
    add_directed_edge(g, 0, 1, 2);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(dist[0] == 0, "dijkstra self loop: dist[0]==0");
    ok &= toolbox::test_utils::check(dist[1] == 2, "dijkstra self loop: dist[1]==2");
    return ok;
}

bool test_parallel_edges() {
    // Two parallel edges 0->1 with different costs; the cheaper must win.
    Graph g = make_graph(2);
    add_directed_edge(g, 0, 1, 7);
    add_directed_edge(g, 0, 1, 2);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    return toolbox::test_utils::check(dist[1] == 2, "dijkstra parallel edges: cheaper wins");
}

bool test_non_zero_source() {
    // Start from a vertex other than 0.
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 2);
    add_edge(g, 1, 2, 3);
    add_edge(g, 2, 3, 4);
    auto dist = toolbox::graph::dijkstra(2LL, g);
    bool ok = true;
    ok &= toolbox::test_utils::check(dist[2] == 0, "dijkstra src=2: dist[2]==0");
    ok &= toolbox::test_utils::check(dist[1] == 3, "dijkstra src=2: dist[1]==3");
    ok &= toolbox::test_utils::check(dist[0] == 5, "dijkstra src=2: dist[0]==5");
    ok &= toolbox::test_utils::check(dist[3] == 4, "dijkstra src=2: dist[3]==4");
    return ok;
}

bool test_undirected_cycle() {
    // Triangle with an expensive direct edge and a cheaper two-hop path.
    Graph g = make_graph(3);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 1);
    add_edge(g, 0, 2, 5);  // direct edge is worse than 0-1-2 (cost 2)
    auto dist = toolbox::graph::dijkstra(0LL, g);
    return toolbox::test_utils::check(dist[2] == 2, "dijkstra triangle: prefers 0-1-2 (2 < 5)");
}

// ---- randomized cross-check ------------------------------------------------

bool test_randomized() {
    std::mt19937 rng(20240607);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 12)(rng);
        Graph g = make_graph(n);
        const int max_edges = n * n;
        const int edge_count = std::uniform_int_distribution<int>(0, max_edges)(rng);
        std::uniform_int_distribution<int> vtx(0, n - 1);
        std::uniform_int_distribution<int> weight(0, 30);
        for (int i = 0; i < edge_count; ++i) {
            add_directed_edge(g, vtx(rng), vtx(rng), weight(rng));
        }
        const long long s = vtx(rng);
        ok &= toolbox::test_utils::check(toolbox::graph::dijkstra(s, g) == brute_sssp(s, g),
                                         "dijkstra randomized trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"basic", test_basic},
        {"disconnected", test_disconnected},
        {"single_node", test_single_node},
        {"multiple_paths", test_multiple_paths},
        {"zero_weight_edges", test_zero_weight_edges},
        {"self_loop", test_self_loop},
        {"parallel_edges", test_parallel_edges},
        {"non_zero_source", test_non_zero_source},
        {"undirected_cycle", test_undirected_cycle},
        {"randomized", test_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
