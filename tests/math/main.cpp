#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "toolbox/math/bit_operation.hpp"
#include "toolbox/math/math.hpp"

#include "utils/test_util.hpp"

namespace {

// ---- helpers ---------------------------------------------------------------

// ---- gcd -------------------------------------------------------------------

bool test_gcd() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::gcd(12, 8) == 4, "gcd(12,8)==4");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(7, 13) == 1, "gcd(7,13)==1");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(100, 25) == 25, "gcd(100,25)==25");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(0, 5) == 5, "gcd(0,5)==5");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(5, 0) == 5, "gcd(5,0)==5");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(1, 1) == 1, "gcd(1,1)==1");
    ok &= toolbox::test_utils::check(toolbox::math::gcd(6, 6) == 6, "gcd(6,6)==6");
    return ok;
}

// ---- lcm -------------------------------------------------------------------

bool test_lcm() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::lcm(4, 6) == 12, "lcm(4,6)==12");
    ok &= toolbox::test_utils::check(toolbox::math::lcm(3, 5) == 15, "lcm(3,5)==15");
    ok &= toolbox::test_utils::check(toolbox::math::lcm(7, 7) == 7, "lcm(7,7)==7");
    ok &= toolbox::test_utils::check(toolbox::math::lcm(1, 100) == 100, "lcm(1,100)==100");
    return ok;
}

// ---- ext_gcd ---------------------------------------------------------------

bool test_ext_gcd() {
    bool ok = true;
    // ax + by == gcd(a, b)
    auto verify = [&](long long a, long long b) -> bool {
        long long x, y;
        long long d = toolbox::math::ext_gcd(a, b, x, y);
        if (a * x + b * y != d) {
            std::cerr << "  FAIL: ext_gcd(" << a << "," << b << "): " << a << "*" << x << "+" << b
                      << "*" << y << "=" << (a * x + b * y) << " != " << d << "\n";
            return false;
        }
        if (d != toolbox::math::gcd(a, b)) {
            std::cerr << "  FAIL: ext_gcd return value != gcd\n";
            return false;
        }
        return true;
    };
    ok &= verify(3, 7);
    ok &= verify(12, 8);
    ok &= verify(35, 15);
    ok &= verify(1, 1);
    ok &= verify(100, 37);
    return ok;
}

// ---- inv_mod ---------------------------------------------------------------

bool test_inv_mod() {
    bool ok = true;
    // a * inv_mod(a, mod) % mod == 1
    auto verify = [&](long long a, long long mod) -> bool {
        long long inv = toolbox::math::inv_mod(a, mod);
        long long product = (a % mod * inv % mod + mod) % mod;
        if (product != 1) {
            std::cerr << "  FAIL: inv_mod(" << a << "," << mod << ")=" << inv << " gives product "
                      << product << "\n";
            return false;
        }
        return true;
    };
    ok &= verify(3, 7);
    ok &= verify(2, 5);
    ok &= verify(1, 1000000007LL);
    ok &= verify(123456, 1000000007LL);
    return ok;
}

// ---- pow -------------------------------------------------------------------

bool test_pow() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::pow(2LL, 10LL) == 1024LL, "2^10==1024");
    ok &= toolbox::test_utils::check(toolbox::math::pow(3LL, 0LL) == 1LL, "3^0==1");
    ok &= toolbox::test_utils::check(toolbox::math::pow(1LL, 100LL) == 1LL, "1^100==1");
    ok &= toolbox::test_utils::check(toolbox::math::pow(10LL, 3LL) == 1000LL, "10^3==1000");
    return ok;
}

// ---- pow_mod ---------------------------------------------------------------

