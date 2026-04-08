#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

template <typename RandomIt, typename Compare>
void merge_sort_impl(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    RandomIt mid = first + n / 2;
    std::vector<T> left(first, mid);
    std::vector<T> right(mid, last);

    merge_sort_impl(left.begin(), left.end(), comp);
    merge_sort_impl(right.begin(), right.end(), comp);

    std::merge(left.begin(), left.end(), right.begin(), right.end(), first, comp);
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void merge_sort(RandomIt first, RandomIt last, Compare comp) {
    detail::merge_sort_impl(first, last, comp);
}

template <typename RandomIt>
void merge_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    merge_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
