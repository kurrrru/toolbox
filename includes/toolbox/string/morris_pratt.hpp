#pragma once

#include <string>
#include <vector>

#include "toolbox/string/kmp.hpp"

namespace toolbox {

namespace string {

/**
 * @brief Finds all occurrences of a pattern in a text using the Morris-Pratt algorithm.
 * @param text The text to search in.
 * @param pattern The pattern to search for.
 * @return 0-based start indices where pattern occurs in text (ascending order).
 * @note [Complexity]: O(|text| + |pattern|) time, O(|pattern|) extra space. Same asymptotic
 *       bound as Knuth-Morris-Pratt (kmp_search), but with a larger constant: on a mismatch it
 *       falls back using the plain prefix function (kmp_failure) instead of Knuth's optimized
 *       failure function (kmp_optimized_failure), so it can re-test a text character against a
 *       pattern character already known to fail.
 *       An empty pattern is treated as matching at every position 0..|text|.
 */
std::vector<int> morris_pratt_search(const std::string &text, const std::string &pattern) {
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
    const std::vector<int> lps = kmp_failure(pattern);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) {
            j = lps[j - 1];
        }
        if (text[i] == pattern[j]) {
            ++j;
        }
        if (j == m) {
            positions.push_back(i - m + 1);
            j = lps[j - 1];
        }
    }
    return positions;
}

}  // namespace string

}  // namespace toolbox
