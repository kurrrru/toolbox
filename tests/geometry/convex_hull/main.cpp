#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "toolbox/geometry/convex_hull/graham_scan.hpp"
#include "toolbox/geometry/convex_hull/jarvis_march.hpp"
#include "toolbox/geometry/convex_hull/monotone_chain.hpp"
#include "toolbox/geometry/convex_hull/quickhull.hpp"
#include "toolbox/geometry/point.hpp"

#include "utils/test_util.hpp"

namespace {

typedef toolbox::geometry::Point<long long> P;
using toolbox::geometry::cross;
using toolbox::geometry::dot;

// Whether p lies between a and b on their common line: (a - p) . (b - p) <= 0.
bool between(const P &p, const P &a, const P &b) { return dot(p, a, b) <= 0; }

// Locates p relative to a CCW convex polygon H (size >= 3): 1 inside, 0 on boundary, -1 out.
int locate_in_convex(const std::vector<P> &h, const P &p) {
    const int n = static_cast<int>(h.size());
    bool on = false;
    for (int i = 0; i < n; ++i) {
        const long long c = cross(h[i], h[(i + 1) % n], p);
        if (c < 0) {
            return -1;
        }
        if (c == 0 && between(p, h[i], h[(i + 1) % n])) {
            on = true;
        }
    }
    return on ? 0 : 1;
}

// Distinct input coordinates.
std::vector<P> distinct_points(const std::vector<P> &points) {
    std::vector<P> d = points;
    std::sort(d.begin(), d.end());
    d.erase(std::unique(d.begin(), d.end(), [](const P &a, const P &b) { return a == b; }),
            d.end());
    return d;
}

// Independently verifies that `hull` (indices into `points`) is a valid convex hull.
bool is_valid_hull(const std::vector<P> &points, const std::vector<int> &hull,
                   bool include_collinear) {
    const int n = static_cast<int>(points.size());
    // Indices in range and distinct.
    std::vector<char> seen(n, 0);
    for (const int i : hull) {
        if (i < 0 || i >= n || seen[i]) {
            return false;
        }
        seen[i] = 1;
    }
    std::vector<P> h;
    for (const int i : hull) {
        h.push_back(points[i]);
    }
    // Hull vertices must be pairwise distinct coordinates.
    for (std::size_t i = 0; i < h.size(); ++i) {
        for (std::size_t j = i + 1; j < h.size(); ++j) {
            if (h[i] == h[j]) {
                return false;
            }
        }
    }
    const std::vector<P> d = distinct_points(points);
    if (d.empty()) {
        return hull.empty();
    }
    if (d.size() == 1) {
        return h.size() == 1 && h[0] == d[0];
    }
    // Determine whether all distinct points are collinear.
    bool all_collinear = true;
    for (std::size_t i = 2; i < d.size(); ++i) {
        if (cross(d[0], d[1], d[i]) != 0) {
            all_collinear = false;
            break;
        }
    }
    if (all_collinear) {
        // d is sorted, hence ordered along the line; endpoints are d.front()/d.back().
        if (include_collinear) {
            return h.size() == d.size() &&
                   std::equal(h.begin(), h.end(), d.begin(),
                              [](const P &a, const P &b) { return a == b; });
        }
        return h.size() == 2 && h[0] == d.front() && h[1] == d.back();
    }
    // Proper 2D hull: at least 3 vertices.
    if (h.size() < 3) {
        return false;
    }
    // Counterclockwise and convex.
    long long twice_area = 0;
    const int hn = static_cast<int>(h.size());
    for (int i = 0; i < hn; ++i) {
        twice_area += cross(h[i], h[(i + 1) % hn]);
        const long long turn = cross(h[i], h[(i + 1) % hn], h[(i + 2) % hn]);
        if (turn < 0) {
            return false;  // reflex/clockwise turn -> not convex CCW
        }
        if (turn == 0 && !include_collinear) {
            return false;  // collinear vertex not allowed when excluding
        }
    }
    if (twice_area <= 0) {
        return false;  // not counterclockwise (or degenerate)
    }
    // Every input point must be inside or on the hull.
    for (const P &p : points) {
        if (locate_in_convex(h, p) < 0) {
            return false;
        }
    }
    // Every hull vertex must be an input point (guaranteed by index validity).
    // For include_collinear, every distinct input point on the boundary must be a vertex.
    if (include_collinear) {
        for (const P &p : d) {
            if (locate_in_convex(h, p) == 0) {
                bool present = false;
                for (const P &v : h) {
                    if (v == p) {
                        present = true;
                        break;
                    }
                }
                if (!present) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Rotates a hull so it starts at its smallest index, for order-insensitive comparison.
std::vector<int> canonical(std::vector<int> hull) {
    if (hull.empty()) {
        return hull;
    }
    const auto it = std::min_element(hull.begin(), hull.end());
    std::rotate(hull.begin(), it, hull.end());
    return hull;
}

std::vector<P> make_points(const std::vector<std::pair<long long, long long>> &coords) {
    std::vector<P> pts;
    for (const auto &[x, y] : coords) {
        pts.push_back(P(x, y));
    }
    return pts;
}

// Runs all four algorithms and checks each is valid and they all agree (up to rotation).
bool all_methods_agree(const std::vector<P> &points, bool include_collinear,
                       const std::string &label) {
    const auto mc = toolbox::geometry::convex_hull_monotone_chain(points, include_collinear);
    const auto gs = toolbox::geometry::convex_hull_graham_scan(points, include_collinear);
    const auto jm = toolbox::geometry::convex_hull_jarvis_march(points, include_collinear);
    const auto qh = toolbox::geometry::convex_hull_quickhull(points, include_collinear);
    bool ok = true;
    ok &= toolbox::test_utils::check(is_valid_hull(points, mc, include_collinear),
                                     label + ": monotone_chain valid");
    ok &= toolbox::test_utils::check(is_valid_hull(points, gs, include_collinear),
                                     label + ": graham_scan valid");
    ok &= toolbox::test_utils::check(is_valid_hull(points, jm, include_collinear),
                                     label + ": jarvis_march valid");
    ok &= toolbox::test_utils::check(is_valid_hull(points, qh, include_collinear),
                                     label + ": quickhull valid");
    // Compare by resulting coordinate sequences (indices may differ for coincident points).
    const auto coords = [&points](const std::vector<int> &hull) {
        std::vector<P> c;
        for (const int i : canonical(hull)) {
            c.push_back(points[i]);
        }
        return c;
    };
    const auto eq = [](const std::vector<P> &a, const std::vector<P> &b) {
        return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(),
                                                  [](const P &x, const P &y) { return x == y; });
    };
    ok &= toolbox::test_utils::check(eq(coords(mc), coords(gs)), label + ": monotone == graham");
    ok &= toolbox::test_utils::check(eq(coords(mc), coords(jm)), label + ": monotone == jarvis");
    ok &= toolbox::test_utils::check(eq(coords(mc), coords(qh)), label + ": monotone == quickhull");
    return ok;
}

// ---- fixed cases -----------------------------------------------------------

bool test_square() {
    // 4 corners + center; hull is the 4 corners.
    auto pts = make_points({{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}});
    return all_methods_agree(pts, false, "square+center");
}

bool test_triangle() {
    auto pts = make_points({{0, 0}, {5, 0}, {0, 5}});
    return all_methods_agree(pts, false, "triangle");
}

bool test_collinear_edge_exclude() {
    // Square with an extra point on the bottom edge; excluded from the strict hull.
    auto pts = make_points({{0, 0}, {2, 0}, {4, 0}, {4, 4}, {0, 4}});
    bool ok = all_methods_agree(pts, false, "collinear edge exclude");
    const auto mc = toolbox::geometry::convex_hull_monotone_chain(pts, false);
    ok &= toolbox::test_utils::check(mc.size() == 4, "collinear exclude: 4 vertices");
    return ok;
}

bool test_collinear_edge_include() {
    // Same square; the midpoint of the bottom edge is kept when including collinear points.
    auto pts = make_points({{0, 0}, {2, 0}, {4, 0}, {4, 4}, {0, 4}});
    bool ok = all_methods_agree(pts, true, "collinear edge include");
    const auto mc = toolbox::geometry::convex_hull_monotone_chain(pts, true);
    ok &= toolbox::test_utils::check(mc.size() == 5, "collinear include: 5 boundary points");
    return ok;
}

bool test_all_collinear() {
    auto pts = make_points({{0, 0}, {1, 1}, {2, 2}, {3, 3}});
    bool ok = all_methods_agree(pts, false, "all collinear exclude");
    ok &= all_methods_agree(pts, true, "all collinear include");
    const auto ex = toolbox::geometry::convex_hull_monotone_chain(pts, false);
    ok &= toolbox::test_utils::check(ex.size() == 2, "all collinear exclude: 2 endpoints");
    const auto in = toolbox::geometry::convex_hull_monotone_chain(pts, true);
    ok &= toolbox::test_utils::check(in.size() == 4, "all collinear include: all 4");
    return ok;
}

bool test_duplicates() {
    auto pts = make_points({{0, 0}, {0, 0}, {4, 0}, {4, 4}, {0, 4}, {4, 4}});
    return all_methods_agree(pts, false, "with duplicates");
}

bool test_few_points() {
    bool ok = true;
    ok &= all_methods_agree(make_points({}), false, "empty");
    ok &= all_methods_agree(make_points({{1, 1}}), false, "single point");
    ok &= all_methods_agree(make_points({{1, 1}, {3, 5}}), false, "two points");
    return ok;
}

// ---- randomized ------------------------------------------------------------

bool test_randomized() {
    std::mt19937 rng(20240714);
    bool ok = true;
    for (int trial = 0; trial < 400 && ok; ++trial) {
        const int n = std::uniform_int_distribution<int>(0, 40)(rng);
        // Small coordinate range so collinear points and duplicates arise frequently.
        std::uniform_int_distribution<int> coord(-6, 6);
        std::vector<P> pts;
        for (int i = 0; i < n; ++i) {
            // Draw x and y in separate statements: the order of evaluation of function
            // arguments is unspecified, so P(coord(rng), coord(rng)) would differ by compiler.
            const long long x = coord(rng);
            const long long y = coord(rng);
            pts.push_back(P(x, y));
        }
        const bool inc = (trial % 2 == 0);
        ok &= all_methods_agree(pts, inc, "randomized trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"square", test_square},
        {"triangle", test_triangle},
        {"collinear_edge_exclude", test_collinear_edge_exclude},
        {"collinear_edge_include", test_collinear_edge_include},
        {"all_collinear", test_all_collinear},
        {"duplicates", test_duplicates},
        {"few_points", test_few_points},
        {"randomized", test_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
