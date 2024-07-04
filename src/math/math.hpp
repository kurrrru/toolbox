#ifndef MATH_HPP
#define MATH_HPP

#include <cassert>
#include <vector>
#include <iostream>


namespace toolbox
{

template <typename T>
T gcd(T a, T b)
{
	assert(a != 0 || b != 0);
	while (b)
	{
		T t = a % b;
		a = b;
		b = t;
	}
	return a;
}

long long gcd(long long a, long long b)
{
	return gcd<long long>(a, b);
}

template <typename T>
T lcm(T a, T b)
{
	return a / gcd(a, b) * b;
}

long long lcm(long long a, long long b)
{
	return lcm<long long>(a, b);
}

template <typename T>
T ext_gcd(T a, T b, T &x, T &y)
{
	assert(a != 0 || b != 0);
	if (b == 0)
	{
		x = 1;
		y = 0;
		return a;
	}
	T d = ext_gcd(b, a % b, y, x);
	y -= a / b * x;
	return d;
}

long long ext_gcd(long long a, long long b, long long &x, long long &y)
{
	return ext_gcd<long long>(a, b, x, y);
}

template <typename T>
T inv_mod(T a, T mod)
{
	assert(mod > 0);
	assert(a != 0);
	T x, y;
	ext_gcd(a, mod, x, y);
	return (mod + x % mod) % mod;
}

long long inv_mod(long long a, long long mod)
{
	return inv_mod<long long>(a, mod);
}

template <typename T>
T pow(T base, T exp)
{
	assert(exp >= 0);
	T ret = 1;
	while (exp)
	{
		if (exp & 1)
			ret *= base;
		base *= base;
		exp >>= 1;
	}
	return ret;
}

long long pow(long long base, long long exp)
{
	return pow<long long>(base, exp);
}

template <typename T>
T pow_mod(T base, T exp, T mod)
{
	assert(mod > 0);
	assert(base > 0 || exp >= 0);
	if (exp < 0)
		return pow_mod(inv_mod(base, mod), -exp, mod);
	T ret = 1;
	while (exp)
	{
		if (exp & 1)
			ret = (ret * base) % mod;
		base = (base * base) % mod;
		exp >>= 1;
	}
	return ret;
}

long long pow_mod(long long base, long long exp, long long mod)
{
	return pow_mod<long long>(base, exp, mod);
}

template <typename T>
bool is_prime(T n)
{
	assert(n >= 0);
	if (n < 2)
		return false;
	if (n == 2 || n == 3)
		return true;
	if (n % 2 == 0 || n % 3 == 0)
		return false;
	for (T i = 5; i <= n / i; i += 6)
	{
		if (n % i == 0 || n % (i + 2) == 0)
			return false;
	}
	return true;
}

bool is_prime(long long n)
{
	return is_prime<long long>(n);
}

// enumration of prime numbers less than n using eratosthenes sieve
template <typename T>
std::vector<T> prime_list(T n)
{
	assert(n >= 0);
	std::vector<bool> is_prime(n, true);
	is_prime[0] = is_prime[1] = false;
	for (T i = 2; i <= n / i; i++)
	{
		if (!is_prime[i])
			continue;
		for (T j = i * i; j < n; j += i)
			is_prime[j] = false;
	}
	std::vector<T> ret;
	for (T i = 2; i < n; i++)
	{
		if (is_prime[i])
			ret.push_back(i);
	}
	return ret;
}

std::vector<long long> prime_list(long long n)
{
	return prime_list<long long>(n);
}

template <typename T>
std::vector<std::pair<T, int> > factorize(T n)
{
	assert(n > 0);
	std::vector<std::pair<T, int> > ret;
	for (T i = 2; i <= n / i; i++)
	{
		if (n % i == 0)
		{
			ret.push_back(std::make_pair(i, 0));
			while (n % i == 0)
			{
				n /= i;
				ret.back().second++;
			}
		}
		if (n == 1)
			break;
	}
	if (n != 1)
		ret.push_back(std::make_pair(n, 1));
	return ret;
}

std::vector<std::pair<long long, int> > factorize(long long n)
{
	return factorize<long long>(n);
}

template <typename T>
std::pair<T, T> chinese_remainder_theorem(T a1, T m1, T a2, T m2)
{
	assert(m1 > 0);
	assert(m2 > 0);
	T x, y;
	T d = ext_gcd(m1, m2, x, y);
	if ((a1 - a2) % d != 0)
		return std::make_pair(0, 0);
	T m = m1 / d * m2;
	T t = (a2 - a1) / d * x % (m2 / d);
	T r = (a1 + m1 * t + m) % m;
	return std::make_pair(r, m);
}

std::pair<long long, long long> chinese_remainder_theorem(long long a1, long long m1, long long a2, long long m2)
{
	return chinese_remainder_theorem<long long>(a1, m1, a2, m2);
}

} // namespace toolbox

#endif // MATH_HPP
