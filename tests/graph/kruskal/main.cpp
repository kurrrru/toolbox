#include <algorithm>
#include <optional>
#include <random>
#include <string>
#include <tuple>
#include <vector>

#include "toolbox/datastructure/unionfind/unionfind.hpp"
#include "toolbox/graph/mst/kruskal.hpp"

#include "utils/test_util.hpp"

namespace {

typedef std::vector<std::tuple<long long, long long, long long>> EdgeList;  // {cost, u, v}

// Brute-force MST for verification: tries every subset of edges of size n-1, keeping the
// cheapest one that forms a spanning tree (acyclic on n vertices with n-1 edges implies
// connected). O(2^E), test-only reference.
std::optional<long long> brute_mst(int n, const EdgeList &edges) {
    const int m = static_cast<int>(edges.size());
    bool found = false;
    long long best = 0;
    for (int mask = 0; mask < (1 << m); ++mask) {
        if (__builtin_popcount(static_cast<unsigned>(mask)) != n - 1) {
            continue;
        }
        toolbox::datastructure::unionfind uf(n);
        long long cost = 0;
        bool acyclic = true;
        for (int i = 0; i < m; ++i) {
            if (!(mask & (1 << i))) {
                continue;
            }
            const auto &[c, u, v] = edges[i];
            if (!uf.unite(static_cast<int>(u), static_cast<int>(v))) {
                acyclic = false;
                break;
            }
            cost += c;
        }
        if (acyclic && (!found || cost < best)) {
            found = true;
            best = cost;
        }
    }
    if (!found) {
        return std::nullopt;
    }
    return best;
}

// ---- basic cases -----------------------------------------------------------

bool test_basic() {
    // 0 -1- 1 -2- 2 -1- 3, plus redundant edges 0-2 (3) and 0-3 (4).
    // MST: (0-1,1) + (2-3,1) + (1-2,2) = 4.
    EdgeList edges = {{1, 0, 1}, {2, 1, 2}, {1, 2, 3}, {4, 0, 3}, {3, 0, 2}};
    auto total = toolbox::graph::kruskal(4, edges);
    bool ok = true;
    ok &= toolbox::test_utils::check(total.has_value(), "kruskal basic: has a spanning tree");
    ok &=
        toolbox::test_utils::check(total == std::optional<long long>(4), "kruskal basic: cost==4");
    return ok;
}

bool test_disconnected() {
    // Two separate components: {0,1} and {2,3}, no edge between them.
    EdgeList edges = {{1, 0, 1}, {1, 2, 3}};
    auto total = toolbox::graph::kruskal(4, edges);
    return toolbox::test_utils::check(!total.has_value(), "kruskal disconnected: no spanning tree");
}

bool test_single_node() {
    EdgeList edges;
    auto total = toolbox::graph::kruskal(1, edges);
    return toolbox::test_utils::check(total == std::optional<long long>(0),
                                      "kruskal single node: cost==0, no edges needed");
}

bool test_no_edges_multi_node() {
    // More than one vertex but no edges -> not connected.
    EdgeList edges;
    auto total = toolbox::graph::kruskal(3, edges);
    return toolbox::test_utils::check(!total.has_value(),
                                      "kruskal no edges, n>1: no spanning tree");
}

bool test_duplicate_edges() {
    // Two parallel edges between 0 and 1 with different costs, plus one more vertex.
    EdgeList edges = {{5, 0, 1}, {2, 0, 1}, {1, 1, 2}};
    auto total = toolbox::graph::kruskal(3, edges);
    return toolbox::test_utils::check(total == std::optional<long long>(3),
                                      "kruskal duplicate edges: cheaper parallel edge wins");
}

bool test_self_loop_ignored() {
    // A self-loop (0-0) can never be part of a tree; it must be skipped without breaking things.
    EdgeList edges = {{100, 0, 0}, {1, 0, 1}, {2, 1, 2}};
    auto total = toolbox::graph::kruskal(3, edges);
    return toolbox::test_utils::check(total == std::optional<long long>(3),
                                      "kruskal self-loop: ignored, cost==3");
}

bool test_already_tree() {
    // Exactly n-1 edges already forming a tree: total is just their sum.
    EdgeList edges = {{3, 0, 1}, {5, 1, 2}, {7, 2, 3}};
    auto total = toolbox::graph::kruskal(4, edges);
    return toolbox::test_utils::check(total == std::optional<long long>(15),
                                      "kruskal already a tree: cost==15");
}

bool test_negative_weights() {
    // MST is well-defined with negative edge weights too.
    EdgeList edges = {{-5, 0, 1}, {-2, 1, 2}, {3, 0, 2}};
    auto total = toolbox::graph::kruskal(3, edges);
    return toolbox::test_utils::check(total == std::optional<long long>(-7),
                                      "kruskal negative weights: cost==-7");
}

// ---- randomized cross-check ------------------------------------------------

bool test_randomized() {
    std::mt19937 rng(2718);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 6)(rng);
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
        EdgeList edges;
        std::uniform_int_distribution<int> weight_dist(1, 20);
        for (int i = 0; i < edge_count; ++i) {
            edges.push_back({weight_dist(rng), all_pairs[i].first, all_pairs[i].second});
        }
        ok &= toolbox::test_utils::check(toolbox::graph::kruskal(n, edges) == brute_mst(n, edges),
                                         "kruskal randomized trial " + std::to_string(trial));
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
        {"randomized", test_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