bool test_pow_mod() {
    bool ok = true;
    const long long MOD = 1000000007LL;
    ok &= toolbox::test_utils::check(toolbox::math::pow_mod(2LL, 10LL, MOD) == 1024LL,
                                     "pow_mod(2,10,MOD)");
    ok &= toolbox::test_utils::check(toolbox::math::pow_mod(3LL, 0LL, MOD) == 1LL,
                                     "pow_mod(3,0,MOD)");
    ok &= toolbox::test_utils::check(toolbox::math::pow_mod(2LL, 30LL, MOD) == (1LL << 30) % MOD,
                                     "pow_mod(2,30,MOD)");
    ok &= toolbox::test_utils::check(toolbox::math::pow_mod(10LL, 9LL, MOD) == 1000000000LL,
                                     "pow_mod(10,9,MOD)");
    return ok;
}

// ---- is_prime --------------------------------------------------------------

bool test_is_prime() {
    bool ok = true;
    ok &= toolbox::test_utils::check(!toolbox::math::is_prime(0), "0 is not prime");
    ok &= toolbox::test_utils::check(!toolbox::math::is_prime(1), "1 is not prime");
    ok &= toolbox::test_utils::check(toolbox::math::is_prime(2), "2 is prime");
    ok &= toolbox::test_utils::check(toolbox::math::is_prime(3), "3 is prime");
    ok &= toolbox::test_utils::check(!toolbox::math::is_prime(4), "4 is not prime");
    ok &= toolbox::test_utils::check(toolbox::math::is_prime(5), "5 is prime");
    ok &= toolbox::test_utils::check(!toolbox::math::is_prime(9), "9 is not prime");
    ok &= toolbox::test_utils::check(toolbox::math::is_prime(97), "97 is prime");
    ok &= toolbox::test_utils::check(!toolbox::math::is_prime(100), "100 is not prime");
    ok &= toolbox::test_utils::check(toolbox::math::is_prime(1000000007), "1e9+7 is prime");
    return ok;
}

// ---- prime_list ------------------------------------------------------------

bool test_prime_list() {
    bool ok = true;
    std::vector<int> primes = toolbox::math::prime_list(30);
    std::vector<int> expected = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    ok &= toolbox::test_utils::check(primes == expected, "prime_list(30)");

    std::vector<int> p0 = toolbox::math::prime_list(0);
    ok &= toolbox::test_utils::check(p0.empty(), "prime_list(0) is empty");

    std::vector<int> p1 = toolbox::math::prime_list(1);
    ok &= toolbox::test_utils::check(p1.empty(), "prime_list(1) is empty");

    std::vector<int> p2 = toolbox::math::prime_list(2);
    ok &= toolbox::test_utils::check(p2.empty(), "prime_list(2) is empty (2 not included)");

    std::vector<int> p3 = toolbox::math::prime_list(3);
    ok &= toolbox::test_utils::check(p3.size() == 1 && p3[0] == 2, "prime_list(3)=={2}");
    return ok;
}

// ---- factorize -------------------------------------------------------------

bool test_factorize() {
    bool ok = true;
    // Verify factorize by reconstructing the product
    auto verify = [&](long long n) -> bool {
        auto factors = toolbox::math::factorize(n);
        long long product = 1;
        for (auto &pf : factors) {
            for (long long k = 0; k < pf.second; k++) {
                product *= pf.first;
            }
        }
        if (product != n) {
            std::cerr << "  FAIL: factorize(" << n << ") product mismatch\n";
            return false;
        }
        // factors should be sorted (each prime appears once)
        for (std::size_t i = 1; i < factors.size(); i++) {
            if (factors[i].first <= factors[i - 1].first) {
                std::cerr << "  FAIL: factorize(" << n << ") not sorted\n";
                return false;
            }
        }
        return true;
    };
    ok &= verify(1);
    ok &= verify(2);
    ok &= verify(12);
    ok &= verify(360);
    ok &= verify(1000000007LL);
    ok &= verify(1000000007LL * 2);
    return ok;
}

// ---- chinese_remainder_theorem ---------------------------------------------

