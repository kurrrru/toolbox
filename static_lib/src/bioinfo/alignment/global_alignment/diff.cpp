#include "../../../../toolbox.h"
#include <vector>
#include <string>

namespace toolbox
{

namespace alignment
{

/**
* @brief Diff algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table.
* @param x The mismatch cost.
* @param g The gap cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nd) time complexity and O(d^2) space complexity.
*/
int diff_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &M, int x = 1, int g = 1)
{
	const int m = (int)s1.size();
	const int n = (int)s2.size();
	int N = std::max(m, n);
	int d;
	int k;

	M.clear();
	M.push_back(std::vector<int>(1, 0));
	while (M[0][0] < m && M[0][0] < n && s1[M[0][0]] == s2[M[0][0]])
	{
		M[0][0]++;
	}
	for (d = 1; d <= N; d++)
	{
		M.push_back(std::vector<int>(2 * d + 1, -1));
		for (k = - d; k < d + 1; k++)
		{
			if (d - x >= 0 && k >= -(d - x) && k <= (d - x) && M[d - x][(d - x) + k] >= 0)
			{
				M[d][d + k] = std::max(M[d][d + k], M[d - x][(d - x) + k] + 1);
			}
			if (d - g >= 0 && k - 1 >= -(d - g) && M[d - g][(d - g) + (k - 1)] >= 0)
			{
				M[d][d + k] = std::max(M[d][d + k], M[d - g][(d - g) + (k - 1)] + 1);
			}
			if (d - g >= 0 && k + 1 <= d - g && M[d - g][(d - g) + (k + 1)] >= 0)
			{
				M[d][d + k] = std::max(M[d][d + k], M[d - g][(d - g) + (k + 1)]);
			}
			if (M[d][d + k] >= 0)
			{
				while (M[d][d + k] < m && M[d][d + k] - k < n && s1[M[d][d + k]] == s2[M[d][d + k] - k])
				{
					M[d][d + k]++;
				}
			}
			if (M[d][d + k] == m && M[d][d + k] - k == n)
			{
				return (M.size() - 1);
			}
		}
	}
	return (M.size() - 1);
}

/**
* @brief Diff traceback.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param x The mismatch cost.
* @param g The gap cost.
* @note O(m) time complexity.
*/
void diff_traceback(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &M,
	std::string &s1_aligned, std::string &s2_aligned, int x = 1, int g = 1)
{
	const int m = (int)s1.size();
	const int n = (int)s2.size();
	int d;
	int k;
	int now_max;
	int src;
	std::vector<std::pair<int, int>> path;
	d = M.size() - 1;
	k = m - n;
	path.push_back(std::make_pair(M[d][d + k], M[d][d + k] - k));
	while (d > 0)
	{
		now_max = -1;
		src = -1;
		if (d - x >= 0 && k >= -(d - x) && k <= (d - x) && M[d - x][(d - x) + k] + 1 >= now_max)
		{
			now_max = M[d - x][(d - x) + k] + 1;
			src = 0;
		}
		if (d - g >= 0 && k - 1 >= -(d - g) && M[d - g][(d - g) + (k - 1)] + 1 >= now_max)
		{
			now_max = M[d - g][(d - g) + (k - 1)] + 1;
			src = 1;
		}
		if (d - g >= 0 && k + 1 <= d - g && M[d - g][(d - g) + (k + 1)] >= now_max)
		{
			now_max = M[d - g][(d - g) + (k + 1)];
			src = 2;
		}
		if (src == 0)
		{
			d = d - x;
			k = k;
		}
		else if (src == 1)
		{
			d = d - g;
			k = k - 1;
		}
		else if (src == 2)
		{
			d = d - g;
			k = k + 1;
		}
		path.push_back(std::make_pair(M[d][d + k], M[d][d + k] - k));
	}
	path.push_back(std::make_pair(0, 0));

	int i;
	int j;
	int i_prev;
	int j_prev;
	s1_aligned = "";
	s2_aligned = "";
	for (d = path.size() - 1; d >= 0; d--)
	{
		i = path[d].first;
		j = path[d].second;
		i_prev = (d == (int)path.size() - 1) ? 0 : path[d + 1].first;
		j_prev = (d == (int)path.size() - 1) ? 0 : path[d + 1].second;
		if (j - i == j_prev - i_prev - 1)
		{
			s2_aligned += "-";
		}
		else if (j - i == j_prev - i_prev + 1)
		{
			s1_aligned += "-";
		}
		s1_aligned += s1.substr(i_prev, i - i_prev);
		s2_aligned += s2.substr(j_prev, j - j_prev);
	}
}

/**
* @brief Diff algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param x The mismatch cost.
* @param g The gap cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nd) time complexity and O(d^2) space complexity.
*/
int diff_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int x = 1, int g = 1)
{
	std::vector<std::vector<int> > M;
	int diff = diff_dp(s1, s2, M, x, g);
	diff_traceback(s1, s2, M, s1_aligned, s2_aligned, x, g);
	return (diff);
}

} // namespace alignment

} // namespace toolbox
