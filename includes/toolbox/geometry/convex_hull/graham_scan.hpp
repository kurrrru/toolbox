#pragma once

#include <algorithm>
#include <vector>

#include "toolbox/geometry/convex_hull/common.hpp"
#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

/**
 * @brief Computes the convex hull of a set of points using the Graham scan algorithm.
 * @tparam T The coordinate type (integer recommended for exact predicates).
 * @param points The input points.
 * @param include_collinear If true, points lying on the hull's edges are kept; if false
 * (default), only the extreme vertices are returned.
 * @return Indices into `points` of the hull vertices in counterclockwise order, with the start
 * vertex not repeated at the end. Among coincident input points, one representative index is
 * returned. For 0, 1, or 2 distinct points the indices of those points are returned as-is.
 * @note [complexity]: O(n log n), dominated by the angular sort around the pivot.
 */
template <typename T>
std::vector<int> convex_hull_graham_scan(const std::vector<Point<T>> &points,
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

    // Pivot: lowest y, breaking ties by lowest x (guaranteed to be a hull vertex).
    int pivot = idx[0];
    for (const int i : idx) {
        if (points[i].y < points[pivot].y ||
            (points[i].y == points[pivot].y && points[i].x < points[pivot].x)) {
            pivot = i;
        }
    }
    // Sort the remaining points by polar angle around the pivot; ties (collinear with the
    // pivot) are ordered by increasing distance.
    std::vector<int> sorted;
    for (const int i : idx) {
        if (i != pivot) {
            sorted.push_back(i);
        }
    }
    const auto by_angle = [&](int a, int b) {
        const T turn = cross(points[pivot], points[a], points[b]);
        if (turn != 0) {
            return turn > 0;  // a comes first when it is counterclockwise of b
        }
        return dot(points[pivot], points[a], points[a]) <
               dot(points[pivot], points[b], points[b]);  // nearer first
    };
    std::sort(sorted.begin(), sorted.end(), by_angle);

    // When collinear points are wanted, the final collinear run (points sharing the largest
    // polar angle) must be visited from farthest to nearest so they all stay on the hull.
    if (include_collinear) {
        int tail = static_cast<int>(sorted.size()) - 1;
        while (tail > 0 &&
               cross(points[pivot], points[sorted[tail]], points[sorted[tail - 1]]) == 0) {
            --tail;
        }
        std::reverse(sorted.begin() + tail, sorted.end());
    }

    std::vector<int> hull;
    hull.push_back(pivot);
    for (const int i : sorted) {
        while (hull.size() >= 2) {
            const T turn = cross(points[hull[hull.size() - 2]], points[hull.back()], points[i]);
            if (turn < 0 || (turn == 0 && !include_collinear)) {
                hull.pop_back();
            } else {
                break;
            }
        }
        hull.push_back(i);
    }
    return hull;
}

}  // namespace geometry

}  // namespace toolbox
