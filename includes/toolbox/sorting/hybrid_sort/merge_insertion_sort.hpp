#pragma once

/*
 * Merge-Insertion Sort (Ford-Johnson Algorithm)
 *
 * This is a faithful port of the PmergeMe implementation from cpp-module-09.
 * The algorithm minimizes comparisons by:
 *   1. Pairing adjacent elements and recursively sorting the larger of each pair.
 *   2. Inserting the smaller elements into the sorted sequence using a binary
 *      search order determined by the Jacobsthal sequence.
 *
 * Internal types mirror the original's ValIdx / PairType / ChainType scheme,
 * but are simplified to work with any value type T.
 */

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

// Advance the Jacobsthal pair (current, prev) to the next values.
// Sequence of "current": 3, 5, 11, 21, 43, ...
inline void next_jacobsthal(std::pair<std::size_t, std::size_t> &j) {
    std::swap(j.first, j.second);
    j.first = j.first * 2 + j.second;
}

// Element in the main chain during insertion.
// value: the element value (with its "pair-level" index for reordering)
// is_anchor: true if this element is the "bigger" (anchor) half of a pair
template <typename T>
struct ChainElem {
    std::pair<T, std::size_t> value;  // (element value, pair-level index)
    bool is_anchor;

    ChainElem() : is_anchor(false) {}
    ChainElem(std::pair<T, std::size_t> v, bool a) : value(v), is_anchor(a) {}
};

// ford_johnson_indexed: sorts a vector of (value, pair-index) pairs using
// the Ford-Johnson algorithm.  After the call the vector is sorted by value.
// The pair-index field is used internally to track which original pair each
// element belongs to; callers may set it arbitrarily for the top-level call.
template <typename T, typename Compare>
void ford_johnson_indexed(std::vector<std::pair<T, std::size_t>> &container, Compare comp) {
    typedef std::pair<T, std::size_t> ValIdx;
    typedef std::pair<ValIdx, ValIdx> PairType;  // (bigger, smaller)
    typedef ChainElem<T> Chain;

    std::size_t num_elements = container.size();
    if (num_elements <= 1) {
        return;
    }

    std::size_t num_pairs = num_elements / 2;
    bool has_straggler = (num_elements % 2) == 1;
    ValIdx straggler = has_straggler ? container.back() : ValIdx(T(), 0);

    // --- Step 1: cmpNeighbor ---
    // Pair adjacent elements; build pairs[] and bigger[].
    // pairs[i] = (bigger_valIdx, smaller_valIdx)
    // bigger[i] = (bigger_value, i)   -- tracks original pair index
    std::vector<PairType> pairs;
    std::vector<ValIdx> bigger;
    pairs.reserve(num_pairs);
    bigger.reserve(num_pairs);

    for (std::size_t i = 0; i < num_pairs; ++i) {
        ValIdx &a = container[2 * i];
        ValIdx &b = container[2 * i + 1];
        if (comp(a.first, b.first)) {
            pairs.push_back(std::make_pair(b, a));
            bigger.push_back(std::make_pair(b.first, i));
        } else {
            pairs.push_back(std::make_pair(a, b));
            bigger.push_back(std::make_pair(a.first, i));
        }
    }

    // --- Step 2: Recursive sort of bigger elements ---
    ford_johnson_indexed(bigger, comp);

    // --- Step 3: reorderPairs ---
    // Reorder pairs[] so they follow the sorted order of bigger[].
    // bigger[i].second is the original pair index for the i-th sorted element.
    std::vector<PairType> ordered_pairs;
    ordered_pairs.reserve(num_pairs);
    for (std::size_t i = 0; i < num_pairs; ++i) {
        ordered_pairs.push_back(pairs[bigger[i].second]);
    }
    pairs.swap(ordered_pairs);

    // --- Step 4: createMainChain ---
    // Main chain starts with b_1 (smaller of first pair) then a_1...a_k.
    std::vector<Chain> main_chain;
    main_chain.reserve(num_pairs + 1);
    main_chain.push_back(Chain(pairs[0].second, false));  // b_1
    for (std::size_t i = 0; i < num_pairs; ++i) {
        main_chain.push_back(Chain(pairs[i].first, true));  // a_i
    }

    // --- Step 5: insertIntoMainChain ---
    // Append straggler to pairs so the Jacobsthal loop handles it uniformly.
    if (has_straggler) {
        pairs.push_back(std::make_pair(ValIdx(T(), 0), straggler));
    }

    // Comparator for binary search: compare by value field only.
    struct ChainComp {
        Compare c;
        explicit ChainComp(Compare c_) : c(c_) {}
        bool operator()(const Chain &a, const Chain &b) const {
            return c(a.value.first, b.value.first);
        }
    } chain_comp(comp);

    std::pair<std::size_t, std::size_t> jacobsthal = std::make_pair(1, 1);
    while (true) {
        next_jacobsthal(jacobsthal);
        for (std::size_t i = jacobsthal.first; i > jacobsthal.second; --i) {
            if (i > pairs.size()) {
                i = pairs.size();
            }
            if (i <= 1) {
                break;
            }

            const ValIdx &insert_value = pairs[i - 1].second;

            // Find the upper bound of the search range.
            // For a real pair (not straggler), b_i <= a_i, so we search only up
            // to the position of the i-th anchor in main_chain.
            typename std::vector<Chain>::iterator end_it = main_chain.end();
            bool is_straggler_elem = (i > num_pairs);
            if (!is_straggler_elem) {
                std::size_t anchor_count = 0;
                for (typename std::vector<Chain>::iterator it = main_chain.begin();
                     it != main_chain.end(); ++it) {
                    if (it->is_anchor) {
                        ++anchor_count;
                        if (anchor_count == i) {
                            end_it = it;
                            break;
                        }
                    }
                }
            }

            // Binary search for insertion position.
            Chain probe(insert_value, false);
            typename std::vector<Chain>::iterator pos =
                std::lower_bound(main_chain.begin(), end_it, probe, chain_comp);
            main_chain.insert(pos, probe);
        }
        if (jacobsthal.first >= pairs.size()) {
            break;
        }
    }

    // --- Step 6: writeBack ---
    for (std::size_t i = 0; i < container.size(); ++i) {
        container[i] = main_chain[i].value;
    }
}

}  // namespace detail

template <typename RandomIt, typename Compare>
void merge_insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) {
        return;
    }

    // Build indexed vector: (value, position)
    std::vector<std::pair<T, std::size_t>> indexed;
    indexed.reserve(static_cast<std::size_t>(n));
    std::size_t idx = 0;
    for (RandomIt it = first; it != last; ++it, ++idx) {
        indexed.push_back(std::make_pair(*it, idx));
    }

    detail::ford_johnson_indexed(indexed, comp);

    // Write sorted values back
    RandomIt out = first;
    for (std::size_t i = 0; i < static_cast<std::size_t>(n); ++i, ++out) {
        *out = indexed[i].first;
    }
}

template <typename RandomIt>
void merge_insertion_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    merge_insertion_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
