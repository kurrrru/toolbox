#pragma once

#include <string>
#include <vector>

namespace toolbox {

namespace string {

/**
 * @brief Computes the KMP failure function (a.k.a. prefix function / LPS array).
 * @param s The string for which to compute the failure function.
 * @return A vector lps where lps[i] is the length of the longest proper prefix
 *         of s[0..i] that is also a suffix of s[0..i].
 * @note [Complexity]: O(|s|) time, O(|s|) space.
 *       LPS[i] = 0 for i = 0, and for i > 0, LPS[i] = max { k < i | s[0..k-1] == s[i-k+1..i] }.
 */
std::vector<int> kmp_failure(const std::string &s) {
    const int n = s.size();
    std::vector<int> lps(n, 0);
    for (int i = 1; i < n; ++i) {
        int j = lps[i - 1];
        while (j > 0 && s[i] != s[j]) {
            j = lps[j - 1];
        }
        if (s[i] == s[j]) {
            ++j;
        }
        lps[i] = j;
    }
    return lps;
}

/**
 * @brief Computes Knuth's optimized failure function.
 * @param s The pattern for which to compute the optimized failure function.
 * @return A vector opt that can be used as a search-time fallback exactly like
 *         kmp_failure(s), except that whenever s[i+1] == s[lps[i]] (the character right
 *         after the matched prefix equals the character a naive fallback would need to
 *         match next, so that fallback is guaranteed to fail too), the value is chained
 *         further to opt[lps[i]-1] instead of stopping at lps[i].
 * @note [Complexity]: O(|s|) time, O(|s|) space.
 * @note This is the refinement that distinguishes Knuth-Morris-Pratt from the plain
 *       Morris-Pratt algorithm (see morris_pratt.hpp): it avoids re-testing a text
 *       character against a pattern character already known to mismatch.
 */
std::vector<int> kmp_optimized_failure(const std::string &s) {
    const int m = s.size();
    const std::vector<int> lps = kmp_failure(s);
    std::vector<int> opt(m, 0);
    for (int i = 0; i < m; ++i) {
        if (i + 1 < m && s[i + 1] == s[lps[i]]) {
            opt[i] = (lps[i] == 0) ? 0 : opt[lps[i] - 1];
        } else {
            opt[i] = lps[i];
        }
    }
    return opt;
}

/**
 * @brief Finds all occurrences of a pattern in a text using the Knuth-Morris-Pratt algorithm.
 * @param text The text to search in.
 * @param pattern The pattern to search for.
 * @return 0-based start indices where pattern occurs in text (ascending order).
 * @note [Complexity]: O(|text| + |pattern|) time, O(|pattern|) extra space.
 *       An empty pattern is treated as matching at every position 0..|text|.
 * @note Falls back on a mismatch using kmp_optimized_failure (Knuth's refinement), not the
 *       plain kmp_failure array; see morris_pratt.hpp for the unoptimized variant.
 */
std::vector<int> kmp_search(const std::string &text, const std::string &pattern) {
    std::vector<int> positions;
    const int n = text.size();
    const int m = pattern.size();
    if (m == 0) {
        positions.reserve(n + 1);
        for (int i = 0; i <= n; ++i) {
            positions.push_back(i);
        }
        return positions;
    }
    const std::vector<int> next = kmp_optimized_failure(pattern);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            positions.push_back(i - m + 1);
            j = next[j - 1];
        }
    }
    return positions;
}

}  // namespace string

}  // namespace toolbox
