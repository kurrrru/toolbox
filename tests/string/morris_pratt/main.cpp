#include <string>
#include <vector>

#include "toolbox/string/morris_pratt.hpp"

#include "utils/test_util.hpp"

namespace {

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

bool verify_search(const std::string &text, const std::string &pattern, const std::string &label) {
    return toolbox::test_utils::check(
        toolbox::string::morris_pratt_search(text, pattern) == brute_search(text, pattern), label);
}

bool test_search_multiple() { return verify_search("ababab", "ab", "'ab' in 'ababab'"); }

bool test_search_overlap() { return verify_search("aaaaa", "aa", "overlapping 'aa' in 'aaaaa'"); }

bool test_search_not_found() { return verify_search("abcabc", "xyz", "pattern absent"); }

bool test_search_full_match() { return verify_search("hello", "hello", "pattern equals text"); }

bool test_search_dna() { return verify_search("ACGTACGTGACGT", "ACGT", "'ACGT' in DNA sequence"); }

bool test_search_repetitive_pattern() {
    // A pattern with internal repetition ("aabaaab") is exactly where Morris-Pratt and
    // Knuth-Morris-Pratt differ internally (see kmp.hpp), but both must return the same
    // match positions.
    return verify_search("aabaaabaabaaab", "aabaaab", "repetitive pattern 'aabaaab'");
}

bool test_search_positions() {
    const std::vector<int> pos = toolbox::string::morris_pratt_search("ababab", "ab");
    bool ok = true;
    ok &= toolbox::test_utils::check(pos.size() == 3, "positions: 3 occurrences");
    ok &= toolbox::test_utils::check(pos == std::vector<int>({0, 2, 4}), "positions: 0,2,4");
    return ok;
}

bool test_empty_pattern() {
    const std::vector<int> pos = toolbox::string::morris_pratt_search("abc", "");
    return toolbox::test_utils::check(pos == std::vector<int>({0, 1, 2, 3}),
                                      "empty pattern matches every position");
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"search_multiple", test_search_multiple},
        {"search_overlap", test_search_overlap},
        {"search_not_found", test_search_not_found},
        {"search_full_match", test_search_full_match},
        {"search_dna", test_search_dna},
        {"search_repetitive_pattern", test_search_repetitive_pattern},
        {"search_positions", test_search_positions},
        {"empty_pattern", test_empty_pattern},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
