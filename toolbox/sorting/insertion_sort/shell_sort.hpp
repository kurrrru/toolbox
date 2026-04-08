#pragma once

#include <functional>
#include <iterator>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

inline std::size_t shell_pow(std::size_t base, std::size_t exp) {
    std::size_t result = 1;
    while (exp) {
        if (exp % 2 == 1) result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

inline void generate_gap_sequence(std::vector<std::size_t>& gaps, std::size_t max_value) {
    gaps.clear();
    gaps.push_back(1);
    std::size_t k = 1;
    while (true) {
        std::size_t gap = shell_pow(4, k) + 3 * shell_pow(2, k - 1) + 1;
        if (gap > max_value) break;
        gaps.push_back(gap);
        ++k;
    }
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void shell_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    std::vector<std::size_t> gaps;
    detail::generate_gap_sequence(gaps, static_cast<std::size_t>(n) / 2);

    for (int gi = static_cast<int>(gaps.size()) - 1; gi >= 0; --gi) {
        std::size_t gap = gaps[gi];
        for (std::ptrdiff_t j = static_cast<std::ptrdiff_t>(gap); j < n; ++j) {
            T temp = *(first + j);
            std::ptrdiff_t k = j;
            while (k >= static_cast<std::ptrdiff_t>(gap) &&
                   comp(temp, *(first + k - static_cast<std::ptrdiff_t>(gap)))) {
                *(first + k) = *(first + k - static_cast<std::ptrdiff_t>(gap));
                k -= static_cast<std::ptrdiff_t>(gap);
            }
            *(first + k) = temp;
        }
    }
}

template <typename RandomIt>
void shell_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    shell_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
