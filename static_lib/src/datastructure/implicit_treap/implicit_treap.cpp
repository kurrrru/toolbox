#include "../../../toolbox.h"
#include <iostream>
#include <cassert>
#include <vector>

namespace toolbox
{

namespace datastructure
{

namespace monoid_implicit_treap
{
long long monoid_range_add_range_min::query_op(long long a, long long b)
{
	return (std::min(a, b));
}

long long monoid_range_add_range_min::query_id()
{
	return (2e18);
}

long long monoid_range_add_range_min::update_op(long long a, long long b)
{
	return (a + b);
}

long long monoid_range_add_range_min::update_id()
{
	return (0);
}

long long monoid_range_add_range_min::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : a + b);
}

long long monoid_range_add_range_max::query_op(long long a, long long b)
{
	return (std::max(a, b));
}

long long monoid_range_add_range_max::query_id()
{
	return (-2e18);
}

long long monoid_range_add_range_max::update_op(long long a, long long b)
{
	return (a + b);
}

long long monoid_range_add_range_max::update_id()
{
	return (0);
}

long long monoid_range_add_range_max::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : a + b);
}

long long monoid_range_add_range_sum::query_op(long long a, long long b)
{
	return (a + b);
}

long long monoid_range_add_range_sum::query_id()
{
	return (0);
}

long long monoid_range_add_range_sum::update_op(long long a, long long b)
{
	return (a + b);
}

long long monoid_range_add_range_sum::update_id()
{
	return (0);
}

long long monoid_range_add_range_sum::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : a + b * len);
}

long long monoid_range_update_range_min::query_op(long long a, long long b)
{
	return (std::min(a, b));
}

long long monoid_range_update_range_min::query_id()
{
	return (2e18);
}

long long monoid_range_update_range_min::update_op(long long a, long long b)
{
	return (b);
}

long long monoid_range_update_range_min::update_id()
{
	return (2e18);
}

long long monoid_range_update_range_min::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : b);
}

long long monoid_range_update_range_max::query_op(long long a, long long b)
{
	return (std::max(a, b));
}

long long monoid_range_update_range_max::query_id()
{
	return (-2e18);
}

long long monoid_range_update_range_max::update_op(long long a, long long b)
{
	return (b);
}

long long monoid_range_update_range_max::update_id()
{
	return (0);
}

long long monoid_range_update_range_max::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : b);
}

long long monoid_range_update_range_sum::query_op(long long a, long long b)
{
	return (a + b);
}

long long monoid_range_update_range_sum::query_id()
{
	return (0);
}

long long monoid_range_update_range_sum::update_op(long long a, long long b)
{
	return (b);
}

long long monoid_range_update_range_sum::update_id()
{
	return (2e18);
}

long long monoid_range_update_range_sum::apply(long long a, long long b, int len)
{
	return (b == update_id() ? a : b * len);
}

} // namespace monoid_implicit_treap

template <typename S, class operations, unsigned int seed>
implicit_treap<S, operations, seed>::implicit_treap() : _rnd(seed) {}

template <typename S, class operations, unsigned int seed>
implicit_treap<S, operations, seed>::implicit_treap(std::vector<S> &vec) : _rnd(seed)
{
	for (size_t i = 0; i < vec.size(); i++)
		insert(i, vec[i]);
}

template <typename S, class operations, unsigned int seed>
implicit_treap<S, operations, seed>::implicit_treap(int size, S val) : _rnd(seed)
{
	assert(size >= 0);
	for (int i = 0; i < size; i++)
		insert(i, val);
}

