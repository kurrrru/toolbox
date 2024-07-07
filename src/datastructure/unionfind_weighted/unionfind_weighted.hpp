#ifndef UNIONFIND_WEIGHTED_HPP
#define UNIONFIND_WEIGHTED_HPP

#include <vector>
#include <cassert>

namespace toolbox
{

namespace datastructure
{

/**
* @brief A data structure for union-find operations with weights.
*/
struct unionfind_weighted
{
	unionfind_weighted() = default;
	unionfind_weighted(const unionfind_weighted&) = default;
	unionfind_weighted(unionfind_weighted&&) = default;
	unionfind_weighted& operator=(const unionfind_weighted&) = default;
	unionfind_weighted& operator=(unionfind_weighted&&) = default;
	~unionfind_weighted() = default;

	/**
	* @brief Constructs a union-find data structure with n elements.
	* @param n The number of elements.
	* @note [constraint]: n > 0
	* @note [complexity]: O(n)
	*/
	unionfind_weighted(int n)
	{
		assert(n > 0);
		_n = n;
		_p.resize(n, -1);
		_r.resize(n, 1);
		_diff_weight.resize(n, 0);
	}

	/**
	* @brief Finds the representative of the set that the specified element belongs to.
	* @param x The element to find.
	* @return The representative of the set that the element belongs to.
	* @note [constraint]: 0 <= x < _n
	* @note [complexity]: O(alpha(n)), where alpha(n) is the inverse Ackermann function.
	*/
	int find(int x)
	{
		assert(0 <= x && x < _n);
		if (_p[x] == -1)
			return x;
		int r = find(_p[x]);
		_diff_weight[x] += _diff_weight[_p[x]];
		return (_p[x] = r);
	}

	/**
	* @brief Unites the sets that the specified elements belong to.
	* @param x The first element.
	* @param y The second element.
	* @param w The weight of the edge between the two elements.
	* @return True if the sets were successfully united, false otherwise.
	* @note [constraint]: 0 <= x < _n, 0 <= y < _n
	* @note [complexity]: O(alpha(n)), where alpha(n) is the inverse Ackermann function.
	*/
	bool unite(int x, int y, int w)
	{
		assert(0 <= x && x < _n);
		assert(0 <= y && y < _n);
		w += weight(x);
		w -= weight(y);
		x = find(x);
		y = find(y);
		if (x == y)
			return false;
		if (_r[x] < _r[y])
		{
			std::swap(x, y);
			w = -w;
		}
		if (_r[x] == _r[y])
			_r[x]++;
		_p[y] = x;
		_diff_weight[y] = w;
		return true;
	}

	/**
	* @brief Checks if the specified elements belong to the same set.
	* @param x The first element.
	* @param y The second element.
	* @return True if the elements belong to the same set, false otherwise.
	* @note [constraint]: 0 <= x < _n, 0 <= y < _n
	* @note [complexity]: O(alpha(n)), where alpha(n) is the inverse Ackermann function.
	*/
	bool same(int x, int y)
	{
		assert(0 <= x && x < _n);
		assert(0 <= y && y < _n);
		return (find(x) == find(y));
	}

	/**
	* @brief Returns the weight of the specified element.
	* @param x The element.
	* @return The weight of the element.
	* @note [constraint]: 0 <= x < _n
	* @note [complexity]: O(alpha(n)), where alpha(n) is the inverse Ackermann function.
	*/
	int weight(int x)
	{
		assert(0 <= x && x < _n);
		find(x);
		return _diff_weight[x];
	}

	/**
	* @brief Returns the difference between the weights of the specified elements.
	* @param x The first element.
	* @param y The second element.
	* @return The difference between the weights of the elements.
	* @note [constraint]: 0 <= x < _n, 0 <= y < _n
	* @note [complexity]: O(alpha(n)), where alpha(n) is the inverse Ackermann function.
	*/
	int diff(int x, int y)
	{
		assert(0 <= x && x < _n);
		assert(0 <= y && y < _n);
		const int inf = (int)(1e9);
		if (!same(x, y))
			return inf;
		return (weight(y) - weight(x));
	}

private:
	int _n;
	std::vector<int> _p, _r, _diff_weight;
};

} // namespace datastructure

} // namespace toolbox

#endif // UNIONFIND_WEIGHTED_HPP
