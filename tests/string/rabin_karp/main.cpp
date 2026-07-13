#include <cstdint>
#include <random>
#include <string>
#include <vector>

#include "toolbox/string/rabin_karp.hpp"

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
        toolbox::string::rabin_karp_search(text, pattern) == brute_search(text, pattern), label);
}

// ---- RollingHash ------------------------------------------------------------

bool test_hash_equal_for_equal_substrings() {
    const std::string s = "abcabcabc";
    toolbox::string::hash::RollingHash rh(s);
    bool ok = true;
    ok &= toolbox::test_utils::check(rh.get(0, 3) == rh.get(3, 6), "hash('abc') == hash('abc')");
    ok &= toolbox::test_utils::check(rh.get(0, 3) == rh.get(6, 9), "hash('abc') == hash('abc') #2");
    ok &= toolbox::test_utils::check(rh.get(0, 3) != rh.get(1, 4), "hash('abc') != hash('bca')");
    return ok;
}

bool test_hash_whole_string() {
    const std::string s = "hello";
    toolbox::string::hash::RollingHash rh(s);
    return toolbox::test_utils::check(rh.get(0, rh.size()) == rh.get(0, 5),
                                      "hash of whole string is self-consistent");
}

bool test_hash_single_char_distinguishes() {
    toolbox::string::hash::RollingHash rh("ab");
    return toolbox::test_utils::check(rh.get(0, 1) != rh.get(1, 2), "hash('a') != hash('b')");
}

bool test_hash_empty_substring() {
    toolbox::string::hash::RollingHash rh("abc");
    return toolbox::test_utils::check(rh.get(1, 1) == rh.get(2, 2),
                                      "hash of any empty substring is the same (0 contribution)");
}

// ---- Rabin-Karp search -------------------------------------------------------

bool test_search_multiple() { return verify_search("ababab", "ab", "'ab' in 'ababab'"); }

bool test_search_overlap() { return verify_search("aaaaa", "aa", "overlapping 'aa' in 'aaaaa'"); }

bool test_search_not_found() { return verify_search("abcabc", "xyz", "pattern absent"); }

bool test_search_full_match() { return verify_search("hello", "hello", "pattern equals text"); }

bool test_search_dna() { return verify_search("ACGTACGTGACGT", "ACGT", "'ACGT' in DNA sequence"); }

bool test_search_pattern_longer_than_text() {
    return verify_search("ab", "abc", "pattern longer than text");
}

bool test_search_positions() {
    const std::vector<int> pos = toolbox::string::rabin_karp_search("ababab", "ab");
    bool ok = true;
    ok &= toolbox::test_utils::check(pos.size() == 3, "positions: 3 occurrences");
    ok &= toolbox::test_utils::check(pos == std::vector<int>({0, 2, 4}), "positions: 0,2,4");
    return ok;
}

bool test_empty_pattern() {
    const std::vector<int> pos = toolbox::string::rabin_karp_search("abc", "");
    return toolbox::test_utils::check(pos == std::vector<int>({0, 1, 2, 3}),
                                      "empty pattern matches every position");
}

// Randomized cross-check against brute force, over a small alphabet (to force hash
// collisions to actually occur and exercise the verification step) and many trials.
bool test_search_randomized() {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> char_dist(0, 2);  // alphabet {'a', 'b', 'c'}
    bool ok = true;
    for (int trial = 0; trial < 200 && ok; ++trial) {
        std::uniform_int_distribution<int> len_dist(0, 30);
        const int n = len_dist(rng);
        const int m = std::uniform_int_distribution<int>(0, 8)(rng);
        std::string text(n, 'a');
        for (char &c : text) {
            c = static_cast<char>('a' + char_dist(rng));
        }
        std::string pattern(m, 'a');
        for (char &c : pattern) {
            c = static_cast<char>('a' + char_dist(rng));
        }
        ok &= toolbox::test_utils::check(
            toolbox::string::rabin_karp_search(text, pattern) == brute_search(text, pattern),
            "randomized trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"hash_equal_for_equal_substrings", test_hash_equal_for_equal_substrings},
        {"hash_whole_string", test_hash_whole_string},
        {"hash_single_char_distinguishes", test_hash_single_char_distinguishes},
        {"hash_empty_substring", test_hash_empty_substring},
        {"search_multiple", test_search_multiple},
        {"search_overlap", test_search_overlap},
        {"search_not_found", test_search_not_found},
        {"search_full_match", test_search_full_match},
        {"search_dna", test_search_dna},
        {"search_pattern_longer_than_text", test_search_pattern_longer_than_text},
        {"search_positions", test_search_positions},
        {"empty_pattern", test_empty_pattern},
        {"search_randomized", test_search_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
