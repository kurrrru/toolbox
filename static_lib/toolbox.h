#ifndef TOOLBOX_H
#define TOOLBOX_H

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

namespace rna_2d
{

int nussinov_dp(const std::string &seq, std::vector<std::vector<int>> &dp);
std::string nussinov_traceback(const std::string &seq, const std::vector<std::vector<int>> &dp);
std::string nussinov_all(const std::string &seq);

} // namespace rna_2d

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

struct unionfind
{
	unionfind();
	unionfind(const unionfind &);
	unionfind(unionfind &&);
	unionfind &operator=(const unionfind &);
	unionfind &operator=(unionfind &&);
	~unionfind();
	unionfind(int n);

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
	unionfind_weighted(const unionfind_weighted &);
	unionfind_weighted(unionfind_weighted &&);
	unionfind_weighted &operator=(const unionfind_weighted &);
	unionfind_weighted &operator=(unionfind_weighted &&);
	~unionfind_weighted();
	unionfind_weighted(int n);

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

namespace monoid_implicit_treap
{

struct monoid_range_update_range_sum
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

struct monoid_range_update_range_max
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

struct monoid_range_update_range_min
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

struct monoid_range_add_range_sum
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

struct monoid_range_add_range_max
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

struct monoid_range_add_range_min
{
	static long long query_op(long long a, long long b);
	static long long query_id();
	static long long update_op(long long a, long long b);
	static long long update_id();
	static long long apply(long long a, long long b, int len);
};

} // namespace monoid_implicit_treap

template <typename S, class operations, unsigned int seed = 1>
struct implicit_treap
{
	implicit_treap();
	implicit_treap(const implicit_treap &);
	implicit_treap(implicit_treap &&);
	implicit_treap &operator=(const implicit_treap &);
	implicit_treap &operator=(implicit_treap &&);
	~implicit_treap();
	implicit_treap(std::vector<S> &vec);
	implicit_treap(int size, S val);

	S query(int l, int r);
	void update(int l, int r, S val);
	S operator[](int pos);
	void insert(int pos, S val);
	void erase(int pos);
	int size() const;
	void reverse(int l, int r);
	void rotate(int l, int m, int r);
	void clear();

private:

	struct xorshift
	{
		unsigned int _state;
		xorshift(unsigned int _seed);
		unsigned int random();
	} _rnd;

	struct node
	{
		S _value;
		S _acc;
		S _lazy;
		int _priority;
		int _cnt;
		bool _rev;
		node *_child[2];
		node(S value, int priority);
	} *_root;

	int cnt(node *t) const;
	S acc(node *t) const;
	void update(node *t);
	void pushup(node *t);
	void pushdown(node *t);
	void split(node *t, int key, node *&l, node *&r);
	void merge(node *&t, node *l, node *r);
	void insert(node *&t, int key, node *item);
	void erase(node *&t, int key);
	void update(node *&t, int l, int r, S val);
	S query(node *t, int l, int r);
	void reverse(node *t, int l, int r);
	void rotate(node *t, int l, int m, int r);
	void clear(node *t);
};

} // namespace datastructure

namespace graph
{

template <typename T>
T diameter_tree(const std::vector<std::vector<std::pair<long long, T> > > &adjacent);

long long diameter_tree(const std::vector<std::vector<std::pair<long long, long long> > > &adjacent);

template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> dijkstra(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost);

std::vector<long long> dijkstra(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost);

template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> bellman_ford(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost);

std::vector<long long> bellman_ford(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost);

template <typename Vertex, typename Cost, Cost INF, Cost Zero>
bool negative_cycle(const Vertex s, const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost);

bool negative_cycle(const long long s, const std::vector<std::vector<std::pair<long long, long long>>> &cost);

template <typename Vertex>
std::vector<Vertex> topological(const std::vector<std::vector<Vertex>> &edge);

std::vector<long long> topological(const std::vector<std::vector<long long>> &edge);

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