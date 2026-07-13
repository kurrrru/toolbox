#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

namespace detail {

template <typename T, typename Compare>
struct IsLessThan {
    T pivot_value;
    Compare comp;
    IsLessThan(T pivot, Compare c) : pivot_value(pivot), comp(c) {}
    bool operator()(const T &value) const { return comp(value, pivot_value); }
};

template <typename RandomIt, typename Compare>
void quick_sort_impl(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    if (std::distance(first, last) <= 1) {
        return;
    }

    RandomIt pivot_it = last - 1;
    T pivot = *pivot_it;
    RandomIt pivot_pos = std::partition(first, pivot_it, IsLessThan<T, Compare>(pivot, comp));
    std::swap(*pivot_pos, *pivot_it);
    quick_sort_impl(first, pivot_pos, comp);
    quick_sort_impl(pivot_pos + 1, last, comp);
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void quick_sort(RandomIt first, RandomIt last, Compare comp) {
    detail::quick_sort_impl(first, last, comp);
}

template <typename RandomIt>
void quick_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    quick_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
