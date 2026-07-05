#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    if (std::distance(first, last) <= 1) return;
    for (RandomIt it = first + 1; it != last; ++it) {
        typename std::iterator_traits<RandomIt>::value_type key = *it;
        RandomIt j = it;
        while (j != first) {
            RandomIt prev = j - 1;
            if (comp(key, *prev)) {
                --j;
            } else {
                break;
            }
        }
        std::rotate(j, it, it + 1);
    }
}

template <typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    insertion_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
