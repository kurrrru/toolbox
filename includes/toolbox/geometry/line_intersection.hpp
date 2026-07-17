#pragma once

#include <algorithm>

#include "toolbox/geometry/point.hpp"

namespace toolbox {

namespace geometry {

/**
 * @brief Tests whether point p lies on the segment [a, b].
 * @tparam T The coordinate type (integer recommended for exact results).
 * @return true if p is collinear with a and b and lies within the segment (endpoints included).
 * @note [complexity]: O(1).
 */
template <typename T>
bool on_segment(const Point<T> &p, const Point<T> &a, const Point<T> &b) {
    if (cross(a, b, p) != 0) {
        return false;  // not collinear
    }
    // Collinear: p is on the segment iff (a - p) and (b - p) point in opposite directions.
    return dot(p, a, b) <= 0;
}

/**
 * @brief Tests whether the segments [p1, p2] and [p3, p4] share at least one point.
 * @tparam T The coordinate type (integer recommended for exact results: the test uses only
 * cross products and comparisons, no division).
 * @param p1 First endpoint of the first segment.
 * @param p2 Second endpoint of the first segment.
 * @param p3 First endpoint of the second segment.
 * @param p4 Second endpoint of the second segment.
 * @return true if the two closed segments intersect, including when they merely touch at an
 * endpoint or overlap along a collinear stretch. Degenerate segments (a segment whose two
 * endpoints coincide, i.e. a point) are handled correctly.
 * @note [complexity]: O(1).
 */
template <typename T>
bool segments_intersect(const Point<T> &p1, const Point<T> &p2, const Point<T> &p3,
                        const Point<T> &p4) {
    const T d1 = cross(p3, p4, p1);
    const T d2 = cross(p3, p4, p2);
    const T d3 = cross(p1, p2, p3);
    const T d4 = cross(p1, p2, p4);
    // General case: p1, p2 lie on opposite sides of line p3-p4, and vice versa (strict
    // straddle on both). sgn(d1) != sgn(d2) and sgn(d3) != sgn(d4) with all nonzero.
    if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) {
        return true;
    }
    // Collinear / touching cases: an endpoint of one segment lies on the other segment.
    if (d1 == 0 && on_segment(p1, p3, p4)) {
        return true;
    }
    if (d2 == 0 && on_segment(p2, p3, p4)) {
        return true;
    }
    if (d3 == 0 && on_segment(p3, p1, p2)) {
        return true;
    }
    if (d4 == 0 && on_segment(p4, p1, p2)) {
        return true;
    }
    return false;
}

}  // namespace geometry

}  // namespace toolbox
