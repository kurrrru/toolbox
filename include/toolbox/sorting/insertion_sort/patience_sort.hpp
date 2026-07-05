#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

// k-way merge of piles: each pile is in descending order (top = smallest)
template <typename T, typename Compare>
std::vector<T> merge_piles(std::vector<std::vector<T>> piles, std::size_t n, Compare comp) {
    std::vector<T> result;
    result.reserve(n);
    while (true) {
        // Find the pile with the minimum top element
        int min_index = -1;
        for (std::size_t i = 0; i < piles.size(); ++i) {
            if (!piles[i].empty()) {
                if (min_index == -1 || comp(piles[i].back(), piles[min_index].back())) {
                    min_index = static_cast<int>(i);
                }
            }
        }
        if (min_index == -1) break;
        result.push_back(piles[min_index].back());
        piles[min_index].pop_back();
        if (piles[min_index].empty()) {
            piles.erase(piles.begin() + min_index);
        }
    }
    return result;
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void patience_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    std::vector<std::vector<T>> piles;

    for (RandomIt it = first; it != last; ++it) {
        const T &val = *it;
        if (piles.empty()) {
            piles.push_back(std::vector<T>(1, val));
        } else {
            bool placed = false;
            for (std::size_t j = 0; j < piles.size(); ++j) {
                if (comp(val, piles[j].back())) {
                    piles[j].push_back(val);
                    placed = true;
                    break;
                }
            }
            if (!placed) {
                piles.push_back(std::vector<T>(1, val));
            }
        }
    }

    std::vector<T> sorted = detail::merge_piles(piles, static_cast<std::size_t>(n), comp);
    std::copy(sorted.begin(), sorted.end(), first);
}

template <typename RandomIt>
void patience_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    patience_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
