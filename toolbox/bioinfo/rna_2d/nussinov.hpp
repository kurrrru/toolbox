#ifndef NUSSINOV_HPP
#define NUSSINOV_HPP

#include <iostream>
#include <vector>
#include <string>
#include <stack>

namespace toolbox
{

namespace rna_2d
{

bool is_base_pair(char a, char b)
{
	if (a > b)
		std::swap(a, b);
	return ((a == 'A' && b == 'U') || (a == 'C' && b == 'G') || (a == 'G' && b == 'U'));
}

/**
* @brief Nussinov algorithm for RNA secondary structure prediction.
* @param seq The RNA sequence.
* @param dp The DP table.
* @return The maximum number of base pairs.
* @note O(n^3) time complexity and O(n^2) space complexity.
*/
int nussinov_dp(const std::string &seq, std::vector<std::vector<int>> &dp)
{
	int n = seq.size();
	dp.resize(n, std::vector<int>(n, 0));
	for (int k = 1; k < n; k++)
	{
		for (int i = 0; i < n - k; i++)
		{
			int j = i + k;
			dp[i][j] = dp[i + 1][j - 1] + is_base_pair(seq[i], seq[j]);
			for (int l = i; l < j; l++)
				dp[i][j] = std::max(dp[i][j], dp[i][l] + dp[l + 1][j]);
		}
	}
	return dp[0][n - 1];
}

/**
* @brief Nussinov traceback.
* @param seq The RNA sequence.
* @param dp The DP table.
* @return The RNA secondary structure.
* @note O(n^2) time complexity.
*/
std::string nussinov_traceback(const std::string &seq, const std::vector<std::vector<int>> &dp)
{
	std::string structure;
	int n = seq.size();
	structure.resize(n, '.');
	std::stack<std::pair<int, int>> st;
	st.push({0, n - 1});
	while (!st.empty())
	{
		int left = st.top().first;
		int right = st.top().second;
		st.pop();
		if (left >= right)
			continue;
		if (dp[left][right] == dp[left + 1][right - 1] + is_base_pair(seq[left], seq[right]))
		{
			if (is_base_pair(seq[left], seq[right]))
			{
				structure[left] = '(';
				structure[right] = ')';
			}
			st.push({left + 1, right - 1});
		}
		else
		{
			for (int mid = left; mid < right; mid++)
			{
				if (dp[left][right] == dp[left][mid] + dp[mid + 1][right])
				{
					st.push({mid + 1, right});
					st.push({left, mid});
					break;
				}
			}
		}
	}
	return structure;
}

/**
* @brief Nussinov algorithm for RNA secondary structure prediction.
* @param seq The RNA sequence.
* @return The RNA secondary structure.
* @note O(n^3) time complexity and O(n^2) space complexity.
*/
std::string nussinov_all(const std::string &seq)
{
	std::vector<std::vector<int>> dp;
	nussinov_dp(seq, dp);
	return nussinov_traceback(seq, dp);
}

} // namespace rna_2d

} // namespace toolbox

#endif
