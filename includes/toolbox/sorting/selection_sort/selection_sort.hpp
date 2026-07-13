#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void selection_sort(RandomIt first, RandomIt last, Compare comp) {
    if (std::distance(first, last) <= 1) {
        return;
    }
    for (RandomIt i = first; i != last - 1; ++i) {
        RandomIt min_it = std::min_element(i, last, comp);
        if (min_it != i) {
            std::swap(*i, *min_it);
        }
    }
}

template <typename RandomIt>
void selection_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    selection_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
