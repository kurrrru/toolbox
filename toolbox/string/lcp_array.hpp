#ifndef LCP_ARRAY_HPP
#define LCP_ARRAY_HPP

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

namespace toolbox
{

namespace string
{

/**
* @brief Computes the longest common prefix array of a given string and its suffix array.
* @param s The string for which to compute the longest common prefix array.
* @param sa The suffix array of the given string.
* @return A longest common prefix array for the given string.
* @note [Complexity]: O(n), where n is the size of the string.
*/
std::vector<int> lcp_array(const std::string &s, const std::vector<int> &sa)
{
	int n = s.size();
	std::vector<int> isa(n + 1);
	for (int i = 0; i < n + 1; i++)
		isa[sa[i]] = i;
	std::vector<int> lcp(n, 0);
	int l = 0;
	for (int i = 0; i < n; i++)
	{
		if (isa[i] == n)
		{
			l = 0;
			continue;
		}
		int j = sa[isa[i] + 1];
		while (i + l < n && j + l < n && s[i + l] == s[j + l])
			l++;
		lcp[isa[i]] = l;
		if (l > 0)
			l--;
	}
	return lcp;
}

} // namespace string

} // namespace toolbox

#endif // LCP_ARRAY_HPP
