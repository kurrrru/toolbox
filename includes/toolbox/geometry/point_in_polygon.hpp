#pragma once

#include <vector>

#include "toolbox/geometry/line_intersection.hpp"  // on_segment
#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

/**
 * @brief The position of a point relative to a polygon.
 */
enum class PolygonPosition {
    outside,   // strictly outside the polygon
    boundary,  // exactly on an edge or vertex
    inside,    // strictly inside the polygon
};

/**
 * @brief Locates a point relative to a simple polygon (convex or not) by ray casting.
 * @tparam T The coordinate type (integer recommended for exact results).
 * @param polygon The polygon vertices in order (clockwise or counterclockwise); the edge from
 * the last vertex back to the first is implied.
 * @param p The query point.
 * @return Whether p is inside, on the boundary of, or outside the polygon.
 * @note [constraint]: the polygon must be simple (non-self-intersecting) and have >= 3
 * vertices for a meaningful interior.
 * @note [complexity]: O(n), where n is the number of polygon vertices.
 * @note Uses a crossing-number (ray casting) test with exact integer predicates: a horizontal
 * ray is not actually cast; instead each edge that straddles the point's y-coordinate is
 * counted by the sign of a cross product, which avoids the usual floating-point ray tricks.
 */
template <typename T>
PolygonPosition point_in_polygon(const std::vector<Point<T>> &polygon, const Point<T> &p) {
    const int n = static_cast<int>(polygon.size());
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        const Point<T> &a = polygon[i];
        const Point<T> &b = polygon[j];
        if (on_segment(p, a, b)) {
            return PolygonPosition::boundary;
        }
        // Count edges crossing the upward ray from p. An edge (a, b) crosses when exactly one
        // endpoint is strictly above p.y and p lies to the left of the directed edge; the
        // cross product gives that side exactly.
        const bool a_above = a.y > p.y;
        const bool b_above = b.y > p.y;
        if (a_above != b_above) {
            // Side of p relative to edge a->b; sign tells whether the crossing is to the right.
            const T side = cross(a, b, p);
            if ((a_above && side < 0) || (!a_above && side > 0)) {
                inside = !inside;
            }
        }
    }
    return inside ? PolygonPosition::inside : PolygonPosition::outside;
}

/**
 * @brief Locates a point relative to a convex polygon in O(log n).
 * @tparam T The coordinate type (integer recommended for exact results).
 * @param polygon The convex polygon vertices in counterclockwise order, with no three
 * consecutive collinear vertices.
 * @param p The query point.
 * @return Whether p is inside, on the boundary of, or outside the polygon.
 * @note [constraint]: `polygon` must be convex, counterclockwise, and have >= 3 vertices.
 * @note [complexity]: O(log n). The polygon is viewed as a fan of triangles from vertex 0; a
 * binary search finds the sector containing p, then two cross products classify it.
 */
template <typename T>
PolygonPosition point_in_convex_polygon(const std::vector<Point<T>> &polygon, const Point<T> &p) {
    const int n = static_cast<int>(polygon.size());
    const Point<T> &o = polygon[0];
    // p must lie within the angular wedge from polygon[1] to polygon[n-1] as seen from o.
    const T side_first = cross(o, polygon[1], p);
    const T side_last = cross(o, polygon[n - 1], p);
    if (side_first < 0 || side_last > 0) {
        // Outside the wedge, but it may still lie on one of the two bounding edges.
        if (side_first == 0 && on_segment(p, o, polygon[1])) {
            return PolygonPosition::boundary;
        }
        if (side_last == 0 && on_segment(p, o, polygon[n - 1])) {
            return PolygonPosition::boundary;
        }
        return PolygonPosition::outside;
    }
    // Binary search for the sector [polygon[lo], polygon[lo + 1]] whose wedge contains p.
    int lo = 1;
    int hi = n - 1;
    while (hi - lo > 1) {
        const int mid = (lo + hi) / 2;
        if (cross(o, polygon[mid], p) >= 0) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    // Classify p against the triangle edge polygon[lo] -> polygon[lo + 1].
    const T edge = cross(polygon[lo], polygon[lo + 1], p);
    if (edge < 0) {
        return PolygonPosition::outside;
    }
    if (edge == 0) {
        return PolygonPosition::boundary;
    }
    // Strictly inside the triangle wedge; also check the two spokes for the boundary case.
    if (lo == 1 && side_first == 0) {
        return PolygonPosition::boundary;
    }
    if (lo == n - 2 && side_last == 0) {
        return PolygonPosition::boundary;
    }
    return PolygonPosition::inside;
}

}  // namespace geometry

}  // namespace toolbox
