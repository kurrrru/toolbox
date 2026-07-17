#pragma once

#include <algorithm>
#include <vector>

#include "toolbox/geometry/convex_hull/common.hpp"  // helper::distinct_sorted_indices, collinear_hull
#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

namespace helper {

// Recursively finds the hull vertices strictly between a and b lying to the left of the
// directed segment a->b (cross(a, b, p) > 0), appending them to `out` in order from a to b.
template <typename T>
void quickhull_recurse(const std::vector<Point<T>> &points, int a, int b,
                       const std::vector<int> &candidates, std::vector<int> &out) {
    if (candidates.empty()) {
        return;
    }
    int farthest = candidates[0];
    T best = cross(points[a], points[b], points[farthest]);
    for (const int p : candidates) {
        const T d = cross(points[a], points[b], points[p]);
        if (d > best) {
            best = d;
            farthest = p;
        }
    }
    std::vector<int> left_of_ac;
    std::vector<int> left_of_cb;
    for (const int p : candidates) {
        if (p == farthest) {
            continue;
        }
        if (cross(points[a], points[farthest], points[p]) > 0) {
            left_of_ac.push_back(p);
        } else if (cross(points[farthest], points[b], points[p]) > 0) {
            left_of_cb.push_back(p);
        }
    }
    quickhull_recurse(points, a, farthest, left_of_ac, out);
    out.push_back(farthest);
    quickhull_recurse(points, farthest, b, left_of_cb, out);
}

}  // namespace helper

/**
 * @brief Computes the convex hull of a set of points using the Quickhull algorithm.
 * @tparam T The coordinate type (integer recommended for exact predicates).
 * @param points The input points.
 * @param include_collinear If true, points lying on the hull's edges are kept; if false
 * (default), only the extreme vertices are returned.
 * @return Indices into `points` of the hull vertices in counterclockwise order, with the start
 * vertex not repeated at the end. Among coincident input points, one representative index is
 * returned. For 0, 1, or 2 distinct points the indices of those points are returned as-is.
 * @note [complexity]: O(n log n) on average, O(n^2) in the worst case.
 */
template <typename T>
std::vector<int> convex_hull_quickhull(const std::vector<Point<T>> &points,
                                       bool include_collinear = false) {
    const std::vector<int> idx = helper::distinct_sorted_indices(points);
    const int m = static_cast<int>(idx.size());
    if (m <= 2) {
        return idx;
    }
    std::vector<int> degenerate;
    if (helper::collinear_hull(points, idx, include_collinear, degenerate)) {
        return degenerate;
    }

    // Split by the line through the leftmost (a) and rightmost (b) points.
    const int a = idx[0];
    const int b = idx[m - 1];
    std::vector<int> above;  // left of a->b
    std::vector<int> below;  // left of b->a
    for (const int p : idx) {
        if (p == a || p == b) {
            continue;
        }
        const T side = cross(points[a], points[b], points[p]);
        if (side > 0) {
            above.push_back(p);
        } else if (side < 0) {
            below.push_back(p);
        }
    }
    // Build the chain a -> above -> b -> below (this order is clockwise), then reverse the tail
    // so the final vertex list runs counterclockwise starting at a.
    std::vector<int> chain;
    chain.push_back(a);
    helper::quickhull_recurse(points, a, b, above, chain);
    chain.push_back(b);
    helper::quickhull_recurse(points, b, a, below, chain);
    std::vector<int> hull;
    hull.push_back(a);
    for (int i = static_cast<int>(chain.size()) - 1; i >= 1; --i) {
        hull.push_back(chain[i]);
    }

    if (include_collinear) {
        return helper::insert_collinear_points(points, idx, hull);
    }
    return hull;
}

}  // namespace geometry

}  // namespace toolbox