template <typename S, class operations, unsigned int seed>
S implicit_treap<S, operations, seed>::query(int l, int r)
{
	assert(0 <= l && l <= r && r <= size());
	return (query(_root, l, r));
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::update(int l, int r, S val)
{
	assert(0 <= l && l <= r && r <= size());
	update(_root, l, r, val);
}

template <typename S, class operations, unsigned int seed>
S implicit_treap<S, operations, seed>::operator[](int pos)
{
	assert(0 <= pos && pos < size());
	node *left;
	node *right;
	node *mid;
	split(_root, pos, left, right);
	split(right, 1, mid, right);
	S res = mid->_value;
	merge(right, mid, right);
	merge(_root, left, right);
	return (res);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::insert(int pos, S val)
{
	insert(_root, pos, new node(val, _rnd.random()));
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::erase(int pos)
{
	assert(0 <= pos && pos < size());
	erase(_root, pos);
}

template <typename S, class operations, unsigned int seed>
int implicit_treap<S, operations, seed>::size() const
{
	if (!_root)
		return (0);
	return (cnt(_root));
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::reverse(int l, int r)
{
	assert(0 <= l && l <= r && r <= size());
	reverse(_root, l, r);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::rotate(int l, int m, int r)
{
	assert(0 <= l && l <= m && m <= r && r <= size());
	rotate(_root, l, m, r);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::clear()
{
	clear(_root);
	_root = nullptr;
}

template <typename S, class operations, unsigned int seed>
implicit_treap<S, operations, seed>::xorshift::xorshift(unsigned int _seed) : _state(_seed) {}

template <typename S, class operations, unsigned int seed>
unsigned int implicit_treap<S, operations, seed>::xorshift::random()
{
	_state ^= _state << 13;
	_state ^= _state >> 17;
	_state ^= _state << 5;
	return (_state);
}

template <typename S, class operations, unsigned int seed>
implicit_treap<S, operations, seed>::node::node(S val, int priority)
{
	_value = val;
	_acc = operations::query_id();
	_lazy = operations::update_id();
	_priority = priority;
	_cnt = 1;
	_rev = false;
	_child[0] = nullptr;
	_child[1] = nullptr;
}

template <typename S, class operations, unsigned int seed>
int implicit_treap<S, operations, seed>::cnt(node *t) const
{
	if (!t)
		return (0);
	return (t->_cnt);
}

template <typename S, class operations, unsigned int seed>
S implicit_treap<S, operations, seed>::acc(node *t) const
{
	if (!t)
		return (operations::query_id());
	return (t->_acc);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::update(node *t)
{
	if (!t)
		return;
	t->_cnt = 1 + cnt(t->_child[0]) + cnt(t->_child[1]);
	t->_acc = operations::query_op(operations::query_op(acc(t->_child[0]), t->_value), acc(t->_child[1]));
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::pushup(node *t)
{
	if (!t)
		return;
	update(t);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::pushdown(node *t)
{
	if (!t)
		return;
	if (t->_rev)
	{
		std::swap(t->_child[0], t->_child[1]);
		if (t->_child[0])
			t->_child[0]->_rev ^= 1;
		if (t->_child[1])
			t->_child[1]->_rev ^= 1;
		t->_rev = false;
	}
	if (t->_lazy != operations::update_id())
	{
		if (t->_child[0])
		{
			t->_child[0]->_lazy = operations::update_op(t->_child[0]->_lazy, t->_lazy);
			t->_child[0]->_acc = operations::apply(t->_child[0]->_acc, t->_lazy, cnt(t->_child[0]));
		}
		if (t->_child[1])
		{
			t->_child[1]->_lazy = operations::update_op(t->_child[1]->_lazy, t->_lazy);
			t->_child[1]->_acc = operations::apply(t->_child[1]->_acc, t->_lazy, cnt(t->_child[1]));
		}
		t->_value = operations::apply(t->_value, t->_lazy, 1);
		t->_lazy = operations::update_id();
	}
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::split(node *t, int key, node *&l, node *&r)
{
	if (!t)
		l = r = nullptr;
	else
	{
		pushdown(t);
		int cur_key = cnt(t->_child[0]);
		if (key > cur_key)
		{
			l = t;
			split(t->_child[1], key - cur_key - 1, l->_child[1], r);
		}
		else
		{
			r = t;
			split(t->_child[0], key, l, r->_child[0]);
		}
		pushup(t);
	}
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::merge(node *&t, node *l, node *r)
{
	pushdown(l);
	pushdown(r);
	if (!l)
		t = r;
	else if (!r)
		t = l;
	else if (l->_priority > r->_priority)
	{
		t = l;
		merge(t->_child[1], l->_child[1], r);
	}
	else
	{
		t = r;
		merge(t->_child[0], l, r->_child[0]);
	}
	pushup(t);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::insert(node *&t, int key, node *item)
{
	node *left;
	node *right;
	split(t, key, left, right);
	merge(left, left, item);
	merge(t, left, right);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::erase(node *&t, int key)
{
	node *left;
	node *right;
	node *mid;
	split(t, key, left, right);
	split(right, 1, mid, right);
	merge(t, left, right);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::update(node *&t, int l, int r, S val)
{
	if (l >= r)
		return;
	node *left;
	node *right;
	node *mid;
	split(t, l, left, right);
	split(right, r - l, mid, right);
	mid->_lazy = operations::update_op(mid->_lazy, val);
	mid->_acc = operations::apply(mid->_acc, val, cnt(mid));
	merge(right, mid, right);
	merge(t, left, right);
}

template <typename S, class operations, unsigned int seed>
S implicit_treap<S, operations, seed>::query(node *t, int l, int r)
{
	node *left;
	node *right;
	node *mid;
	split(t, l, left, right);
	split(right, r - l, mid, right);
	S res = operations::query_id();
	if (mid)
		res = mid->_acc;
	merge(right, mid, right);
	merge(t, left, right);
	return (res);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::reverse(node *t, int l, int r)
{
	if (l >= r)
		return;
	node *left;
	node *right;
	node *mid;
	split(t, l, left, right);
	split(right, r - l, mid, right);
	mid->_rev ^= 1;
	merge(right, mid, right);
	merge(t, left, right);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::rotate(node *t, int l, int m, int r)
{
	if (l >= m || m >= r)
		return;
	reverse(t, l, r);
	reverse(t, l, l + r - m);
	reverse(t, l + r - m, r);
}

template <typename S, class operations, unsigned int seed>
void implicit_treap<S, operations, seed>::clear(node *t)
{
	if (!t)
		return;
	clear(t->_child[0]);
	clear(t->_child[1]);
	delete t;
}

} // namespace datastructure

} // namespace toolbox
