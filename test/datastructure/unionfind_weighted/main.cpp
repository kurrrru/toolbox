#include "toolbox/datastructure/unionfind_weighted/unionfind_weighted.hpp"
#include "test/test_util.hpp"

#include <iostream>
#include <string>

namespace {

// weight(y) - weight(x) == w  means  unite(x, y, w)
// i.e. diff(x, y) == w

bool test_basic_weight() {
    // unite(0, 1, 3) means weight[1] - weight[0] = 3
    toolbox::datastructure::unionfind_weighted uf(3);
    bool ok = true;
    ok &= toolbox::test_utils::check(uf.unite(0, 1, 3) == true, "unite(0,1,3) returns true");
    ok &= toolbox::test_utils::check(uf.same(0, 1),             "same(0,1) after unite");
    ok &= toolbox::test_utils::check(uf.diff(0, 1) == 3,        "diff(0,1)==3");
    ok &= toolbox::test_utils::check(uf.diff(1, 0) == -3,       "diff(1,0)==-3");
    return ok;
}

bool test_transitivity() {
    // unite(0,1,3): w[1]-w[0]=3
    // unite(1,2,5): w[2]-w[1]=5
    // => diff(0,2) should be 8
    toolbox::datastructure::unionfind_weighted uf(3);
    uf.unite(0, 1, 3);
    uf.unite(1, 2, 5);
    bool ok = true;
    ok &= toolbox::test_utils::check(uf.same(0, 2),       "same(0,2) via transitivity");
    ok &= toolbox::test_utils::check(uf.diff(0, 2) == 8,  "diff(0,2)==8 (3+5)");
    ok &= toolbox::test_utils::check(uf.diff(2, 0) == -8, "diff(2,0)==-8");
    return ok;
}

bool test_already_united() {
    toolbox::datastructure::unionfind_weighted uf(2);
    uf.unite(0, 1, 4);
    bool ok = true;
    ok &= toolbox::test_utils::check(uf.unite(0, 1, 4) == false, "re-unite same pair returns false");
    ok &= toolbox::test_utils::check(uf.diff(0, 1) == 4,         "diff unchanged after re-unite");
    return ok;
}

bool test_different_components() {
    // diff across different components returns inf
    toolbox::datastructure::unionfind_weighted uf(4);
    uf.unite(0, 1, 2);
    uf.unite(2, 3, 7);
    bool ok = true;
    ok &= toolbox::test_utils::check(!uf.same(0, 2), "different components");
    const int inf = (int)(1e9);
    ok &= toolbox::test_utils::check(uf.diff(0, 2) == inf, "diff across components == inf");
    return ok;
}

bool test_zero_weight() {
    toolbox::datastructure::unionfind_weighted uf(2);
    uf.unite(0, 1, 0);
    bool ok = true;
    ok &= toolbox::test_utils::check(uf.same(0, 1),     "same after unite with 0 weight");
    ok &= toolbox::test_utils::check(uf.diff(0, 1) == 0,"diff==0 when united with weight 0");
    return ok;
}

} // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"basic_weight",          test_basic_weight},
        {"transitivity",          test_transitivity},
        {"already_united",        test_already_united},
        {"different_components",  test_different_components},
        {"zero_weight",           test_zero_weight},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
