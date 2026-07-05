#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

// A tournament tree node: holds a value and a validity flag.
// An invalid node represents "negative infinity" (already extracted).
template <typename T>
struct TNode {
    T value;
    bool valid;
    TNode() : value(T()), valid(false) {}
    explicit TNode(const T &v) : value(v), valid(true) {}
};

// Sift down: replace heap[0] with an invalid node and restore heap property.
// Uses the same comparator as the original heapDecreaseKey: comp(parent, child)
// means parent < child, so larger-valued nodes are higher (max-heap).
template <typename T, typename Compare>
void heap_decrease_key(std::vector<TNode<T>> &heap, std::size_t index, Compare comp) {
    heap[index].valid = false;
    std::size_t n = heap.size();
    while (true) {
        std::size_t left = 2 * index + 1;
        std::size_t right = 2 * index + 2;
        std::size_t largest = index;
        // A valid node is always "larger" than an invalid one
        if (left < n) {
            if (!heap[largest].valid && heap[left].valid) {
                largest = left;
            } else if (heap[largest].valid && heap[left].valid &&
                       comp(heap[largest].value, heap[left].value)) {
                largest = left;
            }
        }
        if (right < n) {
            if (!heap[largest].valid && heap[right].valid) {
                largest = right;
            } else if (heap[largest].valid && heap[right].valid &&
                       comp(heap[largest].value, heap[right].value)) {
                largest = right;
            }
        }
        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            index = largest;
        } else {
            break;
        }
    }
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void tournament_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 0) return;

    // Build tournament tree as a max-heap
    std::vector<detail::TNode<T>> tree;
    tree.reserve(n);
    for (RandomIt it = first; it != last; ++it) {
        tree.push_back(detail::TNode<T>(*it));
    }

    // Build max-heap using comp: comp(a,b) means a < b, so larger elements rise
    auto node_comp = [&comp](const detail::TNode<T> &a, const detail::TNode<T> &b) {
        if (!a.valid) return true;
        if (!b.valid) return false;
        return comp(a.value, b.value);
    };
    std::make_heap(tree.begin(), tree.end(), node_comp);

    // Extract max one by one, placing from end to front (ascending result)
    for (std::ptrdiff_t i = n - 1; i >= 0; --i) {
        *(first + i) = tree[0].value;
        detail::heap_decrease_key(tree, 0, comp);
    }
}

template <typename RandomIt>
void tournament_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    tournament_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
