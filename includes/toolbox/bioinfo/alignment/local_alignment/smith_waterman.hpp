#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace toolbox {

namespace alignment {

/**
 * @brief Smith-Waterman algorithm for local alignment.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param dp The DP table.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @return The best local alignment score.
 * @note max-score alignment. Unlike the global alignment algorithms in this namespace, which
 * minimize an edit cost, Smith-Waterman maximizes a similarity score: a match must be a genuine
 * reward (a > 0) so that extending an alignment is only worthwhile when it pays off, and each
 * cell is floored at 0 so an unfavorable prefix can always be abandoned in favor of starting a
 * new local alignment.
 * @note [Complexity]: O(nm) time complexity and O(nm) space complexity.
 */
int smith_waterman_dp(const std::string &s1, const std::string &s2,
                      std::vector<std::vector<int>> &dp, int a = 1, int x = 1, int g = 1) {
    const int m = static_cast<int>(s1.size());
    const int n = static_cast<int>(s2.size());
    dp.assign(m + 1, std::vector<int>(n + 1, 0));
    int best = 0;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int diag = dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : -x);
            int up = dp[i - 1][j] - g;
            int left = dp[i][j - 1] - g;
            dp[i][j] = std::max({0, diag, up, left});
            best = std::max(best, dp[i][j]);
        }
    }
    return best;
}

/**
 * @brief Smith-Waterman traceback.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param dp The DP table.
 * @param s1_aligned The first aligned string.
 * @param s2_aligned The second aligned string.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @note Traces back from the highest-scoring cell and stops as soon as a cell with score 0 is
 * reached, so s1_aligned / s2_aligned only cover the aligned substrings, not the full inputs.
 * @note [Complexity]: O(nm) time complexity to locate the best cell, O(m + n) for the walk back.
 */
void smith_waterman_traceback(const std::string &s1, const std::string &s2,
                              const std::vector<std::vector<int>> &dp, std::string &s1_aligned,
                              std::string &s2_aligned, int a = 1, int x = 1, int g = 1) {
    const int m = static_cast<int>(s1.size());
    const int n = static_cast<int>(s2.size());
    int bi = 0;
    int bj = 0;
    int best = 0;
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (dp[i][j] > best) {
                best = dp[i][j];
                bi = i;
                bj = j;
            }
        }
    }
    int i = bi;
    int j = bj;
    s1_aligned = "";
    s2_aligned = "";
    while (i > 0 && j > 0 && dp[i][j] > 0) {
        if (dp[i][j] == dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : -x)) {
            s1_aligned = s1[i - 1] + s1_aligned;
            s2_aligned = s2[j - 1] + s2_aligned;
            i--;
            j--;
        } else if (dp[i][j] == dp[i - 1][j] - g) {
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
 * @brief Smith-Waterman algorithm for local alignment.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param s1_aligned The first aligned string.
 * @param s2_aligned The second aligned string.
 * @param a The match score.
 * @param x The mismatch penalty.
 * @param g The gap penalty.
 * @return The best local alignment score.
 * @note max-score alignment.
 * @note [Complexity]: O(nm) time complexity and O(nm) space complexity.
 */
int smith_waterman_all(const std::string &s1, const std::string &s2, std::string &s1_aligned,
                       std::string &s2_aligned, int a = 1, int x = 1, int g = 1) {
    std::vector<std::vector<int>> dp;
    int score = smith_waterman_dp(s1, s2, dp, a, x, g);
    smith_waterman_traceback(s1, s2, dp, s1_aligned, s2_aligned, a, x, g);
    return score;
}

}  // namespace alignment

}  // namespace toolbox
