#include <algorithm>
#include <string>
#include <vector>

#include "toolbox/geometry/point.hpp"
#include "toolbox/geometry/point_in_polygon.hpp"

#include "utils/test_util.hpp"

namespace {

typedef toolbox::geometry::Point<long long> P;
using toolbox::geometry::PolygonPosition;

// ---- independent reference (winding number) --------------------------------

long long cross2(const P &o, const P &a, const P &b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

bool on_seg_bbox(const P &p, const P &a, const P &b) {
    if (cross2(a, b, p) != 0) {
        return false;
    }
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) && std::min(a.y, b.y) <= p.y &&
           p.y <= std::max(a.y, b.y);
}

// Reference position via the winding-number algorithm (different counting logic than the
// implementation's crossing-number test).
PolygonPosition ref_position(const std::vector<P> &poly, const P &p) {
    const int n = static_cast<int>(poly.size());
    for (int i = 0; i < n; ++i) {
        if (on_seg_bbox(p, poly[i], poly[(i + 1) % n])) {
            return PolygonPosition::boundary;
        }
    }
    int wn = 0;
    for (int i = 0; i < n; ++i) {
        const P &a = poly[i];
        const P &b = poly[(i + 1) % n];
        if (a.y <= p.y) {
            if (b.y > p.y && cross2(a, b, p) > 0) {
                ++wn;
            }
        } else {
            if (b.y <= p.y && cross2(a, b, p) < 0) {
                --wn;
            }
        }
    }
    return wn != 0 ? PolygonPosition::inside : PolygonPosition::outside;
}

// ---- polygons --------------------------------------------------------------

std::vector<P> make_poly(const std::vector<std::pair<long long, long long>> &v) {
    std::vector<P> poly;
    for (const auto &[x, y] : v) {
        poly.push_back(P(x, y));
    }
    return poly;
}

// Compares the general point_in_polygon against the reference over a grid.
bool grid_matches_reference(const std::vector<P> &poly, int lo, int hi, const std::string &label) {
    bool ok = true;
    for (int x = lo; x <= hi && ok; ++x) {
        for (int y = lo; y <= hi && ok; ++y) {
            const P p(x, y);
            if (toolbox::geometry::point_in_polygon(poly, p) != ref_position(poly, p)) {
                ok = false;
            }
        }
    }
    return toolbox::test_utils::check(ok, label + ": matches winding reference on grid");
}

// Compares the convex O(log n) test against the general one over a grid (convex polygons only).
bool convex_matches_general(const std::vector<P> &poly, int lo, int hi, const std::string &label) {
    bool ok = true;
    for (int x = lo; x <= hi && ok; ++x) {
        for (int y = lo; y <= hi && ok; ++y) {
            const P p(x, y);
            if (toolbox::geometry::point_in_convex_polygon(poly, p) !=
                toolbox::geometry::point_in_polygon(poly, p)) {
                ok = false;
            }
        }
    }
    return toolbox::test_utils::check(ok, label + ": convex O(log n) matches general");
}

// ---- fixed cases -----------------------------------------------------------

bool test_square_fixed() {
    auto sq = make_poly({{0, 0}, {4, 0}, {4, 4}, {0, 4}});
    bool ok = true;
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(sq, P(2, 2)) == PolygonPosition::inside,
        "square: center inside");
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(sq, P(0, 2)) == PolygonPosition::boundary,
        "square: edge point boundary");
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(sq, P(0, 0)) == PolygonPosition::boundary,
        "square: vertex boundary");
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(sq, P(5, 2)) == PolygonPosition::outside,
        "square: outside");
    return ok;
}

bool test_nonconvex_l() {
    // An L-shaped (non-convex) polygon, counterclockwise.
    auto l = make_poly({{0, 0}, {4, 0}, {4, 2}, {2, 2}, {2, 4}, {0, 4}});
    bool ok = true;
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(l, P(1, 1)) == PolygonPosition::inside,
        "L-shape: inside lower arm");
    ok &= toolbox::test_utils::check(
        toolbox::geometry::point_in_polygon(l, P(3, 3)) == PolygonPosition::outside,
        "L-shape: in the notch (outside)");
    ok &= grid_matches_reference(l, -1, 5, "L-shape");
    return ok;
}

bool test_nonconvex_arrow() {
    // A non-convex arrow/chevron shape, counterclockwise.
    auto arrow = make_poly({{0, 0}, {6, 3}, {0, 6}, {2, 3}});
    return grid_matches_reference(arrow, -1, 7, "arrow");
}

bool test_triangle_grid() {
    auto tri = make_poly({{0, 0}, {6, 0}, {0, 6}});
    return grid_matches_reference(tri, -1, 7, "triangle");
}

// ---- convex O(log n) cross-checks ------------------------------------------

bool test_convex_square() {
    auto sq = make_poly({{0, 0}, {4, 0}, {4, 4}, {0, 4}});
    return convex_matches_general(sq, -2, 6, "convex square");
}

bool test_convex_triangle() {
    auto tri = make_poly({{0, 0}, {6, 0}, {0, 6}});
    return convex_matches_general(tri, -2, 8, "convex triangle");
}

bool test_convex_hexagon() {
    // A convex hexagon in counterclockwise order.
    auto hex = make_poly({{2, 0}, {5, 0}, {7, 3}, {5, 6}, {2, 6}, {0, 3}});
    return convex_matches_general(hex, -2, 9, "convex hexagon");
}

bool test_convex_pentagon() {
    auto pent = make_poly({{2, 0}, {6, 2}, {5, 6}, {1, 6}, {0, 2}});
    return convex_matches_general(pent, -2, 8, "convex pentagon");
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"square_fixed", test_square_fixed},       {"nonconvex_l", test_nonconvex_l},
        {"nonconvex_arrow", test_nonconvex_arrow}, {"triangle_grid", test_triangle_grid},
        {"convex_square", test_convex_square},     {"convex_triangle", test_convex_triangle},
        {"convex_hexagon", test_convex_hexagon},   {"convex_pentagon", test_convex_pentagon},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
