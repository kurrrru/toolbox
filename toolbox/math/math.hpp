#ifndef MATH_HPP
#define MATH_HPP

#include <cassert>
#include <vector>

namespace toolbox
{

namespace math
{

/**
* @brief Computes the greatest common divisor of two numbers.
* @param a The first number.
* @param b The second number.
* @return The greatest common divisor of a and b.
* @note [constraint]: a != 0 || b != 0
* @note [complexity]: O(log(min(a, b)))
*/
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

/**
* @brief Computes the least common multiple of two numbers.
* @param a The first number.
* @param b The second number.
* @return The least common multiple of a and b.
* @note [constraint]: a != 0 || b != 0
* @note [complexity]: O(log(min(a, b)))
*/
template <typename T>
T lcm(T a, T b)
{
	return a / gcd(a, b) * b;
}

/**
* @brief Computes gcd(a, b) and finds integers x and y such that ax + by = gcd(a, b).
* @param a The first number.
* @param b The second number.
* @param x The reference to the variable to store the value of x.
* @param y The reference to the variable to store the value of y.
* @return gcd(a, b).
* @note [constraint]: a != 0 || b != 0
* @note [complexity]: O(log(min(a, b)))
*/
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

/**
* @brief Computes the modular inverse of a number.
* @param a The number.
* @param mod The modulo.
* @return The modular inverse of a modulo mod.
* @note [constraint]: mod > 0, a != 0
* @note [complexity]: O(log(min(a, mod)))
*/
template <typename T>
T inv_mod(T a, T mod)
{
	assert(mod > 0);
	assert(a != 0);
	T x, y;
	ext_gcd(a, mod, x, y);
	return (mod + x % mod) % mod;
}

/**
* @brief Computes the power of a number.
* @param base The base.
* @param exp The exponent.
* @return base raised to the power of exp.
* @note [constraint]: exp >= 0
* @note [complexity]: O(log(exp))
*/
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

/**
* @brief Computes pow(base, exp) % mod.
* @param base The base.
* @param exp The exponent.
* @param mod The modulo.
* @return pow(base, exp) % mod.
* @note [constraint]: mod > 0, base > 0 || exp >= 0
* @note [complexity]: O(log(exp))
*/
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

/**
* @brief Checks if a number is prime.
* @param n The number.
* @return True if n is prime, false otherwise.
* @note [constraint]: n >= 0
* @note [complexity]: O(sqrt(n))
*/
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

/**
* @brief Generates a list of prime numbers up to n using the sieve of Eratosthenes.
* @param n The upper bound.
* @return A list of prime numbers up to n.
* @note [constraint]: n >= 0
* @note [complexity]: O(n log log n)
*/
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

/**
* @brief Computes the prime factorization of a number.
* @param n The number.
* @return The prime factorization of n.
* @note [constraint]: n > 0
* @note [complexity]: O(sqrt(n))
*/
template <typename T>
std::vector<std::pair<T, T> > factorize(T n)
{
	assert(n > 0);
	std::vector<std::pair<T, T> > ret;
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

/**
* @brief Computes the number x such that x = a1 (mod m1) and x = a2 (mod m2).
* @param a1 The first residue.
* @param m1 The first modulus.
* @param a2 The second residue.
* @param m2 The second modulus.
* @return The pair (x, m) such that x = a (mod m).
* @note [constraint]: m1 > 0, m2 > 0
* @note [complexity]: O(log(min(m1, m2)))
*/
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

/**
* @brief Computes the number x such that x = a[i] (mod m[i]) for all i.
* @param a The residues.
* @param m The moduli.
* @return The pair (x, m) such that x = a (mod m).
* @note [constraint]: a.size() == m.size(), a.size() > 0
* @note [complexity]: O(n log(min(m[i])))
*/
template <typename T>
std::pair<T, T> chinese_remainder_theorem(const std::vector<T> &a, const std::vector<T> &m)
{
	assert(a.size() == m.size());
	assert(a.size() > 0);
	int n = a.size();
	std::pair<T, T> ret = std::make_pair(a[0], m[0]);
	for (int i = 1; i < n; i++)
	{
		ret = chinese_remainder_theorem(ret.first, ret.second, a[i], m[i]);
		if (ret.second == 0)
			break;
	}
	return ret;
}

/**
* @brief Makes an array fact where fact[i] is i! % mod.
* @param n The size of the array.
* @param mod The modulo.
* @return The array fact.
* @note [constraint]: n >= 0, mod > 0
* @note [complexity]: O(n)
*/
template <typename T>
std::vector<T> factorial_mod(T n, T mod)
{
	assert(n >= 0);
	assert(mod > 0);
	std::vector<T> fact(n + 1, 1);
	for (T i = 1; i <= n; i++)
		fact[i] = (fact[i - 1] * i) % mod;
	return fact;
}

/**
* @brief Makes an array fact_inv where fact_inv[i] is the modular inverse of i! % mod.
* @param n The size of the array.
* @param mod The modulo.
* @return The array fact_inv.
* @note [constraint]: n >= 0, mod > 0
* @note [complexity]: O(n)
*/
template <typename T>
std::vector<T> factorial_inv_mod(T n, T mod)
{
	assert(n >= 0);
	assert(mod > 0);
	std::vector<T> fact_inv(n + 1, 1);
	std::vector<T> fact(n + 1, 1);
	for (T i = 1; i <= n; i++)
		fact[i] = (fact[i - 1] * i) % mod;
	fact_inv[n] = inv_mod(fact[n], mod);
	for (T i = n - 1; i >= 0; i--)
		fact_inv[i] = (fact_inv[i + 1] * (i + 1)) % mod;
	return fact_inv;
}

/**
* @brief Computes nCk % mod.
* @param n The number of elements.
* @param k The number of elements to choose.
* @param mod The modulo.
* @param fact The array fact.
* @param fact_inv The array fact_inv.
* @return nCk % mod.
* @note [constraint]: n >= 0, k >= 0, n >= k, n < fact.size(), k < fact_inv.size(), n - k < fact_inv.size()
* @note [complexity]: O(1)
*/
template <typename T>
T combination_mod(T n, T k, T mod, const std::vector<T> &fact, const std::vector<T> &fact_inv)
{
	assert(n >= 0 && k >= 0 && n >= k);
	assert(n < fact.size());
	assert(k < fact_inv.size() && n - k < fact_inv.size());
	return (fact[n] * fact_inv[k] % mod) * fact_inv[n - k] % mod;
}

/**
* @brief Computes the square root of a number.
* @param n The number.
* @return The square root of n.
* @note [constraint]: n >= 0
* @note [complexity]: O(log(n))
*/
template <typename T>
T sqrt(T n)
{
	assert(n >= 0);
	if (n == 0)
		return 0;
	T left = 0, right = n + 1;
	while (right - left > 1)
	{
		T mid = (left + right) / 2;
		if (mid <= n / mid)
			left = mid;
		else
			right = mid;
	}
	return left;
}

template <typename T>
std::vector<std::vector<T>> matrix_pow(const std::vector<std::vector<T>> &a, T exp)
{
	assert(a.size() == a[0].size());
	int n = a.size();
	std::vector<std::vector<T>> ret(n, std::vector<T>(n, 0));
	for (int i = 0; i < n; i++)
		ret[i][i] = 1;
	std::vector<std::vector<T>> base = a;
	while (exp)
	{
		if (exp & 1)
		{
			std::vector<std::vector<T>> new_ret(n, std::vector<T>(n, 0));
			for (int i = 0; i < n; i++)
			{
				for (int k = 0; k < n; k++)
				{
					for (int j = 0; j < n; j++)
					{
						new_ret[i][j] += ret[i][k] * base[k][j];
					}
				}
			}
			std::swap(ret, new_ret);
		}
		std::vector<std::vector<T>> new_base(n, std::vector<T>(n, 0));
		for (int i = 0; i < n; i++)
		{
			for (int k = 0; k < n; k++)
			{
				for (int j = 0; j < n; j++)
				{
					new_base[i][j] += base[i][k] * base[k][j];
				}
			}
		}
		std::swap(base, new_base);
		exp >>= 1;
	}
	return ret;
}

} // namespace math

} // namespace toolbox

#endif // MATH_HPP
