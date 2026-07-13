#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

// Note: the condition !comp(*(it-1), *it) is equivalent to *(it-1) <= *it
// (i.e., the pair is already in non-decreasing order), matching the original
// cmp(container[index-1], container[index]) || container[index-1] == container[index].
template <typename RandomIt, typename Compare>
void gnome_sort(RandomIt first, RandomIt last, Compare comp) {
    if (first == last) {
        return;
    }
    RandomIt it = first + 1;
    while (it != last) {
        if (!comp(*it, *(it - 1))) {
            ++it;
        } else {
            std::swap(*(it - 1), *it);
            if (it - 1 == first) {
                ++it;
            } else {
                --it;
            }
        }
    }
}

template <typename RandomIt>
void gnome_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    gnome_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
