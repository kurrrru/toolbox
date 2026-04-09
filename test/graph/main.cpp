#include "toolbox/graph/shortest_path/dijkstra.hpp"
#include "toolbox/graph/shortest_path/bellman_ford.hpp"
#include "toolbox/graph/directed/topological.hpp"
#include "toolbox/graph/other/diameter_tree.hpp"
#include "test/test_util.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

const long long INF = 1LL << 60;

bool check(bool cond, const std::string &label) {
    if (!cond)
        std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// ---- helper: build adjacency list ------------------------------------------

typedef std::vector<std::vector<std::pair<long long, long long>>> Graph;

Graph make_graph(int n) { return Graph(n); }

void add_edge(Graph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

void add_directed_edge(Graph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
}

// ---- dijkstra --------------------------------------------------------------

bool test_dijkstra_basic() {
    // 0 -1- 1 -2- 2 -1- 3
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    add_edge(g, 2, 3, 1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= check(dist[0] == 0, "dijkstra: dist[0]==0");
    ok &= check(dist[1] == 1, "dijkstra: dist[1]==1");
    ok &= check(dist[2] == 3, "dijkstra: dist[2]==3");
    ok &= check(dist[3] == 4, "dijkstra: dist[3]==4");
    return ok;
}

bool test_dijkstra_disconnected() {
    // 0 -1- 1    2 (disconnected)
    Graph g = make_graph(3);
    add_edge(g, 0, 1, 1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= check(dist[0] == 0,   "dijkstra disconnected: dist[0]==0");
    ok &= check(dist[1] == 1,   "dijkstra disconnected: dist[1]==1");
    ok &= check(dist[2] == INF, "dijkstra disconnected: dist[2]==INF");
    return ok;
}

bool test_dijkstra_single_node() {
    Graph g = make_graph(1);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    return check(dist[0] == 0, "dijkstra single node: dist[0]==0");
}

bool test_dijkstra_multiple_paths() {
    // Two paths from 0 to 3: 0->1->3 (cost 5) vs 0->2->3 (cost 3)
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 3);
    add_directed_edge(g, 0, 2, 1);
    add_directed_edge(g, 1, 3, 2);
    add_directed_edge(g, 2, 3, 2);
    auto dist = toolbox::graph::dijkstra(0LL, g);
    bool ok = true;
    ok &= check(dist[3] == 3, "dijkstra multiple paths: shortest is 3");
    return ok;
}

// ---- bellman_ford ----------------------------------------------------------

bool test_bellman_ford_basic() {
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1, 1);
    add_directed_edge(g, 1, 2, 2);
    add_directed_edge(g, 2, 3, 1);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= check(!dist.empty(), "bellman_ford basic: no negative cycle");
    ok &= check(dist[0] == 0, "bellman_ford: dist[0]==0");
    ok &= check(dist[1] == 1, "bellman_ford: dist[1]==1");
    ok &= check(dist[2] == 3, "bellman_ford: dist[2]==3");
    ok &= check(dist[3] == 4, "bellman_ford: dist[3]==4");
    return ok;
}

bool test_bellman_ford_negative_edges() {
    // 0 -1-> 1 -(-3)-> 2 -2-> 3  : path cost = 1 + (-3) + 2 = 0
    Graph g = make_graph(4);
    add_directed_edge(g, 0, 1,  1);
    add_directed_edge(g, 1, 2, -3);
    add_directed_edge(g, 2, 3,  2);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= check(!dist.empty(), "bellman_ford negative: no negative cycle");
    ok &= check(dist[3] == 0,  "bellman_ford negative edges: dist[3]==0");
    return ok;
}

bool test_bellman_ford_negative_cycle() {
    // 0 -> 1 -> 2 -> 1 (negative cycle: 1->2->1 with total cost -1)
    Graph g = make_graph(3);
    add_directed_edge(g, 0, 1,  1);
    add_directed_edge(g, 1, 2,  2);
    add_directed_edge(g, 2, 1, -4); // forms negative cycle
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    return check(dist.empty(), "bellman_ford detects negative cycle");
}

bool test_bellman_ford_disconnected() {
    Graph g = make_graph(3);
    add_directed_edge(g, 0, 1, 5);
    auto dist = toolbox::graph::bellman_ford(0LL, g);
    bool ok = true;
    ok &= check(!dist.empty(), "bellman_ford disconnected: no negative cycle");
    ok &= check(dist[2] == INF, "bellman_ford disconnected: unreachable is INF");
    return ok;
}

// ---- topological sort ------------------------------------------------------

bool is_valid_topological(const std::vector<long long> &order,
                           const std::vector<std::vector<long long>> &edge) {
    int n = edge.size();
    std::vector<int> pos(n);
    for (int i = 0; i < (int)order.size(); i++)
        pos[order[i]] = i;
    for (int u = 0; u < n; u++)
        for (long long v : edge[u])
            if (pos[u] >= pos[v])
                return false;
    return true;
}

bool test_topological_basic() {
    // 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3
    std::vector<std::vector<long long>> edge(4);
    edge[0].push_back(1); edge[0].push_back(2);
    edge[1].push_back(3); edge[2].push_back(3);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= check(order.size() == 4, "topological basic: all nodes in order");
    ok &= check(is_valid_topological(order, edge), "topological basic: valid ordering");
    return ok;
}

bool test_topological_cycle() {
    // 0 -> 1 -> 2 -> 0 (cycle)
    std::vector<std::vector<long long>> edge(3);
    edge[0].push_back(1); edge[1].push_back(2); edge[2].push_back(0);
    auto order = toolbox::graph::topological(edge);
    return check(order.empty(), "topological cycle: returns empty");
}

bool test_topological_single() {
    std::vector<std::vector<long long>> edge(1);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= check(order.size() == 1, "topological single: size==1");
    ok &= check(order[0] == 0,     "topological single: order[0]==0");
    return ok;
}

bool test_topological_chain() {
    // 0 -> 1 -> 2 -> 3 -> 4
    std::vector<std::vector<long long>> edge(5);
    for (int i = 0; i < 4; i++)
        edge[i].push_back(i + 1);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= check(order.size() == 5, "topological chain: all nodes");
    ok &= check(is_valid_topological(order, edge), "topological chain: valid");
    return ok;
}

// ---- diameter_tree ---------------------------------------------------------

typedef std::vector<std::vector<std::pair<long long, long long>>> TreeGraph;

void add_tree_edge(TreeGraph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

bool test_diameter_path() {
    // Path graph: 0 -1- 1 -2- 2 -3- 3  diameter = 6
    TreeGraph g(4);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 1, 2, 2);
    add_tree_edge(g, 2, 3, 3);
    long long d = toolbox::graph::diameter_tree(g);
    return check(d == 6, "diameter path graph: 6");
}

bool test_diameter_star() {
    // Star: center=0, leaves 1,2,3,4 with weights 1,5,3,2 -> diameter = 5+3=8
    TreeGraph g(5);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 0, 2, 5);
    add_tree_edge(g, 0, 3, 3);
    add_tree_edge(g, 0, 4, 2);
    long long d = toolbox::graph::diameter_tree(g);
    return check(d == 8, "diameter star: 8 (5+3)");
}

bool test_diameter_single_edge() {
    TreeGraph g(2);
    add_tree_edge(g, 0, 1, 7);
    long long d = toolbox::graph::diameter_tree(g);
    return check(d == 7, "diameter single edge: 7");
}

bool test_diameter_balanced() {
    // Balanced binary tree with equal weights
    //       0
    //      / \
    //     1   2   (each edge weight=1)
    //    / \
    //   3   4
    // diameter: 3->1->0->2 = 3
    TreeGraph g(5);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 0, 2, 1);
    add_tree_edge(g, 1, 3, 1);
    add_tree_edge(g, 1, 4, 1);
    long long d = toolbox::graph::diameter_tree(g);
    return check(d == 3, "diameter balanced tree: 3");
}

