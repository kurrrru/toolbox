#ifndef UNIONFIND_HPP
#define UNIONFIND_HPP

#include <vector>
#include <cassert>

/**
* @brief A data structure for efficient union-find operations.
*/
struct unionfind {
	/**
	* @brief Constructs a union-find data structure with the specified number of elements.
	* @param n The number of elements.
	* @note [constraint]: n > 0
	*/
	unionfind(int n)
	{
		assert(n > 0);
		_n = n;
		_p.resize(n, -1);
		_r.resize(n, 1);
	}

	/**
	* @brief Finds the representative of the set that the specified element belongs to.
	* @param x The element to find.
	* @return The representative of the set that the element belongs to.
	* @note [constraint]: 0 <= x < _n
	*/
	int find(int x)
	{
		assert(0 <= x && x < _n);
		if (_p[x] == -1)
			return x;
		else
			return (_p[x] = find(_p[x]));
	}

	/**
	* @brief Unites the sets that the specified elements belong to.
	* @param x The first element.
	* @param y The second element.
	* @return True if the sets were successfully united, false otherwise.
	* @note [constraint]: 0 <= x < _n, 0 <= y < _n
	*/
	bool unite(int x, int y)
	{
		assert(0 <= x && x < _n);
		assert(0 <= y && y < _n);
		x = find(x);
		y = find(y);
		if (x == y)
			return false;
		if (_r[x] > _r[y])
			std::swap(x, y);
		if (_r[x] == _r[y])
			_r[y]++;
		_p[x] = y;
		return true;
	}

	/**
	* @brief Checks if the specified elements belong to the same set.
	* @param x The first element.
	* @param y The second element.
	* @return True if the elements belong to the same set, false otherwise.
	* @note [constraint]: 0 <= x < _n, 0 <= y < _n
	*/
	bool same(int x, int y)
	{
		assert(0 <= x && x < _n);
		return (find(x) == find(y));
	}

private:
	std::vector<int> _p; // parent array
	std::vector<int> _r; // rank array
	int _n; // number of elements
};

#endif // UNIONFIND_HPP