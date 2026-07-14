#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include "toolbox/datastructure/unionfind/unionfind.hpp"

namespace toolbox {

namespace graph {

/**
 * @brief Boruvka's algorithm for finding a minimum spanning tree.
 * @tparam Vertex Type of vertex
 * @tparam Cost Type of cost
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log V). Each round every component picks its cheapest outgoing edge
 * and those edges are merged in; the number of components at least halves per round, so there
 * are O(log V) rounds and each round scans all edges in O(E).
 * @note Uses toolbox::datastructure::unionfind for components; like kruskal/prim (see
 * kruskal.hpp, prim.hpp), it returns only the total cost, not the selected edges.
 */
template <typename Vertex, typename Cost>
std::optional<Cost> boruvka(int n, const std::vector<std::tuple<Cost, Vertex, Vertex>> &edges) {
    const int m = static_cast<int>(edges.size());
    datastructure::unionfind uf(n);
    Cost total = Cost();
    int num_components = n;

    // A consistent tie-break (cost first, then edge index) makes every edge effectively
    // unique. This guarantees the edges chosen by different components within a single round
    // can never form a cycle among themselves, so Boruvka stays correct even when several
    // edges share the same weight.
    const auto cheaper = [&edges](const int lhs, const int rhs) {
        if (std::get<0>(edges[lhs]) != std::get<0>(edges[rhs])) {
            return std::get<0>(edges[lhs]) < std::get<0>(edges[rhs]);
        }
        return lhs < rhs;
    };

    while (num_components > 1) {
        // For each component (identified by its union-find representative), the index of the
        // cheapest edge leaving it, or -1 if the component has no outgoing edge this round.
        std::vector<int> cheapest(n, -1);
        for (int i = 0; i < m; ++i) {
            const int cu = uf.find(static_cast<int>(std::get<1>(edges[i])));
            const int cv = uf.find(static_cast<int>(std::get<2>(edges[i])));
            if (cu == cv) {
                continue;  // both endpoints already merged (also skips self-loops)
            }
            if (cheapest[cu] == -1 || cheaper(i, cheapest[cu])) {
                cheapest[cu] = i;
            }
            if (cheapest[cv] == -1 || cheaper(i, cheapest[cv])) {
                cheapest[cv] = i;
            }
        }

        bool progress = false;
        for (int c = 0; c < n; ++c) {
            if (cheapest[c] == -1) {
                continue;
            }
            const auto &[cost, u, v] = edges[cheapest[c]];
            if (uf.unite(static_cast<int>(u), static_cast<int>(v))) {
                total += cost;
                --num_components;
                progress = true;
            }
        }
        if (!progress) {
            break;  // no component could grow: the graph is disconnected
        }
    }

    if (num_components != 1) {
        return std::nullopt;
    }
    return total;
}

/**
 * @brief Boruvka's algorithm for finding a minimum spanning tree.
 * @param n Number of vertices
 * @param edges Edge list, each edge given as {cost, u, v}
 * @return The total cost of the minimum spanning tree, or std::nullopt if the graph is not
 * connected (no spanning tree exists).
 * @note [constraint]: n > 0
 * @note [complexity]: O(E log V)
 */
std::optional<long long> boruvka(
    int n, const std::vector<std::tuple<long long, long long, long long>> &edges) {
    return boruvka<long long, long long>(n, edges);
}

}  // namespace graph

}  // namespace toolbox
