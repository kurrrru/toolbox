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
 * @brief Finds all occurrences of a pattern in a text using the KMP algorithm.
 * @param text The text to search in.
 * @param pattern The pattern to search for.
 * @return 0-based start indices where pattern occurs in text (ascending order).
 * @note [Complexity]: O(|text| + |pattern|) time, O(|pattern|) extra space.
 *       An empty pattern is treated as matching at every position 0..|text|.
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
