# 数学ユーティリティ

整数論・組合せ論・線形代数に関する基本的な数学関数群。`toolbox/math/math.hpp` に含まれる。

## 関数一覧

### gcd / lcm

```cpp
template <typename T> T toolbox::math::gcd(T a, T b);
template <typename T> T toolbox::math::lcm(T a, T b);
```

最大公約数・最小公倍数を求める。ユークリッドの互除法による $O(\log \min(a, b))$。  
制約: $a \neq 0$ または $b \neq 0$。

---

### ext_gcd（拡張ユークリッド互除法）

```cpp
template <typename T> T toolbox::math::ext_gcd(T a, T b, T &x, T &y);
```

$ax + by = \gcd(a, b)$ を満たす整数 $x, y$ を求め、$\gcd(a, b)$ を返す。$O(\log \min(a, b))$。

---

### inv_mod（モジュラ逆元）

```cpp
template <typename T> T toolbox::math::inv_mod(T a, T mod);
```

$a \cdot \text{inv} \equiv 1 \pmod{mod}$ を満たす $\text{inv}$ を返す。$O(\log \min(a, \text{mod}))$。  
制約: $mod > 0$、$a \neq 0$、$\gcd(a, mod) = 1$（暗黙）。

---

### pow / pow_mod

```cpp
template <typename T> T toolbox::math::pow(T base, T exp);
template <typename T> T toolbox::math::pow_mod(T base, T exp, T mod);
```

繰り返し二乗法による累乗・累乗剰余。$O(\log \text{exp})$。  
`pow_mod` は `exp < 0` の場合 `inv_mod(base, mod)` を用いて計算する。

---

### is_prime

```cpp
template <typename T> bool toolbox::math::is_prime(T n);
```

$O(\sqrt{n})$ の試し割りによる素数判定。

---

### prime_list（エラトステネスの篩）

```cpp
template <typename T> std::vector<T> toolbox::math::prime_list(T n);
```

$[2, n)$ に含まれる素数の一覧を返す。$O(n \log \log n)$。

---

### factorize（素因数分解）

```cpp
template <typename T> std::vector<std::pair<T, T>> toolbox::math::factorize(T n);
```

$n$ を素因数分解し、`{素因数, 指数}` のペア列を昇順で返す。$O(\sqrt{n})$。  
制約: $n > 0$。

---

### chinese_remainder_theorem（中国剰余定理）

```cpp
// 2連立
template <typename T>
std::pair<T,T> toolbox::math::chinese_remainder_theorem(T a1, T m1, T a2, T m2);

// 多連立
template <typename T>
std::pair<T,T> toolbox::math::chinese_remainder_theorem(
    const std::vector<T> &a, const std::vector<T> &m);
```

$x \equiv a_i \pmod{m_i}$ を満たす $x$ と法 $m$ のペア $(x, m)$ を返す。解なしの場合は $(0, 0)$。  
$O(n \log \min(m_i))$（$n$ は連立方程式の数）。

---

### factorial_mod / factorial_inv_mod / combination_mod

```cpp
template <typename T>
std::vector<T> toolbox::math::factorial_mod(T n, T mod);

template <typename T>
std::vector<T> toolbox::math::factorial_inv_mod(T n, T mod);

template <typename T>
T toolbox::math::combination_mod(T n, T k, T mod,
    const std::vector<T> &fact, const std::vector<T> &fact_inv);
```

$i!\ \%\ \text{mod}$ の配列、その逆元配列を前計算し、$\binom{n}{k}\ \%\ \text{mod}$ を $O(1)$ で求める。  
前計算: $O(n)$。`combination_mod` の制約: $0 \le k \le n < \text{fact.size()}$。  
$k < 0$ のとき $0$、$k = 0$ のとき $1$、$k > n$ のとき $0$ を返す。

---

### sqrt（整数平方根）

```cpp
template <typename T> T toolbox::math::sqrt(T n);
```

$\lfloor \sqrt{n} \rfloor$ を二分探索で求める。$O(\log n)$。制約: $n \geq 0$。

---

### matrix_pow（行列累乗）

```cpp
template <typename T>
std::vector<std::vector<T>> toolbox::math::matrix_pow(
    const std::vector<std::vector<T>> &a, T exp);
```

正方行列 $a$ の $\text{exp}$ 乗を繰り返し二乗法で求める。$O(n^3 \log \text{exp})$（$n$: 行列サイズ）。  
制約: $a$ は正方行列。$\text{exp} = 0$ のとき単位行列を返す。

## 使用例

```cpp
#include "toolbox/math/math.hpp"

toolbox::math::gcd(12LL, 8LL);    // 4
toolbox::math::lcm(4LL, 6LL);     // 12
toolbox::math::pow_mod(2LL, 10LL, 1000000007LL); // 1024

// 組合せ数の前計算
const long long MOD = 1e9 + 7, N = 1000;
auto fact     = toolbox::math::factorial_mod(N, MOD);
auto fact_inv = toolbox::math::factorial_inv_mod(N, MOD);
toolbox::math::combination_mod(10LL, 3LL, MOD, fact, fact_inv); // 120

// 行列累乗（フィボナッチ数列）
std::vector<std::vector<long long>> F = {{1,1},{1,0}};
auto res = toolbox::math::matrix_pow(F, 10LL);
// res[0][1] == fib(10) == 55
```

## 参考文献
