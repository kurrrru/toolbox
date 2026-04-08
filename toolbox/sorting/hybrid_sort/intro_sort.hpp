#pragma once

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>

#include "toolbox/sorting/selection_sort/heap_sort.hpp"
#include "toolbox/sorting/insertion_sort/insertion_sort.hpp"
#include "toolbox/sorting/exchange_sort/quick_sort.hpp"

namespace toolbox {
namespace sorting {

namespace detail {

template <typename RandomIt, typename Compare>
typename std::iterator_traits<RandomIt>::value_type
median_to_last(RandomIt first, RandomIt mid, RandomIt last, Compare comp) {
    if (comp(*mid, *first)) std::swap(*mid, *first);
    if (comp(*last, *mid)) std::swap(*last, *mid);
    if (comp(*mid, *first)) std::swap(*mid, *first);
    std::swap(*mid, *last);
    return *last;
}

template <typename RandomIt, typename Compare>
void intro_sort_impl(RandomIt first, RandomIt last, std::size_t depth_limit, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t size = std::distance(first, last);
    if (size <= 32) {
        insertion_sort(first, last, comp);
        return;
    }
    if (depth_limit == 0) {
        heap_sort(first, last, comp);
        return;
    }
    T pivot = median_to_last(first, first + size / 2, last - 1, comp);
    RandomIt pivot_pos = std::partition(first, last - 1,
        IsLessThan<T, Compare>(pivot, comp));
    std::swap(*pivot_pos, *(last - 1));
    intro_sort_impl(first, pivot_pos, depth_limit - 1, comp);
    intro_sort_impl(pivot_pos + 1, last, depth_limit - 1, comp);
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void intro_sort(RandomIt first, RandomIt last, Compare comp) {
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;
    std::size_t depth_limit = static_cast<std::size_t>(
        std::log(static_cast<double>(n)) / std::log(2.0));
    detail::intro_sort_impl(first, last, depth_limit, comp);
}

template <typename RandomIt>
void intro_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    intro_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
