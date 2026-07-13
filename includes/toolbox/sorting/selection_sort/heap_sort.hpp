#pragma once

#include <algorithm>
#include <functional>
#include <iterator>

namespace toolbox {
namespace sorting {

template <typename RandomIt, typename Compare>
void heap_sort(RandomIt first, RandomIt last, Compare comp) {
    std::make_heap(first, last, comp);
    std::sort_heap(first, last, comp);
}

template <typename RandomIt>
void heap_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    heap_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
