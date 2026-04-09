#include "toolbox/datastructure/unionfind/unionfind.hpp"
#include "test/test_util.hpp"

#include <iostream>
#include <string>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

bool test_basic() {
    toolbox::datastructure::unionfind uf(5);
    bool ok = true;
    // Initially all separate
    ok &= check(!uf.same(0, 1), "initially not same(0,1)");
    ok &= check(!uf.same(2, 4), "initially not same(2,4)");

    // Unite 0 and 1
    ok &= check(uf.unite(0, 1) == true, "unite(0,1) returns true");
    ok &= check(uf.same(0, 1),          "same(0,1) after unite");
    ok &= check(!uf.same(0, 2),         "not same(0,2)");

    // Unite 1 and 2
    uf.unite(1, 2);
    ok &= check(uf.same(0, 2), "same(0,2) after unite(1,2)");

    // Unite already same
    ok &= check(uf.unite(0, 2) == false, "unite already-same returns false");
    return ok;
}

bool test_multiple_components() {
    toolbox::datastructure::unionfind uf(6);
    uf.unite(0, 1);
    uf.unite(2, 3);
    uf.unite(4, 5);
    bool ok = true;
    ok &= check( uf.same(0, 1), "same(0,1)");
    ok &= check( uf.same(2, 3), "same(2,3)");
    ok &= check( uf.same(4, 5), "same(4,5)");
    ok &= check(!uf.same(0, 2), "not same(0,2)");
    ok &= check(!uf.same(0, 4), "not same(0,4)");
    ok &= check(!uf.same(2, 4), "not same(2,4)");
    return ok;
}

bool test_chain_union() {
    // 0-1-2-3-4 all connected via chain
    toolbox::datastructure::unionfind uf(5);
    for (int i = 0; i < 4; i++) uf.unite(i, i + 1);
    bool ok = true;
    for (int i = 0; i < 5; i++)
        for (int j = i + 1; j < 5; j++)
            ok &= check(uf.same(i, j), "chain: all same");
    return ok;
}

bool test_self_union() {
    toolbox::datastructure::unionfind uf(3);
    bool ok = true;
    ok &= check(uf.unite(1, 1) == false, "unite(1,1) returns false");
    ok &= check(uf.same(1, 1),           "same(1,1) is always true");
    return ok;
}

bool test_find_consistency() {
    // After many operations, find should return a stable representative
    toolbox::datastructure::unionfind uf(10);
    for (int i = 0; i < 9; i++) uf.unite(i, i + 1);
    int rep = uf.find(0);
    bool ok = true;
    for (int i = 1; i < 10; i++)
        ok &= check(uf.find(i) == rep, "all find() return same representative");
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"basic",              test_basic},
        {"multiple_components",test_multiple_components},
        {"chain_union",        test_chain_union},
        {"self_union",         test_self_union},
        {"find_consistency",   test_find_consistency},
    };
    const std::size_t num = sizeof(tests) / sizeof(tests[0]);
    int pass = 0, fail = 0;
    for (std::size_t i = 0; i < num; i++) {
        bool ok = tests[i].fn();
        if (ok) { std::cout << toolbox::color::cyan  << "PASS " << tests[i].name << toolbox::color::reset << "\n"; ++pass; }
        else    { std::cout << toolbox::color::yellow << "FAIL " << tests[i].name << toolbox::color::reset << "\n"; ++fail; }
    }
    std::cout << "\n";
    if (fail == 0)
        std::cout << toolbox::color::green << "All " << pass << " tests passed!" << toolbox::color::reset << "\n";
    else
        std::cout << toolbox::color::red << fail << " out of " << (pass + fail) << " tests failed." << toolbox::color::reset << "\n";
    return fail == 0 ? 0 : 1;
}
