#pragma once

#include <algorithm>
#include <optional>
#include <tuple>
#include <vector>

#include "toolbox/datastructure/unionfind/unionfind.hpp"

namespace toolbox {

namespace graph {

/**
 * @brief Kruskal's algorithm for finding a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E), dominated by sorting the edges.
 * @note Uses toolbox::datastructure::unionfind to detect cycles; does not reconstruct the
 * selected edges themselves (only their total cost), matching the level of detail returned by
 * dijkstra/bellman_ford in this library.
 */
template <typename Vertex, typename Cost>
std::optional<Cost> kruskal(int n, std::vector<std::tuple<Cost, Vertex, Vertex>> edges) {
    std::sort(edges.begin(), edges.end(),
              [](const auto &a, const auto &b) { return std::get<0>(a) < std::get<0>(b); });
    datastructure::unionfind uf(n);
    Cost total = Cost();
    int edges_used = 0;
    for (const auto &[cost, u, v] : edges) {
        if (uf.unite(static_cast<int>(u), static_cast<int>(v))) {
            total += cost;
            ++edges_used;
        }
    }
    if (edges_used != n - 1) {
        return std::nullopt;
    }
    return total;
}

/**
 * @brief Kruskal's algorithm for finding a minimum spanning tree.
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log E)
 */
std::optional<long long> kruskal(int n,
                                 std::vector<std::tuple<long long, long long, long long>> edges) {
    return kruskal<long long, long long>(n, edges);
}

}  // namespace graph

}  // namespace toolbox
