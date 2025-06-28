/*
Wavelet Tree

This is an implementation of a wavelet tree data structure. 
It supports the following operations:
- build: build a wavelet tree from an input array
- access: get the value of the bit at index i
- rank: count the occurrence of c in [0, i)
- range_count: count the number of k that satisfies i <= k < j and lower <= T[k] < upper
- range_list: list the characters in [i, j) that satisfies lower <= T[k] < upper
- freq: count the number of k that satisfies i <= k < j and T[k] = c
- is_range_unary: check if the range [i, j) is unary

It will support the following operations in the future:
- select: get the index of the j-th occurrence of c
*/

#pragma once

#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

namespace {

uint32_t popcount(uint32_t x) {
    x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
    x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
    x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);
    return (x);
}

uint32_t bit_width(uint32_t x) {
    uint32_t ret = 0;
    while (x) {
        x >>= 1;
        ret++;
    }
    return (ret);
}

}  // namespace

namespace toolbox
{

namespace datastructure
{

class bitvector {
public:
    bitvector(): _num_blocks(0), _len(0) {}
    explicit bitvector(std::vector<bool> &arr) { build(arr); }
    bitvector(const bitvector &other): _bit(other._bit), _sum(other._sum), _num_blocks(other._num_blocks), _len(other._len) {}
    bitvector &operator=(const bitvector &other) {
        if (this != &other) {
            _bit = other._bit;
            _sum = other._sum;
            _num_blocks = other._num_blocks;
            _len = other._len;
        }
        return (*this);
    }
    ~bitvector() {}


    // build
    // @ arr: input array
    // return: void
    void build(std::vector<bool> &arr) {
        _len = arr.size();
        _num_blocks = (_len + 0b11111) >> 5;
        _bit.assign(_num_blocks, 0u);
        _sum.assign(_num_blocks + 1, 0u);
        _set(arr);
        for (std::size_t i = 1; i <= _num_blocks; i++)
            _sum[i] = _sum[i - 1] + popcount(_bit[i - 1]);
    }

    // operator[]
    // @ i: index
    // return: value of the bit at index i
    bool operator[](std::size_t i) const {
        assert(i < _len);
        return (_bit[i >> 5] & (1u << (i & 0b11111)));
    }

    // rank
    // @ i: index
    // Count the occurrence of 1 in [0, i) in O(1) time
    // O(n) bits are used in this implementation, but it can be reduced to n + o(n) bits
    // return: the occurrence of 1 in [0, i)
    uint32_t rank(std::size_t i) const {
        assert(i <= _len);
        if (i == 0)
            return (0);
        return (_sum[i >> 5] + popcount(_bit[i >> 5] & ((1u << (i & 0b11111)) - 1)));
    }

    // select
    // @ j: the j-th occurrence of 1
    // Ideally, time complexity is O(1), but it is O(log n) in this implementation 
    // return: the index of the j-th occurrence of 1
    std::size_t select(uint32_t j) const {
        assert(0 < j && j <= _sum[_num_blocks]);
        int left = -1, right = _len;
        while (left + 1 < right) {
            int mid = (left + right) >> 1;
            if (rank(static_cast<std::size_t>(mid)) < j)
                left = mid;
            else
                right = mid;
        }
        return (left);
    }

private:
    std::vector<uint32_t> _bit;
    std::vector<uint32_t> _sum;
    std::size_t _num_blocks;
    std::size_t _len;

    void    _set(std::vector<bool> &arr) {
        for (std::size_t i = 0; i < _len; i++)
            if (arr[i])
                _bit[i >> 5] |= 1u << (i & 0b11111);
    }
};

class wavelet_tree {
public:
    wavelet_tree(): root(nullptr), _len(0), _size(0), _capacity(0) {}
    explicit wavelet_tree(std::vector<uint32_t> &arr) {
        build(arr);
    }
    wavelet_tree(const wavelet_tree &other): root(other.root), _len(other._len), _size(other._size), _capacity(other._capacity) {}
    wavelet_tree &operator=(const wavelet_tree &other) {
        if (this != &other) {
            root = other.root;
            _len = other._len;
            _size = other._size;
            _capacity = other._capacity;
        }
        return (*this);
    }
    ~wavelet_tree() {
    }

    // wavelet_tree::build
    // @ arr: input array
    // assumption: min(arr) = 0
    // Build a wavelet tree in O(n log s) time (s = _size, n = _len)
    // return: void
    void build(std::vector<uint32_t> &arr) {
        _len = arr.size();
        if (_len == 0)
            _size = 0;
        else
            _size = *std::max_element(arr.begin(), arr.end()) + 1;
        _capacity = 1 << bit_width(_size);
        build_rec(&root, arr, 0, _capacity);
    }

    // wavelet_tree::access
    // @ i: index
    // Access the i-th element in O(log s) time (s = _size)
    // return: value of the bit at index i
    uint32_t access(std::size_t i) const {
        assert(i < _len);
        _node *cur = root;
        uint32_t ret = 0;
        while (cur != nullptr && cur->_l < cur->_r - 1) {
            if (cur->_bv[i]) {
                ret += (cur->_r - cur->_l) / 2;
                i = cur->_bv.rank(i);
                cur = cur->_right;
            } else {
                i = i - cur->_bv.rank(i);
                cur = cur->_left;
            }
        }
        return (ret);
    }

