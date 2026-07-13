#include <iostream>
#include <string>
#include <vector>

#include "toolbox/string/kmp.hpp"

#include "utils/test_util.hpp"

namespace {

// Brute-force failure function for verification. O(n^2), test-only reference.
std::vector<int> brute_failure(const std::string &s) {
    const int n = s.size();
    std::vector<int> lps(n, 0);
    for (int i = 1; i < n; ++i) {
        for (int len = i; len >= 1; --len) {
            if (s.compare(0, len, s, i - len + 1, len) == 0) {
                lps[i] = len;
                break;
            }
        }
    }
    return lps;
}

// Brute-force search for verification. O(n*m), test-only reference.
std::vector<int> brute_search(const std::string &text, const std::string &pattern) {
    std::vector<int> positions;
    const int n = text.size();
    const int m = pattern.size();
    if (m == 0) {
        for (int i = 0; i <= n; ++i) {
            positions.push_back(i);
        }
        return positions;
    }
    for (int i = 0; i + m <= n; ++i) {
        if (text.compare(i, m, pattern) == 0) {
            positions.push_back(i);
        }
    }
    return positions;
}

bool verify_failure(const std::string &s, const std::string &label) {
    if (toolbox::string::kmp_failure(s) != brute_failure(s)) {
        std::cerr << "  FAIL: " << label << ": failure function mismatch\n";
        return false;
    }
    return true;
}

bool verify_search(const std::string &text, const std::string &pattern, const std::string &label) {
    if (toolbox::string::kmp_search(text, pattern) != brute_search(text, pattern)) {
        std::cerr << "  FAIL: " << label << ": search result mismatch\n";
        return false;
    }
    return true;
}

bool test_failure_basic() { return verify_failure("ababaca", "ababaca"); }

bool test_failure_all_same() { return verify_failure("aaaaaa", "all same characters"); }

bool test_failure_no_repeat() { return verify_failure("abcdef", "no repeated prefix"); }

bool test_search_multiple() { return verify_search("ababab", "ab", "'ab' in 'ababab'"); }

bool test_search_overlap() { return verify_search("aaaaa", "aa", "overlapping 'aa' in 'aaaaa'"); }

bool test_search_not_found() { return verify_search("abcabc", "xyz", "pattern absent"); }

bool test_search_full_match() { return verify_search("hello", "hello", "pattern equals text"); }

bool test_search_dna() { return verify_search("ACGTACGTGACGT", "ACGT", "'ACGT' in DNA sequence"); }

bool test_search_positions() {
    const std::vector<int> pos = toolbox::string::kmp_search("ababab", "ab");
    bool ok = true;
    ok &= toolbox::test_utils::check(pos.size() == 3, "positions: 3 occurrences");
    ok &= toolbox::test_utils::check(pos == std::vector<int>({0, 2, 4}), "positions: 0,2,4");
    return ok;
}

bool test_empty_pattern() {
    const std::vector<int> pos = toolbox::string::kmp_search("abc", "");
    return toolbox::test_utils::check(pos == std::vector<int>({0, 1, 2, 3}),
                                      "empty pattern matches every position");
}

// ---- Knuth's optimized failure function ------------------------------------
// Hand-verified reference case (a standard textbook example for this optimization):
// pattern "aabaaab" has plain failure function [0,1,0,1,2,2,3] but optimized [0,1,0,0,2,1,3]
// (e.g. opt[3]=0 because s[4]=='a' equals s[lps[3]]=s[1]=='a', so falling back to lps[3]=1
// would immediately fail again, and opt[0]==0 already, so opt[3] chains to opt[0]==0).

bool test_optimized_failure_known_case() {
    const std::vector<int> opt = toolbox::string::kmp_optimized_failure("aabaaab");
    return toolbox::test_utils::check(opt == std::vector<int>({0, 1, 0, 0, 2, 1, 3}),
                                      "optimized failure function: known case 'aabaaab'");
}

bool test_optimized_failure_never_exceeds_plain() {
    bool ok = true;
    for (const std::string s : {"ababaca", "aaaaaa", "abcdef", "aabaaab", "abababab"}) {
        const std::vector<int> lps = toolbox::string::kmp_failure(s);
        const std::vector<int> opt = toolbox::string::kmp_optimized_failure(s);
        for (std::size_t i = 0; i < s.size(); ++i) {
            ok &= toolbox::test_utils::check(opt[i] <= lps[i],
                                             "optimized failure never exceeds plain: " + s);
        }
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"failure_basic", test_failure_basic},
        {"failure_all_same", test_failure_all_same},
        {"failure_no_repeat", test_failure_no_repeat},
        {"search_multiple", test_search_multiple},
        {"search_overlap", test_search_overlap},
        {"search_not_found", test_search_not_found},
        {"search_full_match", test_search_full_match},
        {"search_dna", test_search_dna},
        {"search_positions", test_search_positions},
        {"empty_pattern", test_empty_pattern},
        {"optimized_failure_known_case", test_optimized_failure_known_case},
        {"optimized_failure_never_exceeds_plain", test_optimized_failure_never_exceeds_plain},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
