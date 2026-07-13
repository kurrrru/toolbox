#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void shaker_sort(RandomIt first, RandomIt last, Compare comp) {
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    std::ptrdiff_t left = 0;
    std::ptrdiff_t right = n - 1;
    while (true) {
        std::ptrdiff_t last_swap = left;
        for (std::ptrdiff_t i = left; i < right; ++i) {
            if (comp(*(first + i + 1), *(first + i))) {
                std::swap(*(first + i), *(first + i + 1));
                last_swap = i;
            }
        }
        right = last_swap;
        if (left == right) break;

        last_swap = right;
        for (std::ptrdiff_t i = right; i > left; --i) {
            if (comp(*(first + i), *(first + i - 1))) {
                std::swap(*(first + i), *(first + i - 1));
                last_swap = i;
            }
        }
        left = last_swap;
        if (left == right) break;
    }
}

template <typename RandomIt>
void shaker_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    shaker_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
