#ifndef TREAP_HPP
#define TREAP_HPP

#include <iostream>
#include <cassert>
#include <vector>

namespace toolbox
{

namespace datastructure
{

/**
* @brief An implementation of a treap.
* @tparam Key The type of the key.
* @tparam Value The type of the value.
* @tparam seed The seed for the random number generator. (default is 1)
* @note This implementation supports the following operations:
* insert, erase, is_in, operator[], clear, size, to_vector, to_value_vector.
*/
template<typename Key, typename Value, unsigned int seed = 1>
struct treap
{
	treap() : _rnd(seed), _root(nullptr), _size(0) {}
	treap(treap &) = delete;
	treap(treap &&) = delete;
	treap &operator=(treap &) = delete;
	treap &operator=(treap &&) = delete;
	~treap()
	{
		clear();
	}

	treap(std::vector<std::pair<Key, Value>> &vec) : _rnd(seed), _root(nullptr), _size(0)
	{
		for (auto &p : vec)
			insert(p.first, p.second);
	}

	treap(std::vector<Value> &vec) : _rnd(seed), _root(nullptr), _size(0)
	{
		for (size_t i = 0; i < vec.size(); i++)
			insert(i, vec[i]);
	}

	treap(Key key_size) : _rnd(seed), _root(nullptr), _size(0)
	{
		for (Key i = 0; i < key_size; i++)
			insert(i, Value());
	}

	Value &operator[](Key key)
	{
		node *it = search(_root, key);
		assert(it != nullptr);
		return (it->_val);
	}

	bool is_in(Key key) const
	{
		node *it = search(_root, key);
		return (it);
	}

	void insert(Key key, Value val)
	{
		_root = insert(_root, new node(key, _rnd.random(), val));
		_size++;
	}

	bool erase(Key key)
	{
		if (!erase(_root, key))
			return (false);
		_size--;
		return (true);
	}

	friend std::ostream &operator<<(std::ostream &stream, const treap &tree)
	{
		stream << "each node is shown as (left child [key, value] right child)" << std::endl;
		stream << tree._root;
		return (stream);
	}

	void clear()
	{
		clear(_root);
		_root = nullptr;
		_size = 0;
	}

	size_t size() const
	{
		return (_size);
	}

	std::vector<std::pair<Key, Value>> to_vector() const
	{
		std::vector<std::pair<Key, Value>> vec;
		to_vector(_root, vec);
		return (vec);
	}

	std::vector<Value> to_value_vector() const
	{
		std::vector<std::pair<Key, Value>> vec = to_vector();
		std::vector<Value> res;
		for (auto &p : vec)
			res.push_back(p.second);
		return (res);
	}

private:

	struct xorshift
	{
		unsigned int _state;
		xorshift(unsigned int _seed): _state(_seed) {}
		unsigned int random()
		{
			_state ^= _state << 13;
			_state ^= _state >> 17;
			_state ^= _state << 5;
			return (_state);
		}
	} _rnd;

	struct node
	{
		node(Key key, unsigned int priority, Value val) : _key(key), _priority(priority), _val(val)
		{
			_child[0] = nullptr;
			_child[1] = nullptr;
		}

		friend std::ostream &operator<<(std::ostream &stream, const node *t)
		{
			if (!t)
				return (stream << "");
			else
				return (stream << "(" << t->_child[0] << "[" << t->_key << ", " << t->_val << "]" << t->_child[1] << ")");
		}

		Key _key;
		node *_child[2];
		Value _val;
		unsigned int _priority;
	} *_root;

	size_t _size;

	node *search(node *&t, Key key) const
	{
		if (!t)
			return (nullptr);
		if (t->_key == key)
			return (t);
		return (search(t->_child[key > t->_key], key));
	}

	node *insert(node *t, node *item)
	{
		if (!t)
			return (item);
		assert(t->_key != item->_key);
		if (item->_priority < t->_priority)
		{
			t->_child[item->_key > t->_key] = insert(t->_child[item->_key > t->_key], item);
			return (t);
		}
		split(t, item->_key, item->_child[0], item->_child[1]);
		return (item);
	}

	bool erase(node *&t, Key key)
	{
		if (!t)
			return (false);
		if (key == t->_key)
		{
			merge(t, t->_child[0], t->_child[1]);
			return (true);
		}
		return (erase(t->_child[key > t->_key], key));
	}

	void split(node *t, Key key, node *&l, node *&r)
	{
		if (!t)
			l = r = nullptr;
		else if (key > t->_key)
		{
			l = t;
			split(t->_child[1], key, l->_child[1], r);
		}
		else
		{
			r = t;
			split(t->_child[0], key, l, r->_child[0]);
		}
	}

	void merge(node *&t, node *l, node *r)
	{
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
	}

	void clear(node *t)
	{
		if (!t)
			return;
		clear(t->_child[0]);
		clear(t->_child[1]);
		delete t;
	}

	void to_vector(node *t, std::vector<std::pair<Key, Value>> &vec) const
	{
		if (!t)
			return;
		to_vector(t->_child[0], vec);
		vec.push_back(std::make_pair(t->_key, t->_val));
		to_vector(t->_child[1], vec);
	}
};

} // namespace datastructure

} // namespace toolbox

#endif // TREAP_HPP