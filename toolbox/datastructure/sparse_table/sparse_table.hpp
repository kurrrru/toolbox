#ifndef SPAESE_TABLE_HPP
#define SPAESE_TABLE_HPP

#include <vector>

namespace toolbox
{

namespace datastructure
{

template <typename T, T (*op)(const T, const T)>
struct sparse_table
{
	sparse_table() = default;
	sparse_table(sparse_table &) = default;
	sparse_table(sparse_table &&) = default;
	sparse_table &operator=(sparse_table &) = default;
	sparse_table &operator=(sparse_table &&) = default;
	~sparse_table() = default;

	sparse_table(std::vector<T> &arr)
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

	T staticRMQ(int i, int j)
	{
		int k = _log_table[j - i];
		return (op(_table[i][k], _table[j - (1 << k)][k]));
	}

private:
	int _n, _log_n;
	std::vector<int> _log_table;
	std::vector<std::vector<T>> _table;
};

} // namespace datastructure

} // namespace toolbox

#endif // SPAESE_TABLE_HPP
