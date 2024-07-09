#ifndef FM_INDEX_HPP
#define FM_INDEX_HPP

#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include "suffixarray.hpp"

namespace toolbox
{

namespace string
{

struct fm_index
{
	fm_index() = default;
	fm_index(const fm_index &) = default;
	fm_index(fm_index &&) = default;
	fm_index &operator=(const fm_index &) = default;
	fm_index &operator=(fm_index &&) = default;
	~fm_index() = default;
	fm_index(const std::string &s) {build(s);}
	fm_index(const std::string &s, const std::string &order) {build(s, order);}

	int count(const std::string &p)
	{
		int m = p.size();
		int l = 0, r = _n - 1;
		for (int i = m - 1; i >= 0; i--)
		{
			if (_order[(int)p[i]] == -1)
				return 0;
			if (l - 1 == -1)
				l = _c[_order[(int)p[i]]];
			else
				l = _c[_order[(int)p[i]]] + _occ[_order[(int)p[i]]][l - 1];
			r = _c[_order[(int)p[i]]] + _occ[_order[(int)p[i]]][r] - 1;
		}
		return r - l + 1;
	}

	std::vector<int> locate(const std::string &p)
	{
		int m = p.size();
		int l = 0, r = _n - 1;
		for (int i = m - 1; i >= 0; i--)
		{
			if (_order[(int)p[i]] == -1)
				return std::vector<int>();
			if (l - 1 == -1)
				l = _c[_order[(int)p[i]]];
			else
				l = _c[_order[(int)p[i]]] + _occ[_order[(int)p[i]]][l - 1];
			r = _c[_order[(int)p[i]]] + _occ[_order[(int)p[i]]][r] - 1;
		}
		std::vector<int> res;
		for (int i = l; i <= r; i++)
			res.push_back(_sa[i]);
		return res;
	}

private:
	int _n;
	int _num_c;
	std::vector<int> _order;
	std::string _s;
	std::vector<int> _sa;
	std::string _bwt;
	std::vector<std::vector<int>> _occ;
	std::vector<int> _c;

	void build(const std::string &s)
	{
		_n = s.size();
		_s = s;
		std::vector<int> cnt(256, 0);
		for (int i = 0; i < _n; i++)
			cnt[(int)_s[i]]++;
		assert(cnt[(int)'$'] == 0 || (cnt[(int)'$'] == 1 && _s[_n - 1] == '$'));
		if (cnt[(int)'$'] == 0)
		{
			_s += '$';
			_n++;
		}
		_order.assign(256, -1);
		_order['$'] = 0;
		int rank = 0;
		for (int i = 1; i < 256; i++)
			if (cnt[i] > 0 && (char)i != '$')
				_order[i] = ++rank;
		_num_c = rank;
		build_sa_bwt();
		build_occ();
		build_c();
	}

	void build(const std::string &s, const std::string &order)
	{
		_n = s.size();
		_s = s;
		assert(find(_s.begin(), _s.end() - 1, '$') == _s.end() - 1);
		if (_s.back() != '$')
		{
			_s += '$';
			_n++;
		}
		_order.assign(256, -1);
		_order[(int)'$'] = 0;
		int rank = 0;
		for (int i = 0; i < order.size(); i++)
		{
			if (_order[(int)order[i]] != -1)
				continue;
			_order[(int)order[i]] = ++rank;
		}
		_num_c = rank;
		build_sa_bwt();
		build_occ();
		build_c();
	}

	void build_sa_bwt()
	{
		std::vector<int> s_int(_n);
		for (int i = 0; i < _n; i++)
			s_int[i] = _order[(int)_s[i]];
		_sa = toolbox::string::suffixarray(s_int, _num_c);
		_bwt.resize(_n);
		for (int i = 0; i < _n; i++)
			_bwt[i] = _s[(_sa[i] + _n - 1) % _n];
	}

	void build_occ()
	{
		_occ.assign(_num_c + 1, std::vector<int>(_n, 0));
		_occ[_order[(int)_bwt[0]]][0]++;
		for (int i = 1; i < _n; i++)
		{
			for (int j = 0; j <= _num_c; j++)
				_occ[j][i] = _occ[j][i - 1];
			_occ[_order[(int)_bwt[i]]][i]++;
		}
	}

	void build_c()
	{
		_c.assign(_num_c + 1, 0);
		for (int i = 0; i < _n; i++)
			_c[_order[(int)_bwt[i]]]++;
		for (int i = 1; i <= _num_c; i++)
			_c[i] += _c[i - 1];
		for (int i = _num_c; i > 0; i--)
			_c[i] = _c[i - 1];
		_c[0] = 0;
	}
};

} // namespace string

} // namespace toolbox

#endif
