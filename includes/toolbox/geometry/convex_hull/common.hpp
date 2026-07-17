#pragma once

#include <algorithm>
#include <numeric>
#include <vector>

#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

namespace helper {

/**
 * @brief Returns the indices of `points` with coincident coordinates removed (one
 * representative each), sorted lexicographically by (x, y).
 * @note [Attention]: helper for the convex hull algorithms, not intended for direct use.
 * @note [complexity]: O(n log n).
 */
template <typename T>
std::vector<int> distinct_sorted_indices(const std::vector<Point<T>> &points) {
    std::vector<int> order(points.size());
    std::iota(order.begin(), order.end(), 0);
    std::sort(order.begin(), order.end(),
              [&points](int a, int b) { return points[a] < points[b]; });
    std::vector<int> idx;
    for (const int i : order) {
        if (idx.empty() || points[idx.back()] != points[i]) {
            idx.push_back(i);
        }
    }
    return idx;
}

/**
 * @brief Handles the degenerate case where all distinct points are collinear.
 * @param points The input points.
 * @param idx Distinct point indices, sorted lexicographically (as from distinct_sorted_indices).
 * @param include_collinear Whether collinear points are kept.
 * @param out Receives the degenerate hull if the points are collinear.
 * @return true if all points are collinear (and `out` was written): the two extreme endpoints
 * when excluding collinear points, or the whole ordered chain when including them.
 * @note [Attention]: helper for the convex hull algorithms, not intended for direct use.
 * @note [complexity]: O(n).
 */
template <typename T>
bool collinear_hull(const std::vector<Point<T>> &points, const std::vector<int> &idx,
                    bool include_collinear, std::vector<int> &out) {
    const int m = static_cast<int>(idx.size());
    for (int i = 2; i < m; ++i) {
        if (cross(points[idx[0]], points[idx[1]], points[idx[i]]) != 0) {
            return false;
        }
    }
    if (include_collinear) {
        out = idx;
    } else {
        out = {idx[0], idx[m - 1]};
    }
    return true;
}

/**
 * @brief Given a strict convex hull (CCW vertex indices, no collinear vertices), inserts every
 * input point lying on a hull edge, ordered along that edge, and returns the expanded hull.
 * @param points The input points.
 * @param idx Distinct point indices (as from distinct_sorted_indices).
 * @param hull Strict hull vertex indices in counterclockwise order.
 * @return The hull with collinear boundary points inserted, still counterclockwise.
 * @note [Attention]: helper for the convex hull algorithms, not intended for direct use.
 * @note [complexity]: O(h * n) where h is the number of strict hull edges.
 */
template <typename T>
std::vector<int> insert_collinear_points(const std::vector<Point<T>> &points,
                                         const std::vector<int> &idx,
                                         const std::vector<int> &hull) {
    const int h = static_cast<int>(hull.size());
    std::vector<int> expanded;
    for (int i = 0; i < h; ++i) {
        const int a = hull[i];
        const int b = hull[(i + 1) % h];
        expanded.push_back(a);
        // Collect input points strictly inside the segment a-b (collinear and between).
        std::vector<int> on_edge;
        for (const int p : idx) {
            if (p == a || p == b) {
                continue;
            }
            if (cross(points[a], points[b], points[p]) == 0 &&
                dot(points[p], points[a], points[b]) < 0) {  // p is between a and b
                on_edge.push_back(p);
            }
        }
        std::sort(on_edge.begin(), on_edge.end(), [&](int p, int q) {
            return dot(points[a], points[p], points[p]) < dot(points[a], points[q], points[q]);
        });
        for (const int p : on_edge) {
            expanded.push_back(p);
        }
    }
    return expanded;
}

}  // namespace helper

}  // namespace geometry

}  // namespace toolbox
