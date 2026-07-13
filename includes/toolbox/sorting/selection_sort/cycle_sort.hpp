#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void cycle_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) {
        return;
    }

    for (std::ptrdiff_t cycle_start = 0; cycle_start < n - 1; ++cycle_start) {
        T item = *(first + cycle_start);
        std::ptrdiff_t pos = cycle_start;

        for (std::ptrdiff_t i = cycle_start + 1; i < n; ++i) {
            if (comp(*(first + i), item)) {
                ++pos;
            }
        }
        if (pos == cycle_start) {
            continue;
        }

        // Skip elements equivalent to item (handle duplicates)
        while (!comp(item, *(first + pos)) && !comp(*(first + pos), item)) {
            ++pos;
        }
        if (pos != cycle_start) {
            std::swap(item, *(first + pos));
        }

        while (pos != cycle_start) {
            pos = cycle_start;
            for (std::ptrdiff_t i = cycle_start + 1; i < n; ++i) {
                if (comp(*(first + i), item)) {
                    ++pos;
                }
            }
            while (!comp(item, *(first + pos)) && !comp(*(first + pos), item)) {
                ++pos;
            }
            if (comp(item, *(first + pos)) || comp(*(first + pos), item)) {
                std::swap(item, *(first + pos));
            }
        }
    }
}

template <typename RandomIt>
void cycle_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    cycle_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
