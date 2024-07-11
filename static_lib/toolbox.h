#ifndef TOOLBOX_HPP
#define TOOLBOX_HPP

#include <vector>
#include <string>

namespace toolbox
{

namespace alignment
{

int needleman_wunsch_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int> > &dp, int a, int x, int g);
void needleman_wunsch_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int> > &dp,
	std::string &s1_aligned, std::string &s2_aligned, int a, int x, int g);
int needleman_wunsch_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int a, int x, int g);
int		diff_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int> > &M, int x, int g);
void	diff_traceback(const std::string &s1, const std::string &s2, std::vector<std::vector<int> > &M,
	std::string &s1_aligned, std::string &s2_aligned, int x, int g);
int		diff_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int x, int g);
int needleman_wunsch_gotoh_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int> > &M, std::vector<std::vector<int> > &I, std::vector<std::vector<int> > &D,
	int a, int x, int o, int e);
void needleman_wunsch_gotoh_traceback(const std::string &s1, const std::string &s2,
	const std::vector<std::vector<int> > &M, const std::vector<std::vector<int> > &I,
	const std::vector<std::vector<int> > &D, std::string &s1_aligned, std::string &s2_aligned,
	int a, int x, int o, int e);
int needleman_wunsch_gotoh_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int a, int x, int o, int e);
int wavefront_dp(const std::string &s1, const std::string &s2,
	std::vector<std::vector<int> > &M, std::vector<std::vector<int> > &I, std::vector<std::vector<int> > &D,
	int x, int o, int e);
void wavefront_traceback(const std::string &s1, const std::string &s2,
	const std::vector<std::vector<int> > &M, const std::vector<std::vector<int> > &I, const std::vector<std::vector<int> > &D,
	std::string &s1_aligned, std::string &s2_aligned, int x, int o, int e);
int wavefront_all(const std::string &s1, const std::string &s2,
	std::string &s1_aligned, std::string &s2_aligned, int x, int o, int e);

} // namespace alignment

namespace datastructure
{

template <typename T, T (*op)(const T, const T)>
struct sparse_table
{
	sparse_table();
	sparse_table(sparse_table &);
	sparse_table(sparse_table &&);
	sparse_table &operator=(sparse_table &);
	sparse_table &operator=(sparse_table &&);
	~sparse_table();

	sparse_table(std::vector<T> &arr);
	T staticRMQ(int i, int j);

private:
	int _n, _log_n;
	std::vector<int> _log_table;
	std::vector<std::vector<T>> _table;
};

template <typename T>
struct sparse_table_min
{
	sparse_table_min();
	sparse_table_min(sparse_table_min &st);
	sparse_table_min(sparse_table_min &&st);
	sparse_table_min &operator=(sparse_table_min &st);
	sparse_table_min &operator=(sparse_table_min &&st);
	~sparse_table_min();

	sparse_table_min(std::vector<T> &arr);
	T staticRMQ(int i, int j);

private:
	sparse_table<T, min> st;

	T min(const T a, const T b);
};

template <typename T>
struct sparse_table_max
{
	sparse_table_max();
	sparse_table_max(sparse_table_max &st);
	sparse_table_max(sparse_table_max &&st);
	sparse_table_max &operator=(sparse_table_max &st);
	sparse_table_max &operator=(sparse_table_max &&st);
	~sparse_table_max();

	sparse_table_max(std::vector<T> &arr);
	T staticRMQ(int i, int j);

private:
	sparse_table<T, max> st;

	T max(const T a, const T b);
};

struct unionfind
{
	unionfind();
	unionfind(int n);
	~unionfind();

	int find(int x);
	bool unite(int x, int y);
	bool same(int x, int y);

private:
	int _n;
	std::vector<int> _p;
	std::vector<int> _r;
};

struct unionfind_weighted
{
	unionfind_weighted();
	unionfind_weighted(int n);
	~unionfind_weighted();

	int find(int x);
	bool unite(int x, int y, int w);
	bool same(int x, int y);
	int weight(int x);
	int diff(int x, int y);

private:
	int _n;
	std::vector<int> _p;
	std::vector<int> _r;
	std::vector<int> _diff_weight;
};

} // namespace datastructure

namespace graph
{

template <typename T>
T diameter_tree(const std::vector<std::vector<std::pair<long long, T> > > &adjacent);

long long diameter_tree(const std::vector<std::vector<std::pair<long long, long long> > > &adjacent);

} // namespace graph

namespace math
{

int popcount(int x);
int popcount(long long x);
int clz(int x);
int clz(long long x);
int bit_width(int x);
int bit_width(long long x);

template <typename T>
T gcd(T a, T b);
long long gcd(long long a, long long b);

template <typename T>
T lcm(T a, T b);
long long lcm(long long a, long long b);

template <typename T>
T ext_gcd(T a, T b, T &x, T &y);
long long ext_gcd(long long a, long long b, long long &x, long long &y);

template <typename T>
T inv_mod(T a, T mod);
long long inv_mod(long long a, long long mod);

template <typename T>
T pow(T base, T exp);
long long pow(long long base, long long exp);

template <typename T>
T pow_mod(T base, T exp, T mod);
long long pow_mod(long long base, long long exp, long long mod);

template <typename T>
bool is_prime(T n);
bool is_prime(long long n);

template <typename T>
std::vector<T> prime_list(T n);
std::vector<long long> prime_list(long long n);

template <typename T>
std::vector<std::pair<T, T> > factorize(T n);
std::vector<std::pair<long long, long long> > factorize(long long n);

template <typename T>
std::pair<T, T> chinese_remainder_theorem(T a1, T m1, T a2, T m2);
std::pair<long long, long long> chinese_remainder_theorem(long long a1, long long m1, long long a2, long long m2);

template <typename T>
std::pair<T, T> chinese_remainder_theorem(const std::vector<T> &a, const std::vector<T> &m);
std::pair<long long, long long> chinese_remainder_theorem(const std::vector<long long> &a, const std::vector<long long> &m);

template <typename T>
std::vector<T> factorial_mod(T n, T mod);
std::vector<long long> factorial_mod(long long n, long long mod);

template <typename T>
std::vector<T> factorial_inv_mod(T n, T mod);
std::vector<long long> factorial_inv_mod(long long n, long long mod);

template <typename T>
std::vector<T> combination_mod(T n, T k, T mod);
std::vector<long long> combination_mod(long long n, long long k, long long mod);

template <typename T>
T sqrt(T n);
long long sqrt(long long n);

} // namespace math

namespace string
{

struct fm_index
{
	fm_index();
	fm_index(const fm_index &);
	fm_index(fm_index &&);
	fm_index &operator=(const fm_index &);
	fm_index &operator=(fm_index &&);
	~fm_index();
	fm_index(const std::string &s);
	fm_index(const std::string &s, const std::string &order);

	int count(const std::string &p);
	std::vector<int> locate(const std::string &p);

private:
	int _n;
	int _num_c;
	std::vector<int> _order;
	std::string _s;
	std::vector<int> _sa;
	std::string _bwt;
	std::vector<std::vector<int> > _occ;
	std::vector<int> _c;

	void build(const std::string &s);
	void build(const std::string &s, const std::string &order);
	void build_sa_bwt();
	void build_occ();
	void build_c();
};

std::vector<int> lcp_array(const std::string &s, const std::vector<int> &sa);
std::vector<int> suffixarray(const std::vector<int> &s, const int max_s);
std::vector<int> suffixarray(const std::vector<int> &s);
std::vector<int> suffixarray(const std::string &str);

} // namespace string

} // namespace toolbox

#endif