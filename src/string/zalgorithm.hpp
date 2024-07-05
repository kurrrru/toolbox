#ifndef ZALGORITHM_HPP
#define ZALGORITHM_HPP

#include <string>
#include <vector>

namespace toolbox
{

/**
* @brief Computes the Z-array, which satisfies Z[i] = the length of lcp(S, S[i:]).
* @param S The string for which to compute the Z-algorithm.
* @return A Z-array for the given string.
* @note [Complexity]: O(|S|)
*/
std::vector<int> zalgorithm(const std::string &S)
{
	int n = S.size();
	std::vector<int> ans(n, 0);
	ans[0] = n;
	int start = 1, length = 0;
	while (start < n)
	{
		while (start + length < n && S[length] == S[start + length])
			++length;
		ans[start] = length;
		if (length == 0)
		{
			++start;
			continue;
		}
		int inner = 1;
		while (start + inner < n && inner + ans[inner] < length)
		{
			ans[start + inner] = ans[inner];
			++inner;
		}
		start += inner;
		length -= inner;
	}
	return ans;
}

} // namespace toolbox

#endif // ZALGORITHM_HPP