// ---- runner ----------------------------------------------------------------

struct Test {
    std::string name;
    bool (*fn)();
};

} // namespace

int main() {
    Test tests[] = {
        {"dijkstra_basic",              test_dijkstra_basic},
        {"dijkstra_disconnected",       test_dijkstra_disconnected},
        {"dijkstra_single_node",        test_dijkstra_single_node},
        {"dijkstra_multiple_paths",     test_dijkstra_multiple_paths},
        {"bellman_ford_basic",          test_bellman_ford_basic},
        {"bellman_ford_negative_edges", test_bellman_ford_negative_edges},
        {"bellman_ford_negative_cycle", test_bellman_ford_negative_cycle},
        {"bellman_ford_disconnected",   test_bellman_ford_disconnected},
        {"topological_basic",           test_topological_basic},
        {"topological_cycle",           test_topological_cycle},
        {"topological_single",          test_topological_single},
        {"topological_chain",           test_topological_chain},
        {"diameter_path",               test_diameter_path},
        {"diameter_star",               test_diameter_star},
        {"diameter_single_edge",        test_diameter_single_edge},
        {"diameter_balanced",           test_diameter_balanced},
    };
    const std::size_t num = sizeof(tests) / sizeof(tests[0]);

    int pass = 0, fail = 0;
    for (std::size_t i = 0; i < num; i++) {
        bool ok = tests[i].fn();
        if (ok) {
            std::cout << toolbox::color::cyan << "PASS " << tests[i].name
                      << toolbox::color::reset << "\n";
            ++pass;
        } else {
            std::cout << toolbox::color::yellow << "FAIL " << tests[i].name
                      << toolbox::color::reset << "\n";
            ++fail;
        }
    }
    std::cout << "\n";
    if (fail == 0)
        std::cout << toolbox::color::green << "All " << pass << " tests passed!"
                  << toolbox::color::reset << "\n";
    else
        std::cout << toolbox::color::red << fail << " out of " << (pass + fail)
                  << " tests failed." << toolbox::color::reset << "\n";
    return fail == 0 ? 0 : 1;
}
