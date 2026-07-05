#pragma once

#include <functional>
#include <iterator>
#include <vector>

namespace toolbox {
namespace sorting {

namespace detail {

template <typename T, typename Compare>
class BST {
public:
    explicit BST(Compare comp) : _root(NULL), _comp(comp) {}
    ~BST() { clear(_root); }

    void insert(const T &value) { insert(_root, value); }

    std::vector<T> in_order_traversal() const {
        std::vector<T> result;
        in_order_traversal(_root, result);
        return result;
    }

private:
    struct Node {
        T value;
        Node *child[2];  // child[0]: left, child[1]: right
        explicit Node(const T &val) : value(val) {
            child[0] = NULL;
            child[1] = NULL;
        }
    };

    Node *_root;
    Compare _comp;

    void insert(Node *&node, const T &value) {
        if (!node) {
            node = new Node(value);
            return;
        }
        // If _comp(node->value, value) is true: value > node->value, go right
        insert(node->child[_comp(node->value, value) ? 1 : 0], value);
    }

    void in_order_traversal(Node *node, std::vector<T> &result) const {
        if (!node) return;
        in_order_traversal(node->child[0], result);
        result.push_back(node->value);
        in_order_traversal(node->child[1], result);
    }

    void clear(Node *node) {
        if (!node) return;
        clear(node->child[0]);
        clear(node->child[1]);
        delete node;
    }

    BST(const BST &);
    BST &operator=(const BST &);
};

}  // namespace detail

template <typename RandomIt, typename Compare>
void tree_sort(RandomIt first, RandomIt last, Compare comp) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    if (first == last) return;

    detail::BST<T, Compare> bst(comp);
    for (RandomIt it = first; it != last; ++it) {
        bst.insert(*it);
    }
    std::vector<T> sorted = bst.in_order_traversal();
    std::copy(sorted.begin(), sorted.end(), first);
}

template <typename RandomIt>
void tree_sort(RandomIt first, RandomIt last) {
    typedef typename std::iterator_traits<RandomIt>::value_type T;
    tree_sort(first, last, std::less<T>());
}

}  // namespace sorting
}  // namespace toolbox
