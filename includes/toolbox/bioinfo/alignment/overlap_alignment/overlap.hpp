#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace toolbox {

namespace alignment {

/**
 * @brief Overlap alignment: the best-scoring global alignment between some suffix of s1 and
 * some prefix of s2.
 * @param s1 The first string (a suffix of it forms one side of the alignment).
 * @param s2 The second string (a prefix of it forms the other side of the alignment).
 * @param dp The DP table.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @return The best overlap alignment score.
 * @note max-score alignment, for the same reason as smith_waterman_dp: a match must be a
 * genuine reward (a > 0), otherwise the trivial empty overlap (score 0) is always at least as
 * good as any real one.
 * @note Column 0 is reset to 0 for every row, since s1's own prefix can be skipped for free (s1
 * only needs to contribute a suffix). Row 0 keeps the ordinary gap penalty, since s2 must be
 * used starting from its very first character. The answer is the best score anywhere in the
 * last row, since s1's suffix must reach the end of s1 but s2's prefix may stop anywhere.
 * @note [Complexity]: O(nm) time complexity and O(nm) space complexity.
 */
int overlap_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &dp,
               int a = 1, int x = 1, int g = 1) {
    const int m = static_cast<int>(s1.size());
    const int n = static_cast<int>(s2.size());
    dp.assign(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) {
        dp[i][0] = 0;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = -j * g;
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int diag = dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : -x);
            int up = dp[i - 1][j] - g;
            int left = dp[i][j - 1] - g;
            dp[i][j] = std::max({diag, up, left});
        }
    }
    int best = dp[m][0];
    for (int j = 1; j <= n; j++) {
        best = std::max(best, dp[m][j]);
    }
    return best;
}

/**
 * @brief Overlap alignment traceback.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param dp The DP table.
 * @param s1_aligned The first aligned string.
 * @param s2_aligned The second aligned string.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @note Traces back from the best-scoring cell in the last row and stops as soon as column 0 is
 * reached, so s1_aligned ends up a suffix of s1 and s2_aligned a prefix of s2.
 * @note [Complexity]: O(n) time complexity to locate the best cell in the last row,
 * O(m + n) for the walk back.
 */
void overlap_traceback(const std::string &s1, const std::string &s2,
                       const std::vector<std::vector<int>> &dp, std::string &s1_aligned,
                       std::string &s2_aligned, int a = 1, int x = 1, int g = 1) {
    const int m = static_cast<int>(s1.size());
    const int n = static_cast<int>(s2.size());
    int j_star = 0;
    int best = dp[m][0];
    for (int j = 1; j <= n; j++) {
        if (dp[m][j] > best) {
            best = dp[m][j];
            j_star = j;
        }
    }
    int i = m;
    int j = j_star;
    s1_aligned = "";
    s2_aligned = "";
    while (j > 0) {
        if (i > 0 && dp[i][j] == dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : -x)) {
            s1_aligned = s1[i - 1] + s1_aligned;
            s2_aligned = s2[j - 1] + s2_aligned;
            i--;
            j--;
        } else if (i > 0 && dp[i][j] == dp[i - 1][j] - g) {
            s1_aligned = s1[i - 1] + s1_aligned;
            s2_aligned = "-" + s2_aligned;
            i--;
        } else {
            s1_aligned = "-" + s1_aligned;
            s2_aligned = s2[j - 1] + s2_aligned;
            j--;
        }
    }
}

/**
 * @brief Overlap alignment: the best-scoring global alignment between some suffix of s1 and
 * some prefix of s2.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param s1_aligned The first aligned string.
 * @param s2_aligned The second aligned string.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @return The best overlap alignment score.
 * @note max-score alignment.
 * @note [Complexity]: O(nm) time complexity and O(nm) space complexity.
 */
int overlap_all(const std::string &s1, const std::string &s2, std::string &s1_aligned,
                std::string &s2_aligned, int a = 1, int x = 1, int g = 1) {
    std::vector<std::vector<int>> dp;
    int score = overlap_dp(s1, s2, dp, a, x, g);
    overlap_traceback(s1, s2, dp, s1_aligned, s2_aligned, a, x, g);
    return score;
}

}  // namespace alignment

}  // namespace toolbox
