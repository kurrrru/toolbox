#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "toolbox/graph/directed/topological.hpp"

#include "utils/test_util.hpp"

namespace {

typedef std::vector<std::vector<long long>> DiGraph;

// Checks that `order` is a permutation of [0, n) in which every edge u->v has u before v.
bool is_valid_topological(const std::vector<long long> &order, const DiGraph &edge) {
    const int n = static_cast<int>(edge.size());
    if (static_cast<int>(order.size()) != n) {
        return false;
    }
    std::vector<int> pos(n, -1);
    for (int i = 0; i < n; ++i) {
        if (order[i] < 0 || order[i] >= n || pos[order[i]] != -1) {
            return false;  // out of range or duplicate
        }
        pos[order[i]] = i;
    }
    for (int u = 0; u < n; ++u) {
        for (long long v : edge[u]) {
            if (pos[u] >= pos[v]) {
                return false;
            }
        }
    }
    return true;
}

// ---- basic cases -----------------------------------------------------------

bool test_basic() {
    // 0 -> 1, 0 -> 2, 1 -> 3, 2 -> 3
    DiGraph edge(4);
    edge[0].push_back(1);
    edge[0].push_back(2);
    edge[1].push_back(3);
    edge[2].push_back(3);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= toolbox::test_utils::check(order.size() == 4, "topological basic: all nodes in order");
    ok &= toolbox::test_utils::check(is_valid_topological(order, edge),
                                     "topological basic: valid ordering");
    return ok;
}

bool test_chain() {
    // 0 -> 1 -> 2 -> 3 -> 4 : the only valid order is 0,1,2,3,4
    DiGraph edge(5);
    for (int i = 0; i < 4; ++i) {
        edge[i].push_back(i + 1);
    }
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= toolbox::test_utils::check(order.size() == 5, "topological chain: all nodes");
    ok &= toolbox::test_utils::check(order == std::vector<long long>({0, 1, 2, 3, 4}),
                                     "topological chain: unique order 0..4");
    return ok;
}

bool test_single() {
    DiGraph edge(1);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= toolbox::test_utils::check(order.size() == 1, "topological single: size==1");
    ok &= toolbox::test_utils::check(order[0] == 0, "topological single: order[0]==0");
    return ok;
}

bool test_no_edges() {
    // No edges: any permutation is valid, but all n vertices must appear.
    DiGraph edge(5);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= toolbox::test_utils::check(order.size() == 5, "topological no edges: all 5 nodes");
    ok &= toolbox::test_utils::check(is_valid_topological(order, edge),
                                     "topological no edges: valid (trivially)");
    return ok;
}

bool test_disconnected_dag() {
    // Two independent chains: 0->1 and 2->3, no edge between the components.
    DiGraph edge(4);
    edge[0].push_back(1);
    edge[2].push_back(3);
    auto order = toolbox::graph::topological(edge);
    bool ok = true;
    ok &= toolbox::test_utils::check(order.size() == 4, "topological disconnected: all nodes");
    ok &= toolbox::test_utils::check(is_valid_topological(order, edge),
                                     "topological disconnected: valid ordering");
    return ok;
}

// ---- cycle detection -------------------------------------------------------

bool test_cycle() {
    // 0 -> 1 -> 2 -> 0 (cycle)
    DiGraph edge(3);
    edge[0].push_back(1);
    edge[1].push_back(2);
    edge[2].push_back(0);
    auto order = toolbox::graph::topological(edge);
    return toolbox::test_utils::check(order.empty(), "topological cycle: returns empty");
}

bool test_self_loop() {
    // A self-loop 1->1 is a cycle, so no topological order exists.
    DiGraph edge(3);
    edge[0].push_back(1);
    edge[1].push_back(1);  // self-loop
    auto order = toolbox::graph::topological(edge);
    return toolbox::test_utils::check(order.empty(), "topological self-loop: returns empty");
}

bool test_partial_cycle() {
    // A DAG portion 0->1 plus a separate cycle 2->3->2: still no valid full order.
    DiGraph edge(4);
    edge[0].push_back(1);
    edge[2].push_back(3);
    edge[3].push_back(2);
    auto order = toolbox::graph::topological(edge);
    return toolbox::test_utils::check(order.empty(), "topological partial cycle: returns empty");
}

// ---- randomized validity check ---------------------------------------------

bool test_randomized_dag() {
    // Build random DAGs by only allowing edges u->v with u < v (guarantees acyclic),
    // then verify the returned order is a valid topological order.
    std::mt19937 rng(1234567);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 12)(rng);
        DiGraph edge(n);
        std::bernoulli_distribution coin(0.3);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                if (coin(rng)) {
                    edge[u].push_back(v);
                }
            }
        }
        auto order = toolbox::graph::topological(edge);
        ok &=
            toolbox::test_utils::check(is_valid_topological(order, edge),
                                       "topological randomized DAG trial " + std::to_string(trial));
    }
    return ok;
}

bool test_randomized_cyclic() {
    // Random graphs that contain at least one back edge (u->v with u > v) plus the forward
    // DAG edges are not guaranteed cyclic, so instead build a guaranteed cycle and add random
    // forward edges: the presence of the cycle must always force an empty result.
    std::mt19937 rng(7654321);
    bool ok = true;
    for (int trial = 0; trial < 200 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(2, 12)(rng);
        DiGraph edge(n);
        // Guaranteed cycle 0 -> 1 -> ... -> k -> 0 for some k in [1, n-1].
        const int k = std::uniform_int_distribution<int>(1, n - 1)(rng);
        for (int i = 0; i < k; ++i) {
            edge[i].push_back(i + 1);
        }
        edge[k].push_back(0);
        // Extra random forward edges (do not remove the cycle).
        std::bernoulli_distribution coin(0.2);
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                if (coin(rng)) {
                    edge[u].push_back(v);
                }
            }
        }
        auto order = toolbox::graph::topological(edge);
        ok &= toolbox::test_utils::check(
            order.empty(), "topological randomized cyclic trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"basic", test_basic},
        {"chain", test_chain},
        {"single", test_single},
        {"no_edges", test_no_edges},
        {"disconnected_dag", test_disconnected_dag},
        {"cycle", test_cycle},
        {"self_loop", test_self_loop},
        {"partial_cycle", test_partial_cycle},
        {"randomized_dag", test_randomized_dag},
        {"randomized_cyclic", test_randomized_cyclic},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
