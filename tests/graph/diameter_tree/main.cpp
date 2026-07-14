#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "toolbox/graph/other/diameter_tree.hpp"

#include "utils/test_util.hpp"

namespace {

typedef std::vector<std::vector<std::pair<long long, long long>>> TreeGraph;

void add_tree_edge(TreeGraph &g, long long u, long long v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}

// Reference diameter: the maximum over all vertices s of the farthest distance from s,
// computed by a DFS from every vertex. O(V^2) but simple and obviously correct.
long long brute_diameter(const TreeGraph &g) {
    const int n = static_cast<int>(g.size());
    long long best = 0;
    for (int s = 0; s < n; ++s) {
        std::vector<long long> dist(n, -1);
        std::vector<int> stack = {s};
        dist[s] = 0;
        while (!stack.empty()) {
            const int u = stack.back();
            stack.pop_back();
            for (const auto &[v, w] : g[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + w;
                    stack.push_back(static_cast<int>(v));
                }
            }
        }
        best = std::max(best, *std::max_element(dist.begin(), dist.end()));
    }
    return best;
}

// ---- basic shapes ----------------------------------------------------------

bool test_path() {
    // Path graph: 0 -1- 1 -2- 2 -3- 3  diameter = 6
    TreeGraph g(4);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 1, 2, 2);
    add_tree_edge(g, 2, 3, 3);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 6, "diameter path: 6");
}

bool test_star() {
    // Star: center=0, leaves 1..4 with weights 1,5,3,2 -> diameter = 5+3 = 8
    TreeGraph g(5);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 0, 2, 5);
    add_tree_edge(g, 0, 3, 3);
    add_tree_edge(g, 0, 4, 2);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 8, "diameter star: 8");
}

bool test_single_edge() {
    TreeGraph g(2);
    add_tree_edge(g, 0, 1, 7);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 7,
                                      "diameter single edge: 7");
}

bool test_balanced() {
    //        0
    //       / \   (edges all weight 1)
    //      1   2
    //     / \.
    //    3   4
    // diameter: 3->1->0->2 = 3
    TreeGraph g(5);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 0, 2, 1);
    add_tree_edge(g, 1, 3, 1);
    add_tree_edge(g, 1, 4, 1);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 3,
                                      "diameter balanced tree: 3");
}

// ---- edge cases ------------------------------------------------------------

bool test_single_node() {
    // A single vertex with no edges has diameter 0.
    TreeGraph g(1);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 0,
                                      "diameter single node: 0");
}

bool test_caterpillar() {
    // Spine 0-1-2-3 (weights 2 each) with leaves hanging off, so the longest path runs
    // through two leaves at opposite ends.
    //   4       5
    //   |       |
    //   0--2--1--2--2--2--3
    TreeGraph g(6);
    add_tree_edge(g, 0, 1, 2);
    add_tree_edge(g, 1, 2, 2);
    add_tree_edge(g, 2, 3, 2);
    add_tree_edge(g, 0, 4, 1);  // leaf on the 0 end
    add_tree_edge(g, 3, 5, 1);  // leaf on the 3 end
    // longest: 4-0-1-2-3-5 = 1 + 2 + 2 + 2 + 1 = 8
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 8,
                                      "diameter caterpillar: 8");
}

bool test_diameter_off_center() {
    // The diameter does not pass through vertex 0: the long path is between two deep leaves.
    //   0
    //   |1
    //   1
    //  2/ \2
    //  2   3
    //  |10 |10
    //  4   5
    // longest path: 4-2-1-3-5 = 10 + 2 + 2 + 10 = 24
    TreeGraph g(6);
    add_tree_edge(g, 0, 1, 1);
    add_tree_edge(g, 1, 2, 2);
    add_tree_edge(g, 1, 3, 2);
    add_tree_edge(g, 2, 4, 10);
    add_tree_edge(g, 3, 5, 10);
    return toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == 24,
                                      "diameter off-center: 24");
}

// ---- randomized cross-check ------------------------------------------------

bool test_randomized() {
    // Build random trees by attaching each new vertex i to a random earlier vertex, then
    // compare the two-BFS diameter against the O(V^2) all-pairs reference.
    std::mt19937 rng(31337);
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(1, 20)(rng);
        TreeGraph g(n);
        std::uniform_int_distribution<int> weight(1, 50);
        for (int i = 1; i < n; ++i) {
            const int parent = std::uniform_int_distribution<int>(0, i - 1)(rng);
            add_tree_edge(g, parent, i, weight(rng));
        }
        ok &= toolbox::test_utils::check(toolbox::graph::diameter_tree(g) == brute_diameter(g),
                                         "diameter randomized trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"path", test_path},
        {"star", test_star},
        {"single_edge", test_single_edge},
        {"balanced", test_balanced},
        {"single_node", test_single_node},
        {"caterpillar", test_caterpillar},
        {"diameter_off_center", test_diameter_off_center},
        {"randomized", test_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
