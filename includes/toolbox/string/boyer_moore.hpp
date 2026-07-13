#pragma once

#include <algorithm>
#include <array>
#include <string>
#include <vector>

namespace toolbox {

namespace string {

namespace helper {

/**
 * @brief For each position i, the length of the longest suffix of pattern[0..i] that is also
 * a suffix of the whole pattern.
 * @param pattern The pattern to preprocess.
 * @return A vector common_suffix where common_suffix[i] is that length (0 <= common_suffix[i]
 *         <= i + 1), with common_suffix[m-1] == m by definition (the whole pattern is trivially
 *         a suffix of itself).
 * @note This is the standard preprocessing step used to build the Boyer-Moore strong good
 *       suffix table (see good_suffix_shift), following the classic construction from
 *       Charras & Lecroq, "Handbook of Exact String Matching Algorithms" (the notoriously
 *       tricky part of Boyer-Moore; variable names here follow that reference closely to
 *       minimize the risk of subtly re-deriving it incorrectly).
 * @note [Attention]: This function is not intended to be used directly.
 * @note [Complexity]: O(|pattern|) time, O(|pattern|) space.
 */
std::vector<int> longest_common_suffix_lengths(const std::string &pattern) {
    const int m = static_cast<int>(pattern.size());
    std::vector<int> common_suffix(m, 0);
    common_suffix[m - 1] = m;
    int g = m - 1;  // right edge of the rightmost window known to match a suffix of pattern
    int f = 0;      // the window's start, i.e. pattern[g+1..f+m-1-f] mirrors pattern's suffix
    for (int i = m - 2; i >= 0; --i) {
        if (i > g && common_suffix[i + m - 1 - f] < i - g) {
            common_suffix[i] = common_suffix[i + m - 1 - f];
        } else {
            if (i < g) {
                g = i;
            }
            f = i;
            while (g >= 0 && pattern[g] == pattern[g + m - 1 - f]) {
                --g;
            }
            common_suffix[i] = f - g;
        }
    }
    return common_suffix;
}

/**
 * @brief Builds the strong good suffix shift table for Boyer-Moore.
 * @param pattern The pattern to preprocess.
 * @return A vector shift of length |pattern| where shift[i] is how far to slide the pattern
 *         when a mismatch occurs at pattern position i (i.e. pattern[i+1..m-1] matched the
 *         text but pattern[i] did not), so that the matched suffix realigns with another
 *         occurrence of itself in the pattern (or a matching prefix of the pattern), without
 *         skipping over any possible match.
 * @note [Attention]: This function is not intended to be used directly.
 * @note [Complexity]: O(|pattern|) time, O(|pattern|) space.
 */
std::vector<int> good_suffix_shift(const std::string &pattern) {
    const int m = static_cast<int>(pattern.size());
    const std::vector<int> common_suffix = longest_common_suffix_lengths(pattern);
    std::vector<int> shift(m, m);
    int j = 0;
    for (int i = m - 1; i >= 0; --i) {
        if (common_suffix[i] == i + 1) {
            for (; j < m - 1 - i; ++j) {
                if (shift[j] == m) {
                    shift[j] = m - 1 - i;
                }
            }
        }
    }
    for (int i = 0; i <= m - 2; ++i) {
        shift[m - 1 - common_suffix[i]] = m - 1 - i;
    }
    return shift;
}

/**
 * @brief Builds the bad character table for Boyer-Moore.
 * @param pattern The pattern to preprocess.
 * @return An array last mapping each possible byte value c to the last (rightmost) index in
 *         pattern[0..m-2] where c occurs, or -1 if c does not occur there. The final character
 *         of the pattern itself is deliberately excluded, since aligning a mismatch to it
 *         could never produce a rightward shift.
 * @note [Attention]: This function is not intended to be used directly.
 * @note [Complexity]: O(|pattern| + alphabet size) time and space.
 */
std::array<int, 256> bad_character_last_occurrence(const std::string &pattern) {
    const int m = static_cast<int>(pattern.size());
    std::array<int, 256> last;
    last.fill(-1);
    for (int i = 0; i < m - 1; ++i) {
        last[static_cast<unsigned char>(pattern[i])] = i;
    }
    return last;
}

}  // namespace helper

/**
 * @brief Finds all occurrences of a pattern in a text using the Boyer-Moore algorithm.
 * @param text The text to search in.
 * @param pattern The pattern to search for.
 * @return 0-based start indices where pattern occurs in text (ascending order).
 * @note Combines the bad character rule and the strong good suffix rule, taking the larger of
 *       the two shifts on each mismatch (with a hard floor of 1 to guarantee forward
 *       progress, defensively, even though the good suffix shift is always >= 1 by
 *       construction).
 * @note [Complexity]: O(|text| * |pattern|) worst case, sublinear in practice (does not
 *       examine every character of text); O(|pattern| + alphabet size) preprocessing space.
 *       An empty pattern is treated as matching at every position 0..|text|.
 */
std::vector<int> boyer_moore_search(const std::string &text, const std::string &pattern) {
    std::vector<int> positions;
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());
    if (m == 0) {
        positions.reserve(n + 1);
        for (int i = 0; i <= n; ++i) {
            positions.push_back(i);
        }
        return positions;
    }
    if (m > n) {
        return positions;
    }
    const std::vector<int> good_suffix = helper::good_suffix_shift(pattern);
    const std::array<int, 256> last_occurrence = helper::bad_character_last_occurrence(pattern);
    int s = 0;  // current alignment of pattern's start within text
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j]) {
            --j;
        }
        if (j < 0) {
            positions.push_back(s);
            s += good_suffix[0];
        } else {
            const int bad_char_shift = j - last_occurrence[static_cast<unsigned char>(text[s + j])];
            s += std::max({1, good_suffix[j], bad_char_shift});
        }
    }
    return positions;
}

}  // namespace string

}  // namespace toolbox
