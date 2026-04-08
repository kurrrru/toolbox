#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void binary_insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    if (std::distance(first, last) <= 1) return;
    for (RandomIt it = first + 1; it != last; ++it) {
        typename std::iterator_traits<RandomIt>::value_type key = *it;
        RandomIt pos = std::lower_bound(first, it, key, comp);
        std::rotate(pos, it, it + 1);
    }
}

template <typename RandomIt>
void binary_insertion_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    binary_insertion_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
