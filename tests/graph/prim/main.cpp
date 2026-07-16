#include <algorithm>
#include <optional>
#include <random>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "toolbox/datastructure/unionfind/unionfind.hpp"
#include "toolbox/graph/mst/kruskal.hpp"
#include "toolbox/graph/mst/prim.hpp"

#include "utils/test_util.hpp"

namespace {

typedef std::vector<std::vector<std::pair<long long, long long>>> Graph;
typedef std::vector<std::tuple<long long, long long, long long>> EdgeList;  // {cost, u, v}

Graph make_graph(int n) { return Graph(n); }

void add_edge(Graph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

// Extracts the undirected edges of an adjacency-list graph as a {cost, u, v} list, counting
// each undirected edge once (u < v). Self-loops (u == v) are dropped.
EdgeList edges_of_graph(int n, const Graph &g) {
    EdgeList edges;
    for (int u = 0; u < n; ++u) {
        for (const auto &[v, w] : g[u]) {
            if (u < static_cast<int>(v)) {
                edges.push_back({w, u, v});
            }
        }
    }
    return edges;
}

// Reference MST cost via kruskal on the same undirected graph. Used to cross-check prim.
std::optional<long long> kruskal_of_graph(int n, const Graph &g) {
    return toolbox::graph::kruskal(n, edges_of_graph(n, g));
}

// Normalizes an edge to (cost, min endpoint, max endpoint) so undirected edges compare equal
// regardless of endpoint order.
std::tuple<long long, long long, long long> norm_edge(
    const std::tuple<long long, long long, long long> &e) {
    auto [c, u, v] = e;
    if (u > v) {
        std::swap(u, v);
    }
    return {c, u, v};
}

// Returns the total cost if `tree` is a valid spanning tree of the n-vertex graph whose
// available edges are `available` (every tree edge must appear there, matched once), and it is
// connected and acyclic. Otherwise returns std::nullopt.
std::optional<long long> validate_tree(int n, const EdgeList &available, const EdgeList &tree) {
    if (static_cast<int>(tree.size()) != n - 1) {
        return std::nullopt;
    }
    std::multiset<std::tuple<long long, long long, long long>> pool;
    for (const auto &e : available) {
        pool.insert(norm_edge(e));
    }
    toolbox::datastructure::unionfind uf(n);  // n > 0 guaranteed for the tested cases
    long long total = 0;
    for (const auto &e : tree) {
        const auto key = norm_edge(e);
        const auto it = pool.find(key);
        if (it == pool.end()) {
            return std::nullopt;  // edge not present in the input
        }
        pool.erase(it);
        if (!uf.unite(static_cast<int>(std::get<1>(e)), static_cast<int>(std::get<2>(e)))) {
            return std::nullopt;  // forms a cycle
        }
        total += std::get<0>(e);
    }
    return total;  // n-1 acyclic edges over n vertices implies a connected spanning tree
}

// ---- basic cases -----------------------------------------------------------

bool test_basic() {
    // MST cost is 4 (same graph shape as the kruskal basic test).
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    add_edge(g, 2, 3, 1);
    add_edge(g, 0, 3, 4);
    add_edge(g, 0, 2, 3);
    auto total = toolbox::graph::prim(g);
    bool ok = true;
    ok &= toolbox::test_utils::check(total.has_value(), "prim basic: has a spanning tree");
    ok &= toolbox::test_utils::check(total == std::optional<long long>(4), "prim basic: cost==4");
    return ok;
}

bool test_disconnected() {
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 2, 3, 1);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(!total.has_value(), "prim disconnected: no spanning tree");
}

bool test_single_node() {
    Graph g = make_graph(1);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(0),
                                      "prim single node: cost==0, no edges needed");
}

bool test_no_edges_multi_node() {
    Graph g = make_graph(3);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(!total.has_value(), "prim no edges, n>1: no spanning tree");
}

bool test_duplicate_edges() {
    // Two parallel edges between 0 and 1 with different costs, plus one more vertex.
    Graph g = make_graph(3);
    add_edge(g, 0, 1, 5);
    add_edge(g, 0, 1, 2);
    add_edge(g, 1, 2, 1);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(3),
                                      "prim duplicate edges: cheaper parallel edge wins");
}

bool test_self_loop_ignored() {
    // A self-loop (0-0) must be skipped without affecting the result.
    Graph g = make_graph(3);
    g[0].push_back({0, 100});  // self loop, added directly (add_edge would double it)
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(3),
                                      "prim self-loop: ignored, cost==3");
}

bool test_already_tree() {
    // Exactly n-1 edges already forming a tree: total is just their sum.
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 3);
    add_edge(g, 1, 2, 5);
    add_edge(g, 2, 3, 7);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(15),
                                      "prim already a tree: cost==15");
}

bool test_negative_weights() {
    Graph g = make_graph(3);
    add_edge(g, 0, 1, -5);
    add_edge(g, 1, 2, -2);
    add_edge(g, 0, 2, 3);
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(-7),
                                      "prim negative weights: cost==-7");
}

