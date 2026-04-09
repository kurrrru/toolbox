#include "toolbox/datastructure/sparse_table/sparse_table.hpp"
#include "test/test_util.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

int op_min(const int a, const int b) { return std::min(a, b); }
int op_max(const int a, const int b) { return std::max(a, b); }

// Brute-force RMQ for verification
int brute_min(const std::vector<int> &v, int l, int r) {
    int res = std::numeric_limits<int>::max();
    for (int i = l; i < r; i++) res = std::min(res, v[i]);
    return res;
}
int brute_max(const std::vector<int> &v, int l, int r) {
    int res = std::numeric_limits<int>::min();
    for (int i = l; i < r; i++) res = std::max(res, v[i]);
    return res;
}

bool test_rmq_min() {
    std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    toolbox::datastructure::sparse_table<int, op_min> st(arr);
    bool ok = true;
    // exhaustive range check
    int n = arr.size();
    for (int l = 0; l < n; l++) {
        for (int r = l + 1; r <= n; r++) {
            int expected = brute_min(arr, l, r);
            int got      = st.staticRMQ(l, r);
            if (got != expected) {
                std::cerr << "  FAIL: RMQ_min[" << l << "," << r << ") expected "
                          << expected << " got " << got << "\n";
                ok = false;
            }
        }
    }
    return ok;
}

bool test_rmq_max() {
    std::vector<int> arr = {5, 3, 8, 1, 7, 2, 9, 4, 6, 0};
    toolbox::datastructure::sparse_table<int, op_max> st(arr);
    bool ok = true;
    int n = arr.size();
    for (int l = 0; l < n; l++) {
        for (int r = l + 1; r <= n; r++) {
            int expected = brute_max(arr, l, r);
            int got      = st.staticRMQ(l, r);
            if (got != expected) {
                std::cerr << "  FAIL: RMQ_max[" << l << "," << r << ") expected "
                          << expected << " got " << got << "\n";
                ok = false;
            }
        }
    }
    return ok;
}

bool test_single_element() {
    std::vector<int> arr = {42};
    toolbox::datastructure::sparse_table<int, op_min> st(arr);
    return check(st.staticRMQ(0, 1) == 42, "single element RMQ == 42");
}

bool test_all_same() {
    std::vector<int> arr(8, 7);
    toolbox::datastructure::sparse_table<int, op_min> st(arr);
    bool ok = true;
    ok &= check(st.staticRMQ(0, 8) == 7, "all-same full range");
    ok &= check(st.staticRMQ(2, 5) == 7, "all-same sub range");
    return ok;
}

bool test_sorted_array() {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8};
    toolbox::datastructure::sparse_table<int, op_min> st(arr);
    bool ok = true;
    ok &= check(st.staticRMQ(0, 8) == 1, "sorted: min full == 1");
    ok &= check(st.staticRMQ(4, 8) == 5, "sorted: min [4,8) == 5");
    ok &= check(st.staticRMQ(3, 5) == 4, "sorted: min [3,5) == 4");
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"rmq_min",        test_rmq_min},
        {"rmq_max",        test_rmq_max},
        {"single_element", test_single_element},
        {"all_same",       test_all_same},
        {"sorted_array",   test_sorted_array},
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
