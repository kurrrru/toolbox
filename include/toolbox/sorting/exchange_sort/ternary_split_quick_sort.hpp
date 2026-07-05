#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>

namespace toolbox {
namespace sorting {

namespace detail {

// 3-way (Dutch National Flag) partition quick sort.
// Uses equivalence check !comp(a,b) && !comp(b,a) for the "equal to pivot" region,
// faithful to the original container[i] == pivot check.
template <typename RandomIt, typename Compare>
void ternary_quick_sort_impl(RandomIt first, std::ptrdiff_t left, std::ptrdiff_t right,
                             Compare comp) {
    if (left >= right) return;
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    T pivot = *(first + left + (right - left) / 2);
    std::ptrdiff_t lt = left;
    std::ptrdiff_t gt = right;
    std::ptrdiff_t i = left;
    while (i <= gt) {
        if (comp(*(first + i), pivot)) {
            std::swap(*(first + lt), *(first + i));
            ++lt;
            ++i;
        } else if (!comp(*(first + i), pivot) && !comp(pivot, *(first + i))) {
            // *(first + i) == pivot (equivalent)
            ++i;
        } else {
            std::swap(*(first + i), *(first + gt));
            --gt;
        }
    }
    ternary_quick_sort_impl(first, left, lt - 1, comp);
    ternary_quick_sort_impl(first, gt + 1, right, comp);
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void ternary_split_quick_sort(RandomIt first, RandomIt last, Compare comp) {
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;
    detail::ternary_quick_sort_impl(first, 0, n - 1, comp);
}

template <typename RandomIt>
void ternary_split_quick_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    ternary_split_quick_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
