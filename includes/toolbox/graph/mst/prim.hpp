#pragma once

#include <functional>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

namespace toolbox {

namespace graph {

/**
 * @brief Prim's algorithm returning the edges of a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param cost Adjacency list of the graph
 * @return The edges forming a minimum spanning tree (each as {cost, u, v}), or std::nullopt if
 * the graph is not connected (no spanning tree exists). A connected graph yields exactly n-1
 * edges; a single vertex yields an empty vector.
 * @note [constraint]: cost.size() > 0
 * @note [complexity]: O(E log V), growing the tree from vertex 0 with a priority queue,
 * the same approach as dijkstra in this library.
 * @note The returned edges are given in the order Prim attaches them to the tree.
 */
template <typename Vertex, typename Cost>
std::optional<std::vector<std::tuple<Cost, Vertex, Vertex>>> prim_edges(
    const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost) {
    const int n = static_cast<int>(cost.size());
    std::vector<bool> visited(n, false);
    // Priority queue entries are (edge cost, vertex to attach, vertex it is reached from).
    typedef std::tuple<Cost, Vertex, Vertex> QueueEntry;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> que;
    // Seed with vertex 0; its "from" equals itself to mark that it attaches no edge.
    que.push({Cost(), Vertex(), Vertex()});
    std::vector<std::tuple<Cost, Vertex, Vertex>> tree;
    int visited_count = 0;
    while (!que.empty()) {
        const auto [w, u, from] = que.top();
        que.pop();
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
        ++visited_count;
        // The seed (from == u) attaches no edge; every real attachment has from != u because
        // self-loops never reach here unvisited.
        if (from != u) {
            tree.push_back({w, from, u});
        }
        for (const auto &[v, v_cost] : cost[u]) {
            if (!visited[v]) {
                que.push({v_cost, v, u});
            }
        }
    }
    if (visited_count != n) {
        return std::nullopt;
    }
    return tree;
}

/**
 * @brief Prim's algorithm returning the edges of a minimum spanning tree.
 * @param cost Adjacency list of the graph
 * @return The edges of a minimum spanning tree, or std::nullopt if not connected.
 * @note [constraint]: cost.size() > 0
 * @note [complexity]: O(E log V)
 */
std::optional<std::vector<std::tuple<long long, long long, long long>>> prim_edges(
    const std::vector<std::vector<std::pair<long long, long long>>> &cost) {
    return prim_edges<long long, long long>(cost);
}

/**
 * @brief Prim's algorithm returning the total cost of a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param cost Adjacency list of the graph
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: cost.size() > 0
 * @note [complexity]: O(E log V)
 * @note Use prim_edges if the selected edges themselves are needed.
 */
template <typename Vertex, typename Cost>
std::optional<Cost> prim(const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost) {
    const auto tree = prim_edges<Vertex, Cost>(cost);
    if (!tree) {
        return std::nullopt;
    }
    Cost total = Cost();
    for (const auto &[edge_cost, u, v] : *tree) {
        total += edge_cost;
    }
    return total;
}

/**
 * @brief Prim's algorithm returning the total cost of a minimum spanning tree.
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
