# Sparse Table

静的な配列に対する冪等な二項演算（min・max・gcd など）の区間クエリを $O(1)$ で答えるデータ構造。構築後に配列を変更することはできない。

## アルゴリズム

長さ $n$ の配列に対して、$\text{table}[i][j]$ を「インデックス $i$ から始まる長さ $2^j$ の区間に対する演算結果」として前計算する。

$$
\text{table}[i][j] = \mathrm{op}(\text{table}[i][j-1],\; \text{table}[i + 2^{j-1}][j-1])
$$

クエリ $[l, r)$ に対しては、$k = \lfloor \log_2(r - l) \rfloor$ として重複を許した2区間で計算する。

$$
\text{staticRMQ}(l, r) = \mathrm{op}(\text{table}[l][k],\; \text{table}[r - 2^k][k])
$$

演算 $\mathrm{op}$ が冪等（$\mathrm{op}(x, x) = x$）であれば、2区間が重複していても結果は正しい。

## 計算量

| 操作 | 時間計算量 | 空間計算量 |
|---|---|---|
| 構築 | $O(n \log n)$ | $O(n \log n)$ |
| クエリ `staticRMQ(l, r)` | $O(1)$ | $O(1)$ |

## インターフェース

```cpp
#include "toolbox/datastructure/sparse_table/sparse_table.hpp"

toolbox::datastructure::sparse_table<T, op> st(arr); // arr: std::vector<T>
T st.staticRMQ(int l, int r); // [l, r) の演算結果を返す
```

### テンプレートパラメータ

| パラメータ | 説明 |
|---|---|
| `T` | 要素の型 |
| `op` | 冪等な二項演算関数 `T (*)(const T, const T)` |

## 使用例

```cpp
#include "toolbox/datastructure/sparse_table/sparse_table.hpp"

int op_min(const int a, const int b) { return std::min(a, b); }

std::vector<int> arr = {3, 1, 4, 1, 5, 9, 2, 6};
toolbox::datastructure::sparse_table<int, op_min> st(arr);
st.staticRMQ(0, 8); // 1
st.staticRMQ(4, 8); // 2
```

## 実装上の注意

- 演算 `op` は冪等性（$\mathrm{op}(x, x) = x$）を満たす必要がある。加算など冪等でない演算には使用できない。
- 構築後の配列の変更には対応していない（静的データ構造）。

## 参考文献
