#pragma once

#include <vector>

#include "toolbox/geometry/convex_hull/common.hpp"
#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

/**
 * @brief Computes the convex hull of a set of points using the Jarvis march (gift wrapping)
 * algorithm.
 * @tparam T The coordinate type (integer recommended for exact predicates).
 * @param points The input points.
 * @param include_collinear If true, points lying on the hull's edges are kept; if false
 * (default), only the extreme vertices are returned.
 * @return Indices into `points` of the hull vertices in counterclockwise order, with the start
 * vertex not repeated at the end. Among coincident input points, one representative index is
 * returned. For 0, 1, or 2 distinct points the indices of those points are returned as-is.
 * @note [complexity]: O(n * h), where h is the number of hull vertices output (each wrapping
 * step scans all points). Worst case O(n^2) when most points are on the hull.
 */
template <typename T>
std::vector<int> convex_hull_jarvis_march(const std::vector<Point<T>> &points,
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

    // Wrap the strict hull (collinear points excluded), then, if requested, insert the
    // collinear boundary points afterward. Building the strict hull first keeps the wrapping
    // step's next-vertex choice unambiguous: at a genuine hull vertex the incoming and outgoing
    // edges are never collinear, so collinear ties only ever involve points in the forward
    // (outgoing) direction, where "farthest" reliably selects the true extreme.
    const int start = idx[0];  // lexicographically smallest point (a hull vertex)
    std::vector<int> hull;
    int current = start;
    // The wrap visits each hull vertex once, so it returns to the start within m steps; the cap
    // is a defensive guard against a non-terminating wrap on unexpected input.
    for (int guard = 0; guard <= m; ++guard) {
        hull.push_back(current);
        // Choose the next vertex so that every other point lies to the left of the directed
        // edge current -> next (which walks the hull counterclockwise).
        int next = (current == idx[0]) ? idx[1] : idx[0];
        for (const int cand : idx) {
            if (cand == current) {
                continue;
            }
            const T turn = cross(points[current], points[next], points[cand]);
            if (turn < 0) {
                next = cand;  // cand lies to the right: swing the edge clockwise toward it
            } else if (turn == 0 && cand != next &&
                       dot(points[current], points[next], points[cand]) > 0) {
                // Collinear and in the forward direction: keep the farthest as the vertex so
                // intermediate points are skipped from the strict hull.
                if (dot(points[current], points[cand], points[cand]) >
                    dot(points[current], points[next], points[next])) {
                    next = cand;
                }
            }
        }
        current = next;
        if (current == start) {
            break;
        }
    }

    if (include_collinear) {
        return helper::insert_collinear_points(points, idx, hull);
    }
    return hull;
}

}  // namespace geometry

}  // namespace toolbox
