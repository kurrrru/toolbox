#pragma once

#include <functional>
#include <optional>
#include <queue>
#include <vector>

namespace toolbox {

namespace graph {

/**
 * @brief Prim's algorithm for finding a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param cost Adjacency list of the graph
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: cost.size() > 0
 * @note [complexity]: O(E log V), growing the tree from vertex 0 with a priority queue,
 * the same approach as dijkstra in this library.
 * @note Like kruskal (see kruskal.hpp), only the total cost is returned, not the selected
 * edges themselves.
 */
template <typename Vertex, typename Cost>
std::optional<Cost> prim(const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost) {
    const int n = static_cast<int>(cost.size());
    std::vector<bool> visited(n, false);
    std::priority_queue<std::pair<Cost, Vertex>, std::vector<std::pair<Cost, Vertex>>,
                        std::greater<std::pair<Cost, Vertex>>>
        que;
    que.push({Cost(), Vertex()});
    Cost total = Cost();
    int visited_count = 0;
    while (!que.empty()) {
        auto [w, u] = que.top();
        que.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        total += w;
        ++visited_count;
        for (const auto &[v, v_cost] : cost[u]) {
            if (!visited[v]) {
                que.push({v_cost, v});
            }
        }
    }
    if (visited_count != n) {
        return std::nullopt;
    }
    return total;
}

/**
 * @brief Prim's algorithm for finding a minimum spanning tree.
 * @param cost Adjacency list of the graph
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: cost.size() > 0
 * @note [complexity]: O(E log V)
 */
std::optional<long long> prim(
    const std::vector<std::vector<std::pair<long long, long long>>> &cost) {
    return prim<long long, long long>(cost);
}

}  // namespace graph

}  // namespace toolbox
