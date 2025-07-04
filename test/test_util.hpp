#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <functional>

namespace toolbox {
namespace color {
const std::string reset = "\033[0m";
const std::string red = "\033[31m";
const std::string green = "\033[32m";
const std::string cyan = "\033[36m";
const std::string yellow = "\033[33m";
}

namespace test_utils {
    template <typename ArgType>
    void runTests(const std::vector<std::pair<std::function<bool(ArgType)>, std::string>> &tests, const ArgType &arg) {
    int test_case = 0, success_case = 0, fail_case = 0;
    for (const auto &test : tests) {
        bool result = test.first(arg);
        if (result) {
            success_case++;
            std::cout << color::cyan << "Test passed: " << test.second << color::reset << std::endl;
        } else {
            fail_case++;
            std::cout << color::yellow << "Test failed: " << test.second << color::reset << std::endl;
        }
        test_case++;
    }
    if (fail_case == 0) {
        std::cout << color::green << "All " << success_case << " tests passed!" << color::reset << std::endl;
    } else {
        std::cout << color::red << fail_case << " out of " << test_case << " tests failed." << color::reset << std::endl;
    }
}
}
}
