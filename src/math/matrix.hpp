#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cassert>

namespace toolbox
{

namespace math
{

template <typename T>
struct matrix
{
	matrix() = default;
	matrix(const matrix &) = default;
	matrix(matrix &&) = default;
	matrix &operator=(const matrix &) = default;
	matrix &operator=(matrix &&) = default;
	~matrix() = default;

	matrix(const std::vector<std::vector<T>> &data)
	{
		_row = data.size();
		_col = data[0].size();
		for (int i = 0; i < _row; i++)
			assert(data[i].size() == _col);
		_data = data;
	}
	matrix(const int row, const int col)
	{
		assert(row > 0 && col > 0);
		_row = row;
		_col = col;
		_data.resize(row, std::vector<T>(col));
	}
	matrix(const int row, const int col, const T &val)
	{
		assert(row > 0 && col > 0);
		_row = row;
		_col = col;
		_data.resize(row, std::vector<T>(col, val));
	}
	matrix(const int size, const std::string &type)
	{
		assert(size > 0);
		_row = size;
		_col = size;
		if (type == "identity")
		{
			_data.resize(size, std::vector<T>(size));
			for (int i = 0; i < size; i++)
				_data[i][i] = 1;
		}
		else if (type == "zero")
		{
			_data.resize(size, std::vector<T>(size));
		}
		else if (type == "one")
		{
			_data.resize(size, std::vector<T>(size, 1));
		}
		else
		{
			assert(false);
		}
	}

	const T &operator()(int i, int j) const
	{
		assert(0 <= i && i < _row);
		assert(0 <= j && j < _col);
		return (_data[i][j]);
	}
	T &operator()(int i, int j)
	{
		assert(0 <= i && i < _row);
		assert(0 <= j && j < _col);
		return (_data[i][j]);
	}

	matrix operator+(const matrix &mat2) const
	{
		assert(_row == mat2._row);
		assert(_col == mat2._col);
		matrix res(_row, _col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(i, j) = _data[i][j] + mat2(i, j);
		return (res);
	}

	matrix operator-(const matrix &mat2) const
	{
		assert(_row == mat2._row);
		assert(_col == mat2._col);
		matrix res(_row, _col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(i, j) = _data[i][j] - mat2(i, j);
		return (res);
	}

	matrix operator*(const matrix &mat2) const
	{
		assert(_col == mat2._row);
		matrix res(_row, mat2._col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < mat2._col; j++)
				for (int k = 0; k < _col; k++)
					res(i, j) += _data[i][k] * mat2(k, j);
		return (res);
	}

	matrix operator*(const T &multiplier) const
	{
		matrix res(_row, _col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(i, j) = _data[i][j] * multiplier;
		return (res);
	}

	matrix operator/(const T &divisor) const
	{
		matrix res(_row, _col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(i, j) = _data[i][j] / divisor;
		return (res);
	}

	matrix operator-() const
	{
		matrix res(_row, _col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(i, j) = -_data[i][j];
		return (res);
	}

	matrix &operator+=(const matrix &mat2)
	{
		assert(_row == mat2._row);
		assert(_col == mat2._col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				_data[i][j] += mat2(i, j);
		return (*this);
	}

	matrix &operator-=(const matrix &mat2)
	{
		assert(_row == mat2._row);
		assert(_col == mat2._col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				_data[i][j] -= mat2(i, j);
		return (*this);
	}

	matrix &operator*=(const matrix &mat2)
	{
		assert(_col == mat2._row);
		matrix res(_row, mat2._col);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < mat2._col; j++)
				for (int k = 0; k < _col; k++)
					res(i, j) += _data[i][k] * mat2(k, j);
		*this = res;
		return (*this);
	}

	matrix &operator*=(const T &multiplier)
	{
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				_data[i][j] *= multiplier;
		return (*this);
	}

	matrix &operator/=(const T &divisor)
	{
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				_data[i][j] /= divisor;
		return (*this);
	}

	matrix matrix_pow(int n) const
	{
		assert(_row == _col);
		matrix res(_row, _col, "identity");
		matrix a = *this;
		while (n)
		{
			if (n & 1)
				res *= a;
			a *= a;
			n >>= 1;
		}
		return (res);
	}

	matrix transpose() const
	{
		matrix res(_col, _row);
		for (int i = 0; i < _row; i++)
			for (int j = 0; j < _col; j++)
				res(j, i) = _data[i][j];
		return (res);
	}

	matrix submatrix(int r, int c, int row, int col) const
	{
		assert(0 <= r && r < _row);
		assert(0 <= c && c < _col);
		assert(0 <= row && r + row <= _row);
		assert(0 <= col && c + col <= _col);
		matrix res(row, col);
		for (int i = 0; i < row; i++)
			for (int j = 0; j < col; j++)
				res(i, j) = _data[r + i][c + j];
		return (res);
	}

	matrix &swap_row(int r1, int r2)
	{
		assert(0 <= r1 && r1 < _row);
		assert(0 <= r2 && r2 < _row);
		std::swap(_data[r1], _data[r2]);
		return (*this);
	}

	matrix &swap_column(int c1, int c2)
	{
		assert(0 <= c1 && c1 < _col);
		assert(0 <= c2 && c2 < _col);
		for (int i = 0; i < _row; i++)
			std::swap(_data[i][c1], _data[i][c2]);
		return (*this);
	}

	matrix &row_add(int r1, int r2, T multiplier)
	{
		assert(0 <= r1 && r1 < _row);
		assert(0 <= r2 && r2 < _row);
		for (int i = 0; i < _col; i++)
			_data[r1][i] += multiplier * _data[r2][i];
		return (*this);
	}

	matrix &column_add(int c1, int c2, T multiplier)
	{
		assert(0 <= c1 && c1 < _col);
		assert(0 <= c2 && c2 < _col);
		for (int i = 0; i < _row; i++)
			_data[i][c1] += multiplier * _data[i][c2];
		return (*this);
	}

	matrix &row_multiply(int r, T multiplier)
	{
		assert(0 <= r && r < _row);
		for (int i = 0; i < _col; i++)
			_data[r][i] *= multiplier;
		return (*this);
	}

	matrix &column_multiply(int c, T multiplier)
	{
		assert(0 <= c && c < _col);
		for (int i = 0; i < _row; i++)
			_data[i][c] *= multiplier;
		return (*this);
	}

private:
	std::vector<std::vector<T>> _data;
	int _row = 0;
	int _col = 0;
};

} // namespace math

} // namespace toolbox

#endif // MATRIX_HPP