#include <random>
#include <string>
#include <vector>

#include "toolbox/string/boyer_moore.hpp"

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
        toolbox::string::boyer_moore_search(text, pattern) == brute_search(text, pattern), label);
}

bool test_search_multiple() { return verify_search("ababab", "ab", "'ab' in 'ababab'"); }

bool test_search_overlap() { return verify_search("aaaaa", "aa", "overlapping 'aa' in 'aaaaa'"); }

bool test_search_not_found() { return verify_search("abcabc", "xyz", "pattern absent"); }

bool test_search_full_match() { return verify_search("hello", "hello", "pattern equals text"); }

bool test_search_dna() { return verify_search("ACGTACGTGACGT", "ACGT", "'ACGT' in DNA sequence"); }

bool test_search_pattern_longer_than_text() {
    return verify_search("ab", "abc", "pattern longer than text");
}

bool test_search_all_same_pattern() {
    // Stresses the good suffix table: every suffix of the pattern recurs everywhere.
    return verify_search("aaaaaaaaaaaa", "aaaa", "repetitive pattern 'aaaa' in all-'a' text");
}

bool test_search_periodic_pattern() {
    // Classic stress case for the good suffix rule: a pattern with internal periodicity.
    return verify_search("abababababababab", "ababab", "periodic pattern 'ababab'");
}

bool test_search_good_suffix_no_prefix_match() {
    // The matched suffix recurs inside the pattern, but not as a prefix: exercises the
    // "suff[i] == i+1" vs. general good-suffix-shift branches separately.
    return verify_search("xxabcabcyyabc", "abcabc", "good suffix without prefix restart");
}

bool test_search_bad_character_absent() {
    // Mismatched text character never appears in the pattern at all.
    return verify_search("xxxxxpattern", "pattern", "bad character absent from pattern");
}

bool test_search_positions() {
    const std::vector<int> pos = toolbox::string::boyer_moore_search("ababab", "ab");
    bool ok = true;
    ok &= toolbox::test_utils::check(pos.size() == 3, "positions: 3 occurrences");
    ok &= toolbox::test_utils::check(pos == std::vector<int>({0, 2, 4}), "positions: 0,2,4");
    return ok;
}

bool test_empty_pattern() {
    const std::vector<int> pos = toolbox::string::boyer_moore_search("abc", "");
    return toolbox::test_utils::check(pos == std::vector<int>({0, 1, 2, 3}),
                                      "empty pattern matches every position");
}

// Randomized cross-check against brute force, over a small alphabet (to force patterns to
// recur within texts/themselves and exercise both shift rules) and many trials of varied
// text/pattern lengths, including patterns with internal repetition.
bool test_search_randomized() {
    std::mt19937 rng(98765);
    std::uniform_int_distribution<int> char_dist(0, 2);  // alphabet {'a', 'b', 'c'}
    bool ok = true;
    for (int trial = 0; trial < 300 && ok; ++trial) {
        std::uniform_int_distribution<int> len_dist(0, 40);
        const int n = len_dist(rng);
        const int m = std::uniform_int_distribution<int>(0, 10)(rng);
        std::string text(n, 'a');
        for (char &c : text) {
            c = static_cast<char>('a' + char_dist(rng));
        }
        std::string pattern(m, 'a');
        for (char &c : pattern) {
            c = static_cast<char>('a' + char_dist(rng));
        }
        ok &= toolbox::test_utils::check(
            toolbox::string::boyer_moore_search(text, pattern) == brute_search(text, pattern),
            "randomized trial " + std::to_string(trial));
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"search_multiple", test_search_multiple},
        {"search_overlap", test_search_overlap},
        {"search_not_found", test_search_not_found},
        {"search_full_match", test_search_full_match},
        {"search_dna", test_search_dna},
        {"search_pattern_longer_than_text", test_search_pattern_longer_than_text},
        {"search_all_same_pattern", test_search_all_same_pattern},
        {"search_periodic_pattern", test_search_periodic_pattern},
        {"search_good_suffix_no_prefix_match", test_search_good_suffix_no_prefix_match},
        {"search_bad_character_absent", test_search_bad_character_absent},
        {"search_positions", test_search_positions},
        {"empty_pattern", test_empty_pattern},
        {"search_randomized", test_search_randomized},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
