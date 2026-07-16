#pragma once

#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include "toolbox/datastructure/unionfind/unionfind.hpp"

namespace toolbox {

namespace graph {

/**
 * @brief Kruskal's algorithm returning the edges of a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The edges forming a minimum spanning tree (each as {cost, u, v}), or std::nullopt if
 * the graph is not connected (no spanning tree exists). A connected graph yields exactly n-1
 * edges; a single vertex yields an empty vector.
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E), dominated by sorting the edges.
 * @note Uses toolbox::datastructure::unionfind to detect cycles. The returned edges are given
 * in nondecreasing cost order (the order in which Kruskal selects them).
 */
template <typename Vertex, typename Cost>
std::optional<std::vector<std::tuple<Cost, Vertex, Vertex>>> kruskal_edges(
    int n, std::vector<std::tuple<Cost, Vertex, Vertex>> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const auto &a, const auto &b) { return std::get<0>(a) < std::get<0>(b); });
    datastructure::unionfind uf(n);
    std::vector<std::tuple<Cost, Vertex, Vertex>> tree;
    for (const auto &[cost, u, v] : edges) {
        if (uf.unite(static_cast<int>(u), static_cast<int>(v))) {
            tree.push_back({cost, u, v});
        }
    }
    if (static_cast<int>(tree.size()) != n - 1) {
        return std::nullopt;
    }
    return tree;
}

/**
 * @brief Kruskal's algorithm returning the edges of a minimum spanning tree.
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The edges of a minimum spanning tree, or std::nullopt if not connected.
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E)
 */
std::optional<std::vector<std::tuple<long long, long long, long long>>> kruskal_edges(
    int n, std::vector<std::tuple<long long, long long, long long>> edges) {
    return kruskal_edges<long long, long long>(n, std::move(edges));
}

/**
 * @brief Kruskal's algorithm returning the total cost of a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E)
 * @note Use kruskal_edges if the selected edges themselves are needed.
 */
template <typename Vertex, typename Cost>
std::optional<Cost> kruskal(int n, std::vector<std::tuple<Cost, Vertex, Vertex>> edges) {
    const auto tree = kruskal_edges<Vertex, Cost>(n, std::move(edges));
    if (!tree) {
        return std::nullopt;
    }
    Cost total = Cost();
    for (const auto &[cost, u, v] : *tree) {
        total += cost;
    }
    return total;
}

/**
 * @brief Kruskal's algorithm returning the total cost of a minimum spanning tree.
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E)
 */
std::optional<long long> kruskal(int n,
                                 std::vector<std::tuple<long long, long long, long long>> edges) {
    return kruskal<long long, long long>(n, std::move(edges));
}

}  // namespace graph

}  // namespace toolbox
