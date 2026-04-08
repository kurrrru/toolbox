#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void odd_even_sort(RandomIt first, RandomIt last, Compare comp) {
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (std::ptrdiff_t i = 1; i < n - 1; i += 2) {
            if (comp(*(first + i + 1), *(first + i))) {
                std::swap(*(first + i), *(first + i + 1));
                sorted = false;
            }
        }
        for (std::ptrdiff_t i = 0; i < n - 1; i += 2) {
            if (comp(*(first + i + 1), *(first + i))) {
                std::swap(*(first + i), *(first + i + 1));
                sorted = false;
            }
        }
    }
}

template <typename RandomIt>
void odd_even_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    odd_even_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
