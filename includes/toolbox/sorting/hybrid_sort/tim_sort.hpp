#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <vector>

#include "toolbox/sorting/insertion_sort/binary_insertion_sort.hpp"

namespace toolbox {
namespace sorting {

namespace detail {

struct TimRun {
    std::size_t start;
    std::size_t length;
};

static const std::size_t TIM_MIN_MERGE = 32;

inline std::size_t tim_min_run_length(std::size_t n) {
    std::size_t r = 0;
    while (n >= TIM_MIN_MERGE) {
        r |= (n & 1u);
        n >>= 1;
    }
    return n + r;
}

template <typename T, typename Compare>
std::size_t count_run_and_make_ascending(std::vector<T> &v, std::size_t start, Compare comp) {
    std::size_t size = v.size();
    if (start >= size - 1) return size - start;

    std::size_t run_end = start + 2;
    if (comp(v[start + 1], v[start])) {
        // Descending run: extend and reverse
        while (run_end < size && comp(v[run_end], v[run_end - 1])) {
            ++run_end;
        }
        std::reverse(v.begin() + start, v.begin() + run_end);
    } else {
        // Ascending run: extend
        while (run_end < size && !comp(v[run_end], v[run_end - 1])) {
            ++run_end;
        }
    }
    return run_end - start;
}

template <typename T, typename Compare>
void merge_at(std::vector<T> &v, std::vector<TimRun> &run_stack, std::size_t idx, Compare comp) {
    TimRun &left = run_stack[idx];
    TimRun &right = run_stack[idx + 1];
    std::inplace_merge(v.begin() + left.start, v.begin() + right.start,
                       v.begin() + right.start + right.length, comp);
    left.length += right.length;
    run_stack.erase(run_stack.begin() + idx + 1);
}

template <typename T, typename Compare>
void merge_collapse(std::vector<T> &v, std::vector<TimRun> &run_stack, Compare comp) {
    while (run_stack.size() > 1) {
        std::size_t n = run_stack.size() - 1;
        std::size_t lenX = run_stack[n].length;
        std::size_t lenY = run_stack[n - 1].length;
        std::size_t lenZ = (n >= 2) ? run_stack[n - 2].length : static_cast<std::size_t>(-1);

        if (n >= 2 && lenZ <= lenY + lenX) {
            if (lenZ < lenX) {
                merge_at(v, run_stack, n - 2, comp);
            } else {
                merge_at(v, run_stack, n - 1, comp);
            }
        } else if (lenY <= lenX) {
            merge_at(v, run_stack, n - 1, comp);
        } else {
            break;
        }
    }
}

template <typename T, typename Compare>
void merge_force_collapse(std::vector<T> &v, std::vector<TimRun> &run_stack, Compare comp) {
    while (run_stack.size() > 1) {
        std::size_t n = run_stack.size() - 1;
        if (n >= 2 && run_stack[n - 2].length < run_stack[n].length) {
            merge_at(v, run_stack, n - 2, comp);
        } else {
            merge_at(v, run_stack, n - 1, comp);
        }
    }
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void tim_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    // Copy to internal vector for efficient index access
    std::vector<T> v(first, last);

    const std::size_t min_run = detail::tim_min_run_length(static_cast<std::size_t>(n));
    std::vector<detail::TimRun> run_stack;
    run_stack.reserve(static_cast<std::size_t>(n) / detail::TIM_MIN_MERGE + 1);

    std::size_t start = 0;
    while (start < static_cast<std::size_t>(n)) {
        std::size_t run_len = detail::count_run_and_make_ascending(v, start, comp);
        if (run_len < min_run) {
            std::size_t forced = std::min(min_run, static_cast<std::size_t>(n) - start);
            // Extend run using binary insertion sort
            binary_insertion_sort(v.begin() + start, v.begin() + start + forced, comp);
            run_len = forced;
        }
        detail::TimRun new_run;
        new_run.start = start;
        new_run.length = run_len;
        run_stack.push_back(new_run);
        detail::merge_collapse(v, run_stack, comp);
        start += run_len;
    }

    detail::merge_force_collapse(v, run_stack, comp);

    std::copy(v.begin(), v.end(), first);
}

template <typename RandomIt>
void tim_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    tim_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
