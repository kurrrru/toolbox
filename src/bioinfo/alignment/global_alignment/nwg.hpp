#ifndef NWG_HPP
#define NWG_HPP

#include <vector>
#include <string>

namespace toolbox
{

namespace alignment
{

/**
* @brief Needleman-Wunsch-Gotoh algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table for matches.
* @param I The DP table for insertions.
* @param D The DP table for deletions.
* @param a The match cost.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @return The difference between the two strings.
* @note min-cost alignment.
* @note O(nm) time complexity and O(nm) space complexity.
*/
int needleman_wunsch_gotoh_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int> > &M, std::vector<std::vector<int> > &D, std::vector<std::vector<int> > &I,
	int a = 0, int x = 1, int o = 0, int e = 1)
{
	int n = (int)s1.size(), m = (int)s2.size();
	M.assign(n + 1, std::vector<int>(m + 1, 0));
	D.assign(n + 1, std::vector<int>(m + 1, 0));
	I.assign(n + 1, std::vector<int>(m + 1, 0));

	M[0][0] = 0;
	for (int i = 1; i <= n; i++)
	{
		M[i][0] = o + e * i;
		D[i][0] = o + e * i;
		I[i][0] = (1 << 30);
	}
	for (int j = 1; j <= m; j++)
	{
		M[0][j] = o + e * j;
		D[0][j] = (1 << 30);
		I[0][j] = o + e * j;
	}
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
		{
			D[i][j] = std::min(M[i - 1][j] + o + e, D[i - 1][j] + e);
			I[i][j] = std::min(M[i][j - 1] + o + e, I[i][j - 1] + e);
			M[i][j] = std::min(M[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : x), std::min(D[i][j], I[i][j]));
		}
	return (M[n][m]);
}

/**
* @brief Needleman-Wunsch-Gotoh traceback.
* @param s1 The first string.
* @param s2 The second string.
* @param M The DP table for matches.
* @param I The DP table for insertions.
* @param D The DP table for deletions.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param a The match cost.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @note O(n + m) time complexity.
*/
void needleman_wunsch_gotoh_traceback(const std::string &s1, const std::string &s2,
	const std::vector<std::vector<int> > &M, const std::vector<std::vector<int> > &D,
	const std::vector<std::vector<int> > &I, std::string &s1_aligned, std::string &s2_aligned,
	int a = 0, int x = 1, int o = 0, int e = 1)
{
	int n = (int)s1.size(), m = (int)s2.size();
	int i, j;
	enum table { dp_M, dp_I, dp_D };

	s1_aligned = s2_aligned = "";
	i = n, j = m;
	enum table table_id = table::dp_M;
	while (i > 0 || j > 0)
	{
		if (table_id == table::dp_M)
		{
			if (i > 0 && j > 0 && M[i][j] == M[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? a : x))
			{
				s1_aligned = s1[i - 1] + s1_aligned;
				s2_aligned = s2[j - 1] + s2_aligned;
				i--, j--;
			}
			else if (M[i][j] == D[i][j])
				table_id = table::dp_D;
			else if (M[i][j] == I[i][j])
				table_id = table::dp_I;
		}
		else if (i > 0 && table_id == table::dp_D)
		{
			s1_aligned = s1[i - 1] + s1_aligned;
			s2_aligned = "-" + s2_aligned;
			i--;
			if (i > 0 && D[i][j] == M[i - 1][j] + o + e)
				table_id = table::dp_M;
		}
		else if (j > 0 && table_id == table::dp_I)
		{
			s1_aligned = "-" + s1_aligned;
			s2_aligned = s2[j - 1] + s2_aligned;
			j--;
			if (j > 0 && I[i][j] == M[i][j - 1] + o + e)
				table_id = table::dp_M;
		}
	}
}

/**
* @brief Needleman-Wunsch-Gotoh algorithm for global alignment.
* @param s1 The first string.
* @param s2 The second string.
* @param s1_aligned The first aligned string.
* @param s2_aligned The second aligned string.
* @param a The match score.
* @param x The mismatch cost.
* @param o The gap open cost.
* @param e The gap extension cost.
* @return The difference between the two strings.
* @note O(n + m) time complexity.
*/
int needleman_wunsch_gotoh_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int o = 0, int e = 1)
{
	std::vector<std::vector<int> > M, D, I;
	int diff = needleman_wunsch_gotoh_dp(s1, s2, M, D, I, a, x, o, e);
	needleman_wunsch_gotoh_traceback(s1, s2, M, D, I, s1_aligned, s2_aligned, a, x, o, e);
	return (diff);
}

} // namespace alignment

} // namespace toolbox

#endif // NWG_HPP
