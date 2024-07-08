#ifndef SUFFIXARRAY_HPP
#define SUFFIXARRAY_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

namespace toolbox
{

namespace string
{

namespace helper
{

std::vector<int> sais(const std::vector<int> &s, const int max_s);

/**
* @brief Computes the suffix array of a given string using the doubling algorithm.
* @param s The string for which to compute the suffix array.
* @return A suffix array for the given string.
* @note [Complexity]: O(n log n)
*/
std::vector<int> suffixarray_doubling(const std::vector<int> &s)
{
	int n = s.size();
	std::vector<int> SA(n), ISA(n), nextISA(n);

	std::iota(SA.begin(), SA.end(), 0);
	std::sort(SA.begin(), SA.end(), [&](int i, int j) {return s[i] < s[j];});
	ISA[SA[0]] = 0;
	for (int i = 1; i < n; i++)
		ISA[SA[i]] = ISA[SA[i - 1]] + (s[SA[i - 1]] != s[SA[i]]);
	for (int k = 1; k < n; k *= 2)
	{
		auto cmp = [&](int i, int j)
		{
			if (ISA[i] != ISA[j])
				return ISA[i] < ISA[j];
			int rank_i = i + k < n ? ISA[i + k] : -1;
			int rank_j = j + k < n ? ISA[j + k] : -1;
			return rank_i < rank_j;
		};
		int start = 0;
		while (start < n)
		{
			int end = start + 1;
			while (end < n && ISA[SA[start]] == ISA[SA[end]])
				end++;
			if (end - start > 1)
				std::sort(SA.begin() + start, SA.begin() + end, cmp);
			start = end;
		}
		nextISA[SA[0]] = 0;
		for (int i = 1; i < n; i++)
			nextISA[SA[i]] = nextISA[SA[i - 1]] + cmp(SA[i - 1], SA[i]);
		std::swap(ISA, nextISA);
	}
	return SA;
}

/**
* @brief helper function for the suffix array induced sorting algorithm.
* Classifies the S-type and L-type characters of a given string.
* @param s The string for which to classify the S and L characters.
* @param ls The array to store the classification of S-type or L-type.
* @note [Complexity]: O(n)
* @note [Attention]: This function is not intended to be used directly.
*/
void sais_classify_sl(const std::vector<int> &s, std::vector<bool> &ls)
{
	int n = s.size();
	ls[n - 1] = false;
	for (int i = n - 2; i >= 0; i--)
	{
		if (s[i] == s[i + 1])
			ls[i] = ls[i + 1];
		else
			ls[i] = s[i] < s[i + 1];
	}
}

/**
* @brief helper function for the suffix array induced sorting algorithm.
* Counts the number of S-type and L-type characters in a given string.
* @param s The string for which to count the S and L characters.
* @param cnt_l The array to store the count of L-type characters.
* @param cnt_s The array to store the count of S-type characters.
* @param ls The array that stores the classification of S-type or L-type.
* @param max_s The maximum value of the characters in the string.
* @note [Complexity]: O(n)
* @note [Attention]: This function is not intended to be used directly.
*/
void sais_character_count(const std::vector<int> &s, std::vector<int> &cnt_l, std::vector<int> &cnt_s,
		const std::vector<bool> &ls, const int max_s)
{
	int n = s.size();
	for (int i = 0; i < n; i++)
	{
		if (!ls[i])
			cnt_s[s[i]]++;
		else
			cnt_l[s[i] + 1]++;
	}
	for (int i = 0; i <= max_s; i++)
	{
		cnt_s[i] += cnt_l[i];
		if (i < max_s)
			cnt_l[i + 1] += cnt_s[i];
	}
}

/**
* @brief helper function for the suffix array induced sorting algorithm.
* Searches for the LMS substrings in a given string.
* @param n The size of the string.
* @param ls The array that stores the classification of S-type or L-type.
* @param lms_map The array to store the mapping of LMS substrings.
* @param lms The array to store the LMS substrings.
* @note [Complexity]: O(n)
* @note [Attention]: This function is not intended to be used directly.
*/
void sais_search_lms(const int n, const std::vector<bool> &ls, std::vector<int> &lms_map, std::vector<int> &lms)
{
	int m = 0;
	for (int i = 1; i < n; i++)
	{
		if (!ls[i - 1] && ls[i])
		{
			lms_map[i] = m++;
			lms.push_back(i);
		}
	}
}

/**
* @brief helper function for the suffix array induced sorting algorithm.
* Induces the suffix array of a given string.
* @param s The string for which to induce the suffix array.
* @param sa The array to store the suffix array.
* @param cnt_l The array that stores the count of L-type characters.
* @param cnt_s The array that stores the count of S-type characters.
* @param ls The array that stores the classification of S-type or L-type.
* @param lms The array that stores the LMS substrings.
* @note [Complexity]: O(n)
* @note [Attention]: This function is not intended to be used directly.
*/
void sais_induce(const std::vector<int> &s, std::vector<int> &sa, const std::vector<int> &cnt_l,
		const std::vector<int> &cnt_s, const std::vector<bool> &ls, const std::vector<int> &lms)
{
	int n = s.size();
	std::fill(sa.begin(), sa.end(), -1);
	std::vector<int> buf = cnt_s;
	for (auto d : lms)
	{
		if (d == n)
			continue;
		sa[buf[s[d]]++] = d;
	}
	buf = cnt_l;
	sa[buf[s[n - 1]]++] = n - 1;
	for (int i = 0; i < n; i++)
		if (sa[i] >= 1 && !ls[sa[i] - 1])
			sa[buf[s[sa[i] - 1]]++] = sa[i] - 1;
	buf = cnt_l;
	for (int i = n - 1; i >= 0; i--)
		if (sa[i] >= 1 && ls[sa[i] - 1])
			sa[--buf[s[sa[i] - 1] + 1]] = sa[i] - 1;
}

/**
* @brief helper function for the suffix array induced sorting algorithm.
* Determines the order of the LMS substrings in a given string.
* @param s The string for which to resolve the equivalence classes.
* @param sa The array that stores the suffix array.
* @param cnt_l The array that stores the count of L-type characters.
* @param cnt_s The array that stores the count of S-type characters.
* @param ls The array that stores the classification of S-type or L-type.
* @param lms The array that stores the LMS substrings.
* @param lms_map The array that stores the mapping of LMS substrings.
* @note [Complexity]: O(n)
* @note [Attention]: This function is not intended to be used directly.
*/
void sais_resolve(const std::vector<int> &s, std::vector<int> &sa, const std::vector<int> &cnt_l,
		const std::vector<int> &cnt_s, const std::vector<bool> &ls, std::vector<int> &lms, const std::vector<int> &lms_map)
{
	int n = s.size();
	int m = lms.size();
	if (m == 0)
		return;
	std::vector<int> sorted_lms;
	for (int i = 0; i < n; i++)
		if (lms_map[sa[i]] != -1)
			sorted_lms.push_back(sa[i]);
	std::vector<int> rec_s(m);
	int rec_max_s = 0;
	rec_s[lms_map[sorted_lms[0]]] = 0;
	for (int i = 1; i < m; i++)
	{
		int l = sorted_lms[i - 1];
		int r = sorted_lms[i];
		int end_l = (lms_map[l] + 1 < m) ? lms[lms_map[l] + 1] : n;
		int end_r = (lms_map[r] + 1 < m) ? lms[lms_map[r] + 1] : n;
		if (end_l - l != end_r - r)
			rec_max_s++;
		else
		{
			while (l < end_l && s[l] == s[r])
			{
				l++;
				r++;
			}
			if (l == n || s[l] != s[r])
				rec_max_s++;
		}
		rec_s[lms_map[sorted_lms[i]]] = rec_max_s;
	}
	std::vector<int> rec_sa = sais(rec_s, rec_max_s);
	std::copy(lms.begin(), lms.end(), sorted_lms.begin());
	for (int i = 0; i < m; i++)
		lms[i] = sorted_lms[rec_sa[i]];
	sais_induce(s, sa, cnt_l, cnt_s, ls, lms);
}

/**
* @brief Computes the suffix array of a given string using the induced sorting algorithm.
* @param s The string for which to compute the suffix array.
* @param max_s The maximum value of the characters in the string.
* @return A suffix array for the given string.
* @note [Complexity]: O(n)
*/
std::vector<int> sais(const std::vector<int> &s, const int max_s)
{
	const int threshold_doubling = 100;
	int n = s.size();
	if (n == 0)
		return std::vector<int>();
	if (n == 1)
		return std::vector<int>({0});
	if (n == 2)
	{
		if (s[0] < s[1])
			return std::vector<int>({0, 1});
		else
			return std::vector<int>({1, 0});
	}
	if (n < threshold_doubling)
		return suffixarray_doubling(s);
	std::vector<int> sa(n);
	std::vector<bool> ls(n, false);
	std::vector<int> cnt_l(max_s + 1), cnt_s(max_s + 1);
	std::vector<int> lms_map(n, -1), lms;

	sais_classify_sl(s, ls);
	sais_character_count(s, cnt_l, cnt_s, ls, max_s);
	sais_search_lms(n, ls, lms_map, lms);
	sais_induce(s, sa, cnt_l, cnt_s, ls, lms);
	sais_resolve(s, sa, cnt_l, cnt_s, ls, lms, lms_map);
	return sa;
}

} // namespace helper

/**
* @brief Computes the suffix array of a given string.
* @param s The string for which to compute the suffix array, represented as a vector of integers.
* @param max_s The maximum value of the characters in the string.
* @return A suffix array for the given string.
* @note [Complexity]: O(n)
*/
std::vector<int> suffixarray(const std::vector<int> &s, const int max_s)
{
	return helper::sais(s, max_s);
}

/**
* @brief Computes the suffix array of a given string.
* @param str The string for which to compute the suffix array.
* @return A suffix array for the given string.
* @note [Complexity]: O(n)
*/
std::vector<int> suffixarray(const std::string &str)
{
	int n = str.size();
	std::vector<int> s(n);
	for (int i = 0; i < n; i++)
		s[i] = str[i];
	return helper::sais(s, 255);
}

} // namespace string

} // namespace toolbox

#endif // SUFFIXARRAY_HPP