bool test_complete_graph() {
    // K4 where the MST is the three cheapest non-cycle-forming edges.
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 0, 2, 2);
    add_edge(g, 0, 3, 3);
    add_edge(g, 1, 2, 4);
    add_edge(g, 1, 3, 5);
    add_edge(g, 2, 3, 6);
    // MST: 0-1(1) + 0-2(2) + 0-3(3) = 6
    auto total = toolbox::graph::prim(g);
    return toolbox::test_utils::check(total == std::optional<long long>(6),
                                      "prim complete graph K4: cost==6");
}

// ---- randomized cross-check against kruskal --------------------------------

bool test_randomized() {
    std::mt19937 rng(1618);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 8)(rng);
        std::vector<std::pair<int, int>> all_pairs;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                all_pairs.push_back({u, v});
            }
        }
        std::shuffle(all_pairs.begin(), all_pairs.end(), rng);
        const int edge_count = all_pairs.empty() ? 0
                                                 : std::uniform_int_distribution<int>(
                                                       0, static_cast<int>(all_pairs.size()))(rng);
        Graph g = make_graph(n);
        std::uniform_int_distribution<int> weight_dist(1, 20);
        for (int i = 0; i < edge_count; ++i) {
            add_edge(g, all_pairs[i].first, all_pairs[i].second, weight_dist(rng));
        }
        ok &= toolbox::test_utils::check(toolbox::graph::prim(g) == kruskal_of_graph(n, g),
                                         "prim randomized trial " + std::to_string(trial));
    }
    return ok;
}

// ---- prim_edges (returns the actual MST edges) -----------------------------

bool test_edges_basic() {
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    add_edge(g, 2, 3, 1);
    add_edge(g, 0, 3, 4);
    add_edge(g, 0, 2, 3);
    auto tree = toolbox::graph::prim_edges(g);
    bool ok = true;
    ok &= toolbox::test_utils::check(tree.has_value(), "prim_edges basic: has a spanning tree");
    if (tree.has_value()) {
        ok &= toolbox::test_utils::check(tree->size() == 3, "prim_edges basic: n-1==3 edges");
        ok &= toolbox::test_utils::check(
            validate_tree(4, edges_of_graph(4, g), *tree) == std::optional<long long>(4),
            "prim_edges basic: valid spanning tree of cost 4");
    }
    return ok;
}

bool test_edges_disconnected() {
    Graph g = make_graph(4);
    add_edge(g, 0, 1, 1);
    add_edge(g, 2, 3, 1);
    auto tree = toolbox::graph::prim_edges(g);
    return toolbox::test_utils::check(!tree.has_value(), "prim_edges disconnected: nullopt");
}

bool test_edges_single_node() {
    Graph g = make_graph(1);
    auto tree = toolbox::graph::prim_edges(g);
    bool ok = true;
    ok &= toolbox::test_utils::check(tree.has_value(), "prim_edges single node: has value");
    ok &= toolbox::test_utils::check(tree.has_value() && tree->empty(),
                                     "prim_edges single node: empty edge set");
    return ok;
}

bool test_edges_randomized() {
    std::mt19937 rng(27182818);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 8)(rng);
        std::vector<std::pair<int, int>> all_pairs;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                all_pairs.push_back({u, v});
            }
        }
        std::shuffle(all_pairs.begin(), all_pairs.end(), rng);
        const int edge_count = all_pairs.empty() ? 0
                                                 : std::uniform_int_distribution<int>(
                                                       0, static_cast<int>(all_pairs.size()))(rng);
        Graph g = make_graph(n);
        std::uniform_int_distribution<int> weight_dist(1, 20);
        for (int i = 0; i < edge_count; ++i) {
            add_edge(g, all_pairs[i].first, all_pairs[i].second, weight_dist(rng));
        }
        const auto tree = toolbox::graph::prim_edges(g);
        const auto cost = toolbox::graph::prim(g);
        ok &= toolbox::test_utils::check(
            tree.has_value() == cost.has_value(),
            "prim_edges: presence matches cost, trial " + std::to_string(trial));
        if (tree.has_value() && cost.has_value()) {
            ok &= toolbox::test_utils::check(
                validate_tree(n, edges_of_graph(n, g), *tree) == cost,
                "prim_edges: valid MST of matching cost, trial " + std::to_string(trial));
        }
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"basic", test_basic},
        {"disconnected", test_disconnected},
        {"single_node", test_single_node},
        {"no_edges_multi_node", test_no_edges_multi_node},
        {"duplicate_edges", test_duplicate_edges},
        {"self_loop_ignored", test_self_loop_ignored},
        {"already_tree", test_already_tree},
        {"negative_weights", test_negative_weights},
        {"complete_graph", test_complete_graph},
        {"randomized", test_randomized},
        {"edges_basic", test_edges_basic},
        {"edges_disconnected", test_edges_disconnected},
        {"edges_single_node", test_edges_single_node},
        {"edges_randomized", test_edges_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
