#include <algorithm>
#include <string>
#include <vector>

#include "toolbox/geometry/line_intersection.hpp"
#include "toolbox/geometry/point.hpp"

#include "utils/test_util.hpp"

namespace {

typedef toolbox::geometry::Point<long long> P;

// ---- independent reference -------------------------------------------------
// A parametric (rational) reference, structurally different from the four-orientation
// straddle test in the implementation, so the two are unlikely to share a blind spot.

long long cross2(const P &u, const P &v) { return u.x * v.y - u.y * v.x; }

// Whether point p lies on segment [a, b], checked via collinearity + bounding box (a different
// formulation than the implementation's dot-product test).
bool on_seg_ref(const P &p, const P &a, const P &b) {
    if (cross2(P(b.x - a.x, b.y - a.y), P(p.x - a.x, p.y - a.y)) != 0) {
        return false;
    }
    return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) && std::min(a.y, b.y) <= p.y &&
           p.y <= std::max(a.y, b.y);
}

bool ref_intersect(const P &a, const P &b, const P &c, const P &d) {
    const P ab(b.x - a.x, b.y - a.y);
    const P cd(d.x - c.x, d.y - c.y);
    const P ca(c.x - a.x, c.y - a.y);
    long long denom = cross2(ab, cd);
    if (denom != 0) {
        long long s_num = cross2(ca, cd);  // a + s*ab meets c + t*cd
        long long t_num = cross2(ca, ab);
        if (denom < 0) {
            denom = -denom;
            s_num = -s_num;
            t_num = -t_num;
        }
        return 0 <= s_num && s_num <= denom && 0 <= t_num && t_num <= denom;
    }
    // Parallel: intersect only if collinear and the (degenerate-safe) segments overlap.
    if (cross2(ab, ca) != 0) {
        return false;
    }
    return on_seg_ref(a, c, d) || on_seg_ref(b, c, d) || on_seg_ref(c, a, b) || on_seg_ref(d, a, b);
}

// ---- fixed cases -----------------------------------------------------------

bool test_proper_cross() {
    return toolbox::test_utils::check(
        toolbox::geometry::segments_intersect(P(0, 0), P(4, 4), P(0, 4), P(4, 0)),
        "proper X crossing");
}

bool test_no_cross() {
    return toolbox::test_utils::check(
        !toolbox::geometry::segments_intersect(P(0, 0), P(1, 1), P(2, 2), P(3, 3)),
        "collinear but disjoint");
}

bool test_touch_endpoint() {
    return toolbox::test_utils::check(
        toolbox::geometry::segments_intersect(P(0, 0), P(2, 2), P(2, 2), P(4, 0)),
        "touching at a shared endpoint");
}

bool test_t_junction() {
    // Endpoint of one segment lies in the interior of the other.
    return toolbox::test_utils::check(
        toolbox::geometry::segments_intersect(P(0, 0), P(4, 0), P(2, 0), P(2, 5)),
        "T-junction (endpoint on interior)");
}

bool test_collinear_overlap() {
    return toolbox::test_utils::check(
        toolbox::geometry::segments_intersect(P(0, 0), P(4, 0), P(2, 0), P(6, 0)),
        "collinear overlapping");
}

bool test_parallel_disjoint() {
    return toolbox::test_utils::check(
        !toolbox::geometry::segments_intersect(P(0, 0), P(4, 0), P(0, 1), P(4, 1)),
        "parallel non-collinear");
}

bool test_degenerate_point_on_segment() {
    // A degenerate segment (a point) lying on another segment.
    return toolbox::test_utils::check(
        toolbox::geometry::segments_intersect(P(2, 2), P(2, 2), P(0, 0), P(4, 4)),
        "point on segment");
}

bool test_degenerate_point_off_segment() {
    return toolbox::test_utils::check(
        !toolbox::geometry::segments_intersect(P(2, 3), P(2, 3), P(0, 0), P(4, 4)),
        "point off segment");
}

bool test_on_segment_helper() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::geometry::on_segment(P(2, 2), P(0, 0), P(4, 4)),
                                     "on_segment: midpoint");
    ok &= toolbox::test_utils::check(toolbox::geometry::on_segment(P(0, 0), P(0, 0), P(4, 4)),
                                     "on_segment: endpoint");
    ok &= toolbox::test_utils::check(!toolbox::geometry::on_segment(P(5, 5), P(0, 0), P(4, 4)),
                                     "on_segment: beyond endpoint");
    ok &= toolbox::test_utils::check(!toolbox::geometry::on_segment(P(2, 3), P(0, 0), P(4, 4)),
                                     "on_segment: off the line");
    return ok;
}

// ---- exhaustive small-grid cross-check -------------------------------------

bool test_exhaustive_grid() {
    // Enumerate every pair of segments with endpoints on a small integer grid (including
    // degenerate point-segments) and compare against the independent reference.
    const int lo = 0;
    const int hi = 3;  // 4x4 grid -> 16 points -> 256 segments -> 65536 pairs
    std::vector<P> grid;
    for (int x = lo; x <= hi; ++x) {
        for (int y = lo; y <= hi; ++y) {
            grid.push_back(P(x, y));
        }
    }
    bool ok = true;
    for (const P &a : grid) {
        for (const P &b : grid) {
            for (const P &c : grid) {
                for (const P &d : grid) {
                    const bool got = toolbox::geometry::segments_intersect(a, b, c, d);
                    const bool exp = ref_intersect(a, b, c, d);
                    if (got != exp) {
                        ok = false;
                        break;
                    }
                }
                if (!ok) {
                    break;
                }
            }
            if (!ok) {
                break;
            }
        }
        if (!ok) {
            break;
        }
    }
    return toolbox::test_utils::check(ok, "exhaustive 4x4 grid matches reference");
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"proper_cross", test_proper_cross},
        {"no_cross", test_no_cross},
        {"touch_endpoint", test_touch_endpoint},
        {"t_junction", test_t_junction},
        {"collinear_overlap", test_collinear_overlap},
        {"parallel_disjoint", test_parallel_disjoint},
        {"degenerate_point_on_segment", test_degenerate_point_on_segment},
        {"degenerate_point_off_segment", test_degenerate_point_off_segment},
        {"on_segment_helper", test_on_segment_helper},
        {"exhaustive_grid", test_exhaustive_grid},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
