#include "../../toolbox.h"

namespace toolbox
{

namespace math
{

/**
* @brief Counts the number of set bits in an integer.
* @param x The integer for which to count the set bits.
* @return The number of set bits in the integer.
* @note This function uses the built-in `__builtin_popcount` function to count the number of set bits in the given integer.
*/
int popcount(int x)
{
	return __builtin_popcount(x);
}

/**
* @brief Counts the number of set bits in a long long integer.
* @param x The long long integer for which to count the set bits.
* @return The number of set bits in the long long integer.
* @note This function uses the built-in `__builtin_popcountll` function to count the number of set bits in the given long long integer.
*/
int popcount(long long x)
{
	return __builtin_popcountll(x);
}

/**
* @brief Counts the number of leading zeros in an integer.
* @param x The integer for which to count the leading zeros.
* @return The number of leading zeros in the integer.
* @note This function uses the built-in `__builtin_clz` function to count the number of leading zeros in the given integer.
*/
int clz(int x)
{
	if (x == 0) return 32;
	return __builtin_clz(x);
}

/**
* @brief Counts the number of leading zeros in a long long integer.
* @param x The long long integer for which to count the leading zeros.
* @return The number of leading zeros in the long long integer.
* @note This function uses the built-in `__builtin_clzll` function to count the number of leading zeros in the given long long integer.
*/
int clz(long long x)
{
	if (x == 0) return 64;
	return __builtin_clzll(x);
}

/**
* @brief Counts the number of bits required to represent an integer.
* @param x The integer for which to calculate the number of bits.
* @return The number of bits required to represent the integer.
* @note This function calculates the number of bits required to represent the given integer by subtracting the number of leading zeros from 32.
*/
int bit_width(int x)
{
	if (x == 0) return 0;
	return 32 - clz(x);
}

/**
* @brief Counts the number of bits required to represent a long long integer.
* @param x The long long integer for which to calculate the number of bits.
* @return The number of bits required to represent the long long integer.
* @note This function calculates the number of bits required to represent the given long long integer by subtracting the number of leading zeros from 64.
*/
int bit_width(long long x)
{
	if (x == 0) return 0;
	return 64 - clz(x);
}

} // namespace math

} // namespace toolbox
