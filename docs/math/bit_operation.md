# ビット演算ユーティリティ

整数のビット操作に関する基本関数群。`toolbox/math/bit_operation.hpp` に含まれる。コンパイラ組み込み関数（`__builtin_popcount` 系）をラップし、`int` と `long long` の両方に対してオーバーロードを提供する。

## 関数一覧

### popcount（ビット数のカウント）

```cpp
int toolbox::math::popcount(int x);
int toolbox::math::popcount(long long x);
```

整数 $x$ の立っているビット数（1の個数）を返す。`__builtin_popcount` / `__builtin_popcountll` を使用。

| 入力 | 戻り値 |
|---|---|
| `0` | `0` |
| `1` | `1` |
| `0xFF` | `8` |
| `0xFFFFFFFFFFFFFFFF` | `64` |

---

### clz（先頭ゼロビット数のカウント）

```cpp
int toolbox::math::clz(int x);
int toolbox::math::clz(long long x);
```

整数 $x$ の最上位ビットから続く $0$ の個数を返す。`__builtin_clz` / `__builtin_clzll` を使用。  
$x = 0$ のとき、`int` 版は `32`、`long long` 版は `64` を返す（未定義動作を回避するための特例）。

| 入力 | 戻り値（int版） |
|---|---|
| `0` | `32` |
| `1` | `31` |
| `2` | `30` |
| `0x80000000` | `0` |

---

### bit_width（表現に必要なビット幅）

```cpp
int toolbox::math::bit_width(int x);
int toolbox::math::bit_width(long long x);
```

$\lfloor \log_2 x \rfloor + 1$、すなわち $x$ を2進数で表現するのに必要なビット数を返す。$x = 0$ のとき `0` を返す。

$$
\text{bit\_width}(x) = 32 - \text{clz}(x) \quad (x > 0)
$$

| 入力 | 戻り値 |
|---|---|
| `0` | `0` |
| `1` | `1` |
| `2` | `2` |
| `3` | `2` |
| `4` | `3` |
| `7` | `3` |
| `8` | `4` |

## 使用例

```cpp
#include "toolbox/math/bit_operation.hpp"

toolbox::math::popcount(0b10110110); // 5
toolbox::math::clz(1);              // 31 (int版)
toolbox::math::bit_width(8);        // 4
toolbox::math::bit_width(0LL);      // 0
```

## 実装上の注意

- `clz(0)` は `__builtin_clz(0)` の未定義動作を避けるために個別にハンドリングしている。
- C++20 では `std::popcount`・`std::countl_zero`・`std::bit_width` として標準ライブラリに同等の機能が追加された。

## 参考文献
