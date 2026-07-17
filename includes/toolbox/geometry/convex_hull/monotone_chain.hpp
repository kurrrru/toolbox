#pragma once

#include <vector>

#include "toolbox/geometry/convex_hull/common.hpp"
#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

/**
 * @brief Computes the convex hull of a set of points using Andrew's monotone chain algorithm.
 * @tparam T The coordinate type (integer recommended for exact predicates).
 * @param points The input points.
 * @param include_collinear If true, points lying on the hull's edges are kept; if false
 * (default), only the extreme vertices are returned.
 * @return Indices into `points` of the hull vertices in counterclockwise order, with the start
 * vertex not repeated at the end. Among coincident input points, one representative index is
 * returned. For 0, 1, or 2 distinct points the indices of those points are returned as-is.
 * @note [complexity]: O(n log n), dominated by the initial sort.
 */
template <typename T>
std::vector<int> convex_hull_monotone_chain(const std::vector<Point<T>> &points,
                                            bool include_collinear = false) {
    const std::vector<int> idx = helper::distinct_sorted_indices(points);
    const int m = static_cast<int>(idx.size());
    if (m <= 2) {
        return idx;  // 0, 1, or 2 distinct points: the hull is exactly those points
    }
    std::vector<int> degenerate;
    if (helper::collinear_hull(points, idx, include_collinear, degenerate)) {
        return degenerate;
    }

    // Build the lower then the upper hull. A vertex is popped when it does not make a
    // counterclockwise turn: strictly clockwise turns are always popped, and collinear turns
    // (cross == 0) are popped only when collinear points are to be excluded.
    const auto should_pop = [&](int a, int b, int c) {
        const T turn = cross(points[a], points[b], points[c]);
        return turn < 0 || (turn == 0 && !include_collinear);
    };
    std::vector<int> hull;
    for (int i = 0; i < m; ++i) {
        while (hull.size() >= 2 &&
               should_pop(hull[hull.size() - 2], hull[hull.size() - 1], idx[i])) {
            hull.pop_back();
        }
        hull.push_back(idx[i]);
    }
    const std::size_t lower_size = hull.size() + 1;
    for (int i = m - 2; i >= 0; --i) {
        while (hull.size() >= lower_size &&
               should_pop(hull[hull.size() - 2], hull[hull.size() - 1], idx[i])) {
            hull.pop_back();
        }
        hull.push_back(idx[i]);
    }
    hull.pop_back();  // the last point coincides with the first
    return hull;
}

}  // namespace geometry

}  // namespace toolbox