bool test_crt() {
    bool ok = true;
    // x = a1 (mod m1), x = a2 (mod m2)
    auto verify2 = [&](long long a1, long long m1, long long a2, long long m2) -> bool {
        auto res = toolbox::math::chinese_remainder_theorem(a1, m1, a2, m2);
        if (res.second == 0) {
            return true;  // no solution, skip
        }
        long long x = res.first;
        if (x % m1 != a1 % m1 || x % m2 != a2 % m2) {
            std::cerr << "  FAIL: CRT2(" << a1 << "," << m1 << "," << a2 << "," << m2 << ")=" << x
                      << " does not satisfy congruences\n";
            return false;
        }
        return true;
    };
    ok &= verify2(2, 3, 3, 5);
    ok &= verify2(1, 4, 3, 7);
    ok &= verify2(0, 6, 0, 4);  // gcd(6,4)=2, consistent
    ok &= verify2(1, 6, 0, 4);  // gcd(6,4)=2, inconsistent -> (0,0)
    {
        auto res = toolbox::math::chinese_remainder_theorem(1LL, 6LL, 0LL, 4LL);
        ok &= toolbox::test_utils::check(res.second == 0, "CRT inconsistent returns (0,0)");
    }

    // Multi-modulus CRT
    std::vector<long long> a = {2, 3, 2};
    std::vector<long long> m = {3, 5, 7};
    auto res = toolbox::math::chinese_remainder_theorem(a, m);
    ok &= toolbox::test_utils::check(res.first % 3 == 2 && res.first % 5 == 3 && res.first % 7 == 2,
                                     "CRT multi: x=23 (mod 105)");
    return ok;
}

// ---- factorial_mod / factorial_inv_mod / combination_mod -------------------

bool test_combination() {
    bool ok = true;
    const long long MOD = 1000000007LL;
    const long long N = 20;
    auto fact = toolbox::math::factorial_mod(N, MOD);
    auto fact_inv = toolbox::math::factorial_inv_mod(N, MOD);

    // fact[i] * fact_inv[i] % MOD == 1
    for (long long i = 1; i <= N; i++) {
        long long product = fact[i] * fact_inv[i] % MOD;
        if (product != 1) {
            std::cerr << "  FAIL: fact[" << i << "] * fact_inv[" << i << "] != 1\n";
            ok = false;
        }
    }

    // C(n,k) values
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(10LL, 3LL, MOD, fact, fact_inv) == 120, "C(10,3)==120");
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(10LL, 0LL, MOD, fact, fact_inv) == 1, "C(10,0)==1");
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(10LL, 10LL, MOD, fact, fact_inv) == 1, "C(10,10)==1");
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(5LL, 6LL, MOD, fact, fact_inv) == 0, "C(5,6)==0");
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(5LL, -1LL, MOD, fact, fact_inv) == 0, "C(5,-1)==0");
    ok &= toolbox::test_utils::check(
        toolbox::math::combination_mod(20LL, 10LL, MOD, fact, fact_inv) == 184756,
        "C(20,10)==184756");
    return ok;
}

// ---- sqrt (integer) --------------------------------------------------------

bool test_sqrt() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(0LL) == 0LL, "sqrt(0)==0");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(1LL) == 1LL, "sqrt(1)==1");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(4LL) == 2LL, "sqrt(4)==2");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(9LL) == 3LL, "sqrt(9)==3");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(8LL) == 2LL, "sqrt(8)==2 (floor)");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(10LL) == 3LL, "sqrt(10)==3 (floor)");
    ok &= toolbox::test_utils::check(toolbox::math::sqrt(1000000000000LL) == 1000000LL,
                                     "sqrt(1e12)==1e6");
    return ok;
}

// ---- matrix_pow ------------------------------------------------------------

