#pragma once

#include <algorithm>
#include <cassert>
#include <string>
#include <vector>

namespace toolbox {

namespace string {

/**
 * @brief Computes the longest common prefix array of a given string and its suffix array,
 * using Kasai's algorithm.
 * @param s The string for which to compute the longest common prefix array.
 * @param sa The suffix array of the given string, as returned by suffixarray()
 * (n elements, no sentinel).
 * @return A longest common prefix array of length n, where lcp[i] is the length of the
 * longest common prefix of the suffixes sa[i-1] and sa[i]. lcp[0] is always 0.
 * @note [Complexity]: O(n), where n is the size of the string.
 */
std::vector<int> lcp_array(const std::string &s, const std::vector<int> &sa) {
    int n = s.size();
    assert(static_cast<int>(sa.size()) == n);
    std::vector<int> rank(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;
    std::vector<int> lcp(n, 0);
    int h = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == 0) {
            h = 0;
            continue;
        }
        int j = sa[rank[i] - 1];
        while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
        lcp[rank[i]] = h;
        if (h > 0) h--;
    }
    return lcp;
}

}  // namespace string

}  // namespace toolbox
