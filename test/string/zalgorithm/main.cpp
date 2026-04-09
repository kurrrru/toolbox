#include "toolbox/string/zalgorithm.hpp"
#include "test/test_util.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// Verify Z-array by brute force
std::vector<int> brute_z(const std::string &s) {
    int n = s.size();
    std::vector<int> z(n, 0);
    z[0] = n;
    for (int i = 1; i < n; i++) {
        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
            z[i]++;
    }
    return z;
}

bool verify(const std::string &s, const std::string &label) {
    auto got      = toolbox::string::zalgorithm(s);
    auto expected = brute_z(s);
    if (got != expected) {
        std::cerr << "  FAIL: " << label << ": got vs expected mismatch\n";
        return false;
    }
    return true;
}

bool test_basic() {
    return verify("aabxaa", "aabxaa");
}

bool test_all_same() {
    return verify("aaaaaaa", "all same characters");
}

bool test_no_repeat() {
    return verify("abcdef", "no repeated prefix");
}

bool test_periodic() {
    return verify("abababab", "periodic pattern");
}

bool test_single_char() {
    bool ok = true;
    auto z = toolbox::string::zalgorithm("a");
    ok &= check(z.size() == 1, "single char: size==1");
    ok &= check(z[0] == 1,     "single char: z[0]==1");
    return ok;
}

bool test_two_chars_same() {
    bool ok = true;
    auto z = toolbox::string::zalgorithm("aa");
    ok &= check(z[0] == 2, "aa: z[0]==2");
    ok &= check(z[1] == 1, "aa: z[1]==1");
    return ok;
}

bool test_two_chars_diff() {
    bool ok = true;
    auto z = toolbox::string::zalgorithm("ab");
    ok &= check(z[0] == 2, "ab: z[0]==2");
    ok &= check(z[1] == 0, "ab: z[1]==0");
    return ok;
}

bool test_pattern_search() {
    // Pattern search: find positions of "ab" in "ababab"
    // Concatenate: "ab$ababab" and look for z[i] >= |pattern|
    std::string pat = "ab";
    std::string text = "ababab";
    std::string s = pat + "$" + text;
    auto z = toolbox::string::zalgorithm(s);
    int plen = pat.size();
    int prefix = plen + 1;
    std::vector<int> positions;
    for (int i = prefix; i < (int)s.size(); i++)
        if (z[i] >= plen)
            positions.push_back(i - prefix);
    bool ok = true;
    ok &= check(positions.size() == 3,
                "pattern search: 3 occurrences of 'ab' in 'ababab'");
    ok &= check(positions[0] == 0 && positions[1] == 2 && positions[2] == 4,
                "pattern search: positions are 0,2,4");
    return ok;
}

bool test_longer_string() {
    return verify("mississippi", "mississippi");
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"basic",          test_basic},
        {"all_same",       test_all_same},
        {"no_repeat",      test_no_repeat},
        {"periodic",       test_periodic},
        {"single_char",    test_single_char},
        {"two_chars_same", test_two_chars_same},
        {"two_chars_diff", test_two_chars_diff},
        {"pattern_search", test_pattern_search},
        {"longer_string",  test_longer_string},
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
