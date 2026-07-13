#pragma once

#include <algorithm>
#include <functional>
#include <iterator>
#include <queue>
#include <utility>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

// Cartesian tree built with comp: root satisfies comp(child, root) is true for
// every child (i.e. the root is the "maximum" under comp).
template <typename T, typename Compare>
class CartesianTree {
 public:
    typedef std::pair<T, int> PairType;

    struct PairComp {
        Compare c;
        explicit PairComp(Compare c_) : c(c_) {}
        bool operator()(const PairType &a, const PairType &b) const {
            if (c(a.first, b.first)) return true;
            if (c(b.first, a.first)) return false;
            return a.second < b.second;
        }
    };

    CartesianTree(const std::vector<T> &arr, Compare comp)
        : _root(-1),
          _n(static_cast<int>(arr.size())),
          _parent(arr.size(), -1),
          _right(arr.size(), -1),
          _left(arr.size(), -1),
          _arr(arr),
          _comp(comp) {
        build();
    }

    std::vector<T> sort() const {
        if (_root == -1) return std::vector<T>();

        std::vector<T> sorted_arr;
        sorted_arr.reserve(static_cast<std::size_t>(_n));

        PairComp pc(_comp);
        std::priority_queue<PairType, std::vector<PairType>, PairComp> pq(pc);

        pq.push(std::make_pair(_arr[_root], _root));

        while (!pq.empty()) {
            PairType top = pq.top();
            pq.pop();
            int current_idx = top.second;
            sorted_arr.push_back(_arr[current_idx]);
            if (_left[current_idx] != -1) {
                pq.push(std::make_pair(_arr[_left[current_idx]], _left[current_idx]));
            }
            if (_right[current_idx] != -1) {
                pq.push(std::make_pair(_arr[_right[current_idx]], _right[current_idx]));
            }
        }
        std::reverse(sorted_arr.begin(), sorted_arr.end());
        return sorted_arr;
    }

 private:
    void build() {
        std::vector<int> st;
        for (int i = 0; i < _n; ++i) {
            if (_root == -1) {
                _root = i;
                st.push_back(i);
                continue;
            }
            while (!st.empty() && _comp(_arr[st.back()], _arr[i])) {
                st.pop_back();
            }
            if (st.empty()) {
                _left[i] = _root;
                _parent[_root] = i;
                _root = i;
                st.push_back(i);
            } else {
                _left[i] = _right[st.back()];
                _parent[st.back()] = i;
                _right[st.back()] = i;
                _parent[i] = st.back();
                st.push_back(i);
            }
        }
    }

    int _root;
    int _n;
    std::vector<int> _parent;
    std::vector<int> _right;
    std::vector<int> _left;
    std::vector<T> _arr;
    Compare _comp;
};

}  // namespace detail

template <typename RandomIt, typename Compare>
void cartesian_tree_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    std::ptrdiff_t n = std::distance(first, last);
    if (n <= 1) return;

    std::vector<T> v(first, last);
    detail::CartesianTree<T, Compare> tree(v, comp);
    std::vector<T> sorted = tree.sort();
    std::copy(sorted.begin(), sorted.end(), first);
}

template <typename RandomIt>
void cartesian_tree_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    cartesian_tree_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
