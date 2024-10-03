#ifndef NW_HPP
#define NW_HPP

#include <vector>
#include <string>

namespace toolbox
{

namespace alignment
{

/**
* @brief Needleman-Wunsch algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param dp The DP table.
* @param a The match cost.
* @param x The mismatch cost.
* @param g The gap cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nm) time complexity and O(nm) space complexity.
*/
int needleman_wunsch_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int>> &dp, int a = 0, int x = 1, int g = 1)
{
	const int m = (int)s1.size();
	const int n = (int)s2.size();
	dp.assign(m + 1, std::vector<int>(n + 1, 0));
	for (int i = 0; i <= m; i++)
	{
		dp[i][0] = i * g;
	}
	for (int j = 0; j <= n; j++)
	{
		dp[0][j] = j * g;
	}
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			dp[i][j] = std::min(dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : x), std::min(dp[i - 1][j] + g, dp[i][j - 1] + g));
		}
	}
	return (dp[m][n]);
}

/**
* @brief Needleman-Wunsch traceback.
* @param s1 The first string.
* @param s2 The second string.
* @param dp The DP table.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param a The match cost.
* @param x The mismatch cost.
* @param g The gap cost.
* @note O(m + n) time complexity.
*/
void needleman_wunsch_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int> > &dp,
	std::string &s1_aligned, std::string &s2_aligned, int a = 0, int x = 1, int g = 1)
{
	const int m = (int)s1.size();
	const int n = (int)s2.size();
	int i = m;
	int j = n;
	s1_aligned = "";
	s2_aligned = "";
	while (i > 0 || j > 0)
	{
		if (i > 0 && (dp[i][j] == dp[i - 1][j] + g || j == 0))
		{
			s1_aligned = s1[i - 1] + s1_aligned;
			s2_aligned = "-" + s2_aligned;
			i--;
		}
		else if (j > 0 && (dp[i][j] == dp[i][j - 1] + g || i == 0))
		{
			s1_aligned = "-" + s1_aligned;
			s2_aligned = s2[j - 1] + s2_aligned;
			j--;
		}
		else if (i > 0 && j > 0 && dp[i][j] == dp[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : x))
		{
			s1_aligned = s1[i - 1] + s1_aligned;
			s2_aligned = s2[j - 1] + s2_aligned;
			i--;
			j--;
		}
	}
}

/**
* @brief Needleman-Wunsch algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param a The match cost.
* @param x The mismatch cost.
* @param g The gap cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nm) time complexity and O(nm) space complexity.
*/
int needleman_wunsch_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1)
{
	std::vector<std::vector<int>> dp;
	int diff = needleman_wunsch_dp(s1, s2, dp, a, x, g);
	needleman_wunsch_traceback(s1, s2, dp, s1_aligned, s2_aligned, a, x, g);
	return (diff);
}

} // namespace alignment

} // namespace toolbox

#endif // NW_HPP