bool test_matrix_pow() {
    bool ok = true;
    // Identity matrix ^ n == identity matrix
    {
        std::vector<std::vector<long long>> I = {{1, 0}, {0, 1}};
        auto res = toolbox::math::matrix_pow(I, 10LL);
        ok &= toolbox::test_utils::check(res == I, "I^10 == I");
    }
    // Fibonacci via matrix exponentiation: [[1,1],[1,0]]^n
    {
        std::vector<std::vector<long long>> F = {{1, 1}, {1, 0}};
        auto res = toolbox::math::matrix_pow(F, 10LL);
        // F^10[0][1] == fib(10) == 55
        ok &= toolbox::test_utils::check(res[0][1] == 55, "fib(10) via matrix_pow");
    }
    {
        std::vector<std::vector<long long>> F = {{1, 1}, {1, 0}};
        auto res = toolbox::math::matrix_pow(F, 1LL);
        ok &= toolbox::test_utils::check(res == F, "F^1 == F");
    }
    {
        std::vector<std::vector<long long>> F = {{1, 1}, {1, 0}};
        auto res = toolbox::math::matrix_pow(F, 0LL);
        std::vector<std::vector<long long>> I = {{1, 0}, {0, 1}};
        ok &= toolbox::test_utils::check(res == I, "F^0 == I");
    }
    return ok;
}

// ---- popcount --------------------------------------------------------------

bool test_popcount() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::popcount(0) == 0, "popcount(0)==0");
    ok &= toolbox::test_utils::check(toolbox::math::popcount(1) == 1, "popcount(1)==1");
    ok &= toolbox::test_utils::check(toolbox::math::popcount(0xFF) == 8, "popcount(0xFF)==8");
    ok &= toolbox::test_utils::check(toolbox::math::popcount(0x7FFFFFFF) == 31,
                                     "popcount(INT_MAX)==31");

    ok &= toolbox::test_utils::check(toolbox::math::popcount(0LL) == 0, "popcount(0LL)==0");
    ok &= toolbox::test_utils::check(toolbox::math::popcount(1LL) == 1, "popcount(1LL)==1");
    ok &= toolbox::test_utils::check(
        toolbox::math::popcount(static_cast<long long>(0xFFFFFFFFFFFFFFFFLL)) == 64,
        "popcount(UINT64_MAX)==64");
    return ok;
}

// ---- clz -------------------------------------------------------------------

bool test_clz() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::clz(0) == 32, "clz(0)==32");
    ok &= toolbox::test_utils::check(toolbox::math::clz(1) == 31, "clz(1)==31");
    ok &= toolbox::test_utils::check(toolbox::math::clz(2) == 30, "clz(2)==30");
    ok &= toolbox::test_utils::check(toolbox::math::clz(static_cast<int>(0x80000000)) == 0,
                                     "clz(MSB_INT)==0");

    ok &= toolbox::test_utils::check(toolbox::math::clz(0LL) == 64, "clz(0LL)==64");
    ok &= toolbox::test_utils::check(toolbox::math::clz(1LL) == 63, "clz(1LL)==63");
    return ok;
}

// ---- bit_width -------------------------------------------------------------

bool test_bit_width() {
    bool ok = true;
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(0) == 0, "bit_width(0)==0");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(1) == 1, "bit_width(1)==1");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(2) == 2, "bit_width(2)==2");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(3) == 2, "bit_width(3)==2");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(4) == 3, "bit_width(4)==3");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(7) == 3, "bit_width(7)==3");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(8) == 4, "bit_width(8)==4");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(0x7FFFFFFF) == 31,
                                     "bit_width(INT_MAX)==31");

    ok &= toolbox::test_utils::check(toolbox::math::bit_width(0LL) == 0, "bit_width(0LL)==0");
    ok &= toolbox::test_utils::check(toolbox::math::bit_width(1LL) == 1, "bit_width(1LL)==1");
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"gcd", test_gcd},
        {"lcm", test_lcm},
        {"ext_gcd", test_ext_gcd},
        {"inv_mod", test_inv_mod},
        {"pow", test_pow},
        {"pow_mod", test_pow_mod},
        {"is_prime", test_is_prime},
        {"prime_list", test_prime_list},
        {"factorize", test_factorize},
        {"crt", test_crt},
        {"combination", test_combination},
        {"sqrt", test_sqrt},
        {"matrix_pow", test_matrix_pow},
        {"popcount", test_popcount},
        {"clz", test_clz},
        {"bit_width", test_bit_width},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
