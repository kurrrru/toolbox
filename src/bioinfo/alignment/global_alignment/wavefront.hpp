#ifndef WAVEFRONT_HPP
#define WAVEFRONT_HPP

#include <vector>
#include <string>

namespace toolbox
{

namespace alignment
{

/**
* @brief Wavefront algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table for matches.
* @param I The DP table for insertions.
* @param D The DP table for deletions.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nd) time complexity and O(d^2) space complexity.
*/
int wavefront_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int> > &M, std::vector<std::vector<int> > &I, std::vector<std::vector<int> > &D,
	int x = 1, int o = 0, int e = 1)
{
	int n = (int)s1.size(), m = (int)s2.size();
	int N = std::max(n, m);
	int s, k;
	M.clear();
	I.clear();
	D.clear();
	M.push_back(std::vector<int>(1, 0));
	I.push_back(std::vector<int>(1, -1));
	D.push_back(std::vector<int>(1, -1));
	while (M[0][0] < n && M[0][0] < m && s1[M[0][0]] == s2[M[0][0]])
		M[0][0]++;
	for (s = 1; s <= N; s++)
	{
		M.push_back(std::vector<int>(2 * s + 1, -1));
		I.push_back(std::vector<int>(2 * s + 1, -1));
		D.push_back(std::vector<int>(2 * s + 1, -1));
		for (k = - s; k < s + 1; k++)
		{
			if (s - o - e >= 0 && k - 1 >= -(s - o - e) && k - 1 <= s - o - e && M[s - o - e][(s - o - e) + (k - 1)] >= 0)
				I[s][s + k] = std::max(I[s][s + k], M[s - o - e][(s - o - e) + (k - 1)] + 1);
			if (s - e >= 0 && k - 1 >= -(s - e) && k - 1 <= s - e && I[s - e][(s - e) + (k - 1)] >= 0)
				I[s][s + k] = std::max(I[s][s + k], I[s - e][(s - e) + (k - 1)] + 1);
			if (s - o - e >= 0 && k + 1 >= -(s - o - e) && k + 1 <= s - o - e && M[s - o - e][(s - o - e) + (k + 1)] >= 0)
				D[s][s + k] = std::max(D[s][s + k], M[s - o - e][(s - o - e) + (k + 1)]);
			if (s - e >= 0 && k + 1 >= -(s - e) && k + 1 <= s - e && D[s - e][(s - e) + (k + 1)] >= 0)
				D[s][s + k] = std::max(D[s][s + k], D[s - e][(s - e) + (k + 1)]);
			if (s - x >= 0 && k >= -(s - x) && k <= (s - x) && M[s - x][(s - x) + k] >= 0)
				M[s][s + k] = std::max(M[s][s + k], M[s - x][(s - x) + k] + 1);
			M[s][s + k] = std::max(M[s][s + k], I[s][s + k]);
			M[s][s + k] = std::max(M[s][s + k], D[s][s + k]);
			if (M[s][s + k] >= 0)
				while (M[s][s + k] < n && M[s][s + k] - k < m && s1[M[s][s + k]] == s2[M[s][s + k] - k])
					M[s][s + k]++;
			if (M[s][s + k] == n && M[s][s + k] - k == m)
				return (M.size() - 1);
		}
	}
	return (M.size() - 1);
}

/**
* @brief Wavefront traceback.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table for matches.
* @param I The DP table for insertions.
* @param D The DP table for deletions.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @note O(n) time complexity.
*/
void wavefront_traceback(const std::string &s1, const std::string &s2,
	const std::vector<std::vector<int> > &M, const std::vector<std::vector<int> > &I, const std::vector<std::vector<int> > &D,
	std::string &s1_aligned, std::string &s2_aligned, int x = 1, int o = 0, int e = 1)
{
	int n = (int)s1.size(), m = (int)s2.size();
	int i, j, s, k, i_prev, j_prev;
	enum table { dp_M, dp_I, dp_D };

	s = M.size() - 1;
	k = n - m;
	enum table table_id = table::dp_M;
	std::vector<std::pair<int, int>> path;
	path.push_back(std::make_pair(M[s][s + k], M[s][s + k] - k));
	while (s > 0)
	{
		if (table_id == table::dp_M)
		{
			if (s - x >= 0 && k >= -(s - x) && k <= (s - x) && M[s - x][(s - x) + k] + 1 > I[s][s + k] && M[s - x][(s - x) + k] + 1 > D[s][s + k])
			{
				s = s - x;
				k = k;
				path.push_back(std::make_pair(M[s][s + k], M[s][s + k] - k));
				table_id = table::dp_M;
			}
			else if (I[s][s + k] > D[s][s + k])
			{
				table_id = table::dp_I;
			}
			else
			{
				table_id = table::dp_D;
			}
		}
		else if (table_id == table::dp_I)
		{
			if (s - o - e >= 0 && k - 1 >= -(s - o - e) && k - 1 <= s - o - e && I[s][s + k] == M[s - o - e][(s - o - e) + (k - 1)] + 1)
			{
				s = s - o - e;
				k = k - 1;
				path.push_back(std::make_pair(M[s][s + k], M[s][s + k] - k));
				table_id = table::dp_M;
			}
			else if (s - e >= 0 && k - 1 >= -(s - e) && k - 1 <= s - e && I[s][s + k] == I[s - e][(s - e) + (k - 1)] + 1)
			{
				s = s - e;
				k = k - 1;
				path.push_back(std::make_pair(I[s][s + k], I[s][s + k] - k));
				table_id = table::dp_I;
			}
		}
		else if (table_id == table::dp_D)
		{
			if (s - o - e >= 0 && k + 1 >= -(s - o - e) && k + 1 <= s - o - e && D[s][s + k] == M[s - o - e][(s - o - e) + (k + 1)])
			{
				s = s - o - e;
				k = k + 1;
				path.push_back(std::make_pair(M[s][s + k], M[s][s + k] - k));
				table_id = table::dp_M;
			}
			else if (s - e >= 0 && k + 1 >= -(s - e) && k + 1 <= s - e && D[s][s + k] == D[s - e][(s - e) + (k + 1)])
			{
				s = s - e;
				k = k + 1;
				path.push_back(std::make_pair(D[s][s + k], D[s][s + k] - k));
				table_id = table::dp_D;
			}
		}
	}
	path.push_back(std::make_pair(0, 0));
	s1_aligned = s2_aligned = "";
	for (s = path.size() - 1; s >= 0; s--)
	{
		i = path[s].first;
		j = path[s].second;
		i_prev = (s == (int)path.size() - 1) ? 0 : path[s + 1].first;
		j_prev = (s == (int)path.size() - 1) ? 0 : path[s + 1].second;
		if (j - i == j_prev - i_prev - 1)
			s2_aligned +="-";
		else if (j - i == j_prev - i_prev + 1)
			s1_aligned += "-";
		s1_aligned += s1.substr(i_prev, i - i_prev);
		s2_aligned += s2.substr(j_prev, j - j_prev);
	}
}

/**
* @brief Wavefront algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nd) time complexity and O(d^2) space complexity.
*/
int wavefront_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int x = 1, int o = 0, int e = 1)
{
	std::vector<std::vector<int> > M, I, D;
	int diff = wavefront_dp(s1, s2, M, I, D, x, o, e);
	wavefront_traceback(s1, s2, M, I, D, s1_aligned, s2_aligned, x, o, e);
	return (diff);
}

} // namespace alignment

} // namespace toolbox

#endif // WAVEFRONT_HPP
