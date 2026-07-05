#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace toolbox {
namespace color {
const std::string reset = "\033[0m";
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string cyan = "\033[36m";
const std::string yellow = "\033[33m";
}  // namespace color

namespace test_utils {

// ---- Assertion helper -------------------------------------------------------

inline bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// ---- Test case and runner ---------------------------------------------------

struct Test {
    std::string name;
    bool (*fn)();
};

inline int run_tests(const Test *tests, std::size_t num) {
    int pass = 0, fail = 0;
    for (std::size_t i = 0; i < num; i++) {
        bool ok = tests[i].fn();
        if (ok) {
            std::cout << color::cyan << "PASS " << tests[i].name << color::reset << "\n";
            ++pass;
        } else {
            std::cout << color::yellow << "FAIL " << tests[i].name << color::reset << "\n";
            ++fail;
        }
    }
    std::cout << "\n";
    if (fail == 0)
        std::cout << color::green << "All " << pass << " tests passed!" << color::reset << "\n";
    else
        std::cout << color::red << fail << " out of " << (pass + fail) << " tests failed."
                  << color::reset << "\n";
    return fail == 0 ? 0 : 1;
}

// ---- Legacy: parameterized test runner (wavelet_tree, fm_index) -------------

template <typename ArgType>
void runTests(const std::vector<std::pair<std::function<bool(ArgType)>, std::string>> &tests,
              const ArgType &arg) {
    int test_case = 0, success_case = 0, fail_case = 0;
    for (const auto &test : tests) {
        bool result = test.first(arg);
        if (result) {
            success_case++;
            std::cout << color::cyan << "Test passed: " << test.second << color::reset << std::endl;
        } else {
            fail_case++;
            std::cout << color::yellow << "Test failed: " << test.second << color::reset
                      << std::endl;
        }
        test_case++;
    }
    if (fail_case == 0) {
        std::cout << color::green << "All " << success_case << " tests passed!" << color::reset
                  << std::endl;
    } else {
        std::cout << color::red << fail_case << " out of " << test_case << " tests failed."
                  << color::reset << std::endl;
    }
}

}  // namespace test_utils
}  // namespace toolbox
