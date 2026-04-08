#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void comb_sort(RandomIt first, RandomIt last, Compare comp) {
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    std::ptrdiff_t gap = n;
    bool swapped = false;

    while (gap > 1 || swapped) {
        if (gap > 1) {
            gap = gap * 10 / 13;
        }
        swapped = false;
        for (std::ptrdiff_t i = 0; i + gap < n; ++i) {
            if (comp(*(first + i + gap), *(first + i))) {
                std::swap(*(first + i), *(first + i + gap));
                swapped = true;
            }
        }
    }
}

template <typename RandomIt>
void comb_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    comb_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
