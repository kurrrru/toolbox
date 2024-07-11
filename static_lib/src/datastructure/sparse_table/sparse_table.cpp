#include <vector>
#include "../../../toolbox.h"

namespace toolbox
{

namespace datastructure
{

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op>::sparse_table() = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op>::sparse_table(sparse_table &) = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op>::sparse_table(sparse_table &&) = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op> &sparse_table<T, op>::operator=(sparse_table &) = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op> &sparse_table<T, op>::operator=(sparse_table &&) = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op>::~sparse_table() = default;

template <typename T, T (*op)(const T, const T)>
sparse_table<T, op>::sparse_table(std::vector<T> &arr)
{
	_n = arr.size();
	_log_table.assign(_n + 1, 0);
	for (int i = 2; i <= _n; i++)
		_log_table[i] = _log_table[i / 2] + 1;
	_log_n = _log_table[_n];
	_table.assign(_n, std::vector<T>(_log_n + 1, 0));
	for (int i = 0; i < _n; i++)
		_table[i][0] = arr[i];
	for (int j = 1; j <= _log_n; j++)
		for (int i = 0; i + (1 << j) <= _n; i++)
			_table[i][j] = op(_table[i][j - 1], _table[i + (1 << (j - 1))][j - 1]);
}

template <typename T, T (*op)(const T, const T)>
T sparse_table<T, op>::staticRMQ(int i, int j)
{
	int k = _log_table[j - i];
	return (op(_table[i][k], _table[j - (1 << k)][k]));
}

} // datastructure

} // toolbox