    // wavelet_tree::rank
    // @ i: index
    // @ c: character
    // Count the occurrence of c in [0, i) in O(log s) time (s = _size)
    // return: the occurrence of c in [0, i)
    uint32_t rank(std::size_t i, uint32_t c) const {
        assert(i <= _len);
        if (i == 0 || c >= _size)
            return (0);
        _node *cur = root;
        while (cur->_l < cur->_r - 1) {
            uint32_t mid = (cur->_r + cur->_l) >> 1;
            if (c >= mid) {
                i = cur->_bv.rank(i);
                cur = cur->_right;
            } else {
                i = i - cur->_bv.rank(i);
                cur = cur->_left;
            }
            if (cur == nullptr)
                return (0);
        }
        return (i);
    }

    // wavelet_tree::range_count
    // @ i: index (inclusive)
    // @ j: index (exclusive)
    // @ lower: lower bound of the range of characters (inclusive)
    // @ upper: upper bound of the range of characters (exclusive)
    // Count the number of k that satisfies i <= k < j and lower <= T[k] < upper
    // return: the number of k that satisfies the condition
    uint32_t range_count(std::size_t i, std::size_t j, uint32_t lower, uint32_t upper) const {
        assert(lower <= upper);
        return (range_count_rec(root, i, j, lower, upper));
    }

    // wavelet_tree::freq
    // @ i: index (inclusive)
    // @ j: index (exclusive)
    // @ c: character
    // Count the number of k that satisfies i <= k < j and T[k] = c
    // return: the number of k that satisfies the condition
    uint32_t freq(std::size_t i, std::size_t j, uint32_t c) {
        return (rank(j, c) - rank(i, c));
    }

    // wavelet_tree::is_range_unary
    // @ i: index (inclusive)
    // @ j: index (exclusive)
    // Check if the range [i, j) is unary
    // return: true if the range is unary, false otherwise
    bool is_range_unary(std::size_t i, std::size_t j) {
        assert(i < _len);
        return (freq(i, j, access(i)) == j - i);
    }

    // wavelet_tree::range_list
    // @ i: index (inclusive)
    // @ j: index (exclusive)
    // @ lower: lower bound of the range of characters (inclusive)
    // @ upper: upper bound of the range of characters (exclusive)
    // list the characters in [i, j) that satisfies lower <= T[k] < upper
    // return: the list of characters
    std::vector<uint32_t> range_list(std::size_t i, std::size_t j, uint32_t lower, uint32_t upper) const {
        std::vector<uint32_t> v;
        range_list_rec(root, i, j, lower, upper, v);
        return (v);
    }

private:
    class _node {
    public:
        _node(uint32_t l, uint32_t r, std::vector<bool> &arr)
            : _l(l), _r(r), _bv(arr), _left(nullptr), _right(nullptr) {}
        _node(const _node &other)
            : _l(other._l), _r(other._r), _bv(other._bv), _left(other._left), _right(other._right) {}
        _node &operator=(const _node &other) {
            if (this != &other) {
                _l = other._l;
                _r = other._r;
                _bv = other._bv;
                _left = other._left;
                _right = other._right;
            }
            return (*this);
        }
        ~_node() {
            if (_left) _left->free_node();
            if (_right) _right->free_node();
        }
        void free_node() {
            if (_left) _left->free_node();
            if (_right) _right->free_node();
            delete this;
        }
        uint32_t _l, _r;
        bitvector _bv;
        _node *_left, *_right;
    };
    _node *root;
    std::size_t _len;
    std::size_t _size;
    std::size_t _capacity;

    void build_rec(_node **cur, std::vector<uint32_t> &arr, uint32_t lower, uint32_t upper) {
        std::vector<bool> bit(arr.size());
        std::vector<uint32_t> left_arr, right_arr;
        if (lower >= _size || upper == lower)
            return ;
        uint32_t mid = (lower + upper) >> 1;
        for (std::size_t i = 0; i < arr.size(); i++) {
            bit[i] = (arr[i] >= mid);
            if (bit[i])
                right_arr.push_back(arr[i]);
            else
                left_arr.push_back(arr[i]);
        }
        *cur = new _node(lower, upper, bit);
        if (lower == upper - 1)
            return ;
        build_rec(&((*cur)->_left), left_arr, lower, mid);
        build_rec(&((*cur)->_right), right_arr, mid, upper);
    }

    uint32_t range_count_rec(_node *cur, std::size_t i, std::size_t j, uint32_t lower, uint32_t upper) const {
        if (cur == nullptr || i >= j || upper <= cur->_l || lower >= cur->_r)
            return (0);
        if (lower <= cur->_l && cur->_r <= upper)
            return (j - i);
        return (range_count_rec(cur->_right, cur->_bv.rank(i), cur->_bv.rank(j), lower, upper) +
                range_count_rec(cur->_left, i - cur->_bv.rank(i), j - cur->_bv.rank(j), lower, upper));
    }

    void range_list_rec(_node *cur, std::size_t i, std::size_t j, uint32_t lower, uint32_t upper, std::vector<uint32_t> &v) const {
        if (cur == nullptr || i >= j || upper <= cur->_l || lower >= cur->_r)
            return ;
        if (lower <= cur->_l && cur->_r <= upper && cur->_l == cur->_r - 1) {
            v.push_back(cur->_l);
            return ;
        }
        range_list_rec(cur->_left, i - cur->_bv.rank(i), j - cur->_bv.rank(j), lower, upper, v);
        range_list_rec(cur->_right, cur->_bv.rank(i), cur->_bv.rank(j), lower, upper, v);
    }
};

}  // namespace datastructure

}  // namespace toolbox
