#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void bubble_sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last) {
        return;
    }
    bool swapped;
    for (RandomIt i = first; i != last - 1; ++i) {
        swapped = false;
        for (RandomIt j = last - 1; j != i; --j) {
            if (comp(*j, *(j - 1))) {
                std::swap(*j, *(j - 1));
                swapped = true;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

template <typename RandomIt>
void bubble_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    bubble_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
