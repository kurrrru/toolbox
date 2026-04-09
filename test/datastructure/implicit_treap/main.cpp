#include "toolbox/datastructure/implicit_treap/implicit_treap.hpp"
#include "test/test_util.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

using namespace toolbox::datastructure;
using namespace monoid_implicit_treap;

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// ---- range_add / range_min -------------------------------------------------

bool test_range_add_range_min() {
    // arr = [3, 1, 4, 1, 5]
    std::vector<long long> v = {3, 1, 4, 1, 5};
    implicit_treap<long long, monoid_range_add_range_min> t(v);
    bool ok = true;
    ok &= check(t.query(0, 5) == 1, "initial min [0,5)==1");
    ok &= check(t.query(2, 4) == 1, "initial min [2,4)==1");

    // add 10 to [1,3) => arr = [3, 11, 14, 1, 5]
    t.update(1, 3, 10);
    ok &= check(t.query(0, 5) == 1,  "after update min [0,5)==1");
    ok &= check(t.query(1, 3) == 11, "after update min [1,3)==11");

    // add -5 to [3,5) => arr = [3, 11, 14, -4, 0]
    t.update(3, 5, -5);
    ok &= check(t.query(0, 5) == -4, "after update min [0,5)==-4");
    return ok;
}

// ---- range_add / range_max -------------------------------------------------

bool test_range_add_range_max() {
    std::vector<long long> v = {1, 3, 2, 5, 4};
    implicit_treap<long long, monoid_range_add_range_max> t(v);
    bool ok = true;
    ok &= check(t.query(0, 5) == 5, "initial max [0,5)==5");
    ok &= check(t.query(0, 3) == 3, "initial max [0,3)==3");

    t.update(0, 3, 10); // arr = [11, 13, 12, 5, 4]
    ok &= check(t.query(0, 5) == 13, "after update max [0,5)==13");
    ok &= check(t.query(3, 5) == 5,  "after update max [3,5)==5");
    return ok;
}

// ---- range_add / range_sum -------------------------------------------------

bool test_range_add_range_sum() {
    std::vector<long long> v = {1, 2, 3, 4, 5};
    implicit_treap<long long, monoid_range_add_range_sum> t(v);
    bool ok = true;
    ok &= check(t.query(0, 5) == 15, "initial sum [0,5)==15");
    ok &= check(t.query(1, 4) == 9,  "initial sum [1,4)==9");

    t.update(0, 5, 1); // add 1 to all: [2,3,4,5,6]
    ok &= check(t.query(0, 5) == 20, "after +1 all, sum==20");
    ok &= check(t.query(2, 4) == 9,  "after +1 all, sum [2,4)==9");
    return ok;
}

// ---- operator[] ------------------------------------------------------------

bool test_access() {
    std::vector<long long> v = {10, 20, 30, 40, 50};
    implicit_treap<long long, monoid_range_add_range_sum> t(v);
    bool ok = true;
    ok &= check(t[0] == 10, "t[0]==10");
    ok &= check(t[2] == 30, "t[2]==30");
    ok &= check(t[4] == 50, "t[4]==50");
    return ok;
}

// ---- insert / erase / size -------------------------------------------------

bool test_insert_erase() {
    implicit_treap<long long, monoid_range_add_range_sum> t;
    bool ok = true;
    ok &= check(t.size() == 0, "initial size==0");

    t.insert(0, 100);
    t.insert(1, 200);
    t.insert(2, 300);
    ok &= check(t.size() == 3,         "size==3 after insert");
    ok &= check(t.query(0, 3) == 600,  "sum after insert==600");

    // insert at position 1: [100, 999, 200, 300]
    t.insert(1, 999);
    ok &= check(t.size() == 4,         "size==4 after insert");
    ok &= check(t[1] == 999,           "t[1]==999 after insert");
    ok &= check(t[2] == 200,           "t[2]==200 after insert");

    // erase position 1: [100, 200, 300]
    t.erase(1);
    ok &= check(t.size() == 3,         "size==3 after erase");
    ok &= check(t[1] == 200,           "t[1]==200 after erase");
    ok &= check(t.query(0, 3) == 600,  "sum restored after erase");
    return ok;
}

// ---- reverse ---------------------------------------------------------------

bool test_reverse() {
    std::vector<long long> v = {1, 2, 3, 4, 5};
    implicit_treap<long long, monoid_range_add_range_sum> t(v);
    t.reverse(1, 4); // reverse [1,4): [1, 4, 3, 2, 5]
    bool ok = true;
    ok &= check(t[0] == 1, "reverse: t[0]==1");
    ok &= check(t[1] == 4, "reverse: t[1]==4");
    ok &= check(t[2] == 3, "reverse: t[2]==3");
    ok &= check(t[3] == 2, "reverse: t[3]==2");
    ok &= check(t[4] == 5, "reverse: t[4]==5");
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"range_add_range_min", test_range_add_range_min},
        {"range_add_range_max", test_range_add_range_max},
        {"range_add_range_sum", test_range_add_range_sum},
        {"access",              test_access},
        {"insert_erase",        test_insert_erase},
        {"reverse",             test_reverse},
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
