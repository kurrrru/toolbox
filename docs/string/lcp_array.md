# LCP 配列（Longest Common Prefix Array）

接尾辞配列（SA）において、辞書順に隣接する2つの接尾辞の最長共通接頭辞の長さを格納した配列。Kasai のアルゴリズムにより $O(n)$ で構築する。

## LCP 配列の定義

$$
\text{lcp}[i] = \text{LCP}(S[\text{sa}[i]:],\; S[\text{sa}[i-1]:])
$$

つまり、SA において $i$ 番目と $i-1$ 番目の接尾辞の共通接頭辞の長さ。

## アルゴリズム（Kasai法）

LCP 配列を直接構築するのではなく、元の文字列上の順序で各接尾辞を処理する。隣接するLCP値が高々1ずつしか減少しないという性質を利用して全体 $O(n)$ を達成する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(n)$ | $O(n)$ |

## インターフェース

```cpp
#include "toolbox/string/lcp_array.hpp"

std::vector<int> toolbox::string::lcp_array(
    const std::string &s,
    const std::vector<int> &sa
);
```

- `s`: 元の文字列（長さ $n$）
- `sa`: 長さ $n+1$ の接尾辞配列（**センチネル付き**。先頭が $n$、残りが `suffixarray(s)` の出力）
- 戻り値: 長さ $n$ の LCP 配列。`lcp[0]` はセンチネルとの LCP なので `0`

### 注意: SA のフォーマット

本実装の `lcp_array` は、**センチネル（位置 $n$）を先頭に付加した $n+1$ 要素の SA** を要求する。

```cpp
// suffixarray(s) は n 要素の SA を返す
auto sa_n   = toolbox::string::suffixarray(s);

// lcp_array 用に n+1 要素の SA を構築
std::vector<int> sa_np1;
sa_np1.push_back((int)s.size()); // センチネル位置
sa_np1.insert(sa_np1.end(), sa_n.begin(), sa_n.end());

auto lcp = toolbox::string::lcp_array(s, sa_np1);
// lcp[i+1] が sa_n[i] と sa_n[i+1] のLCP長に対応
```

## 使用例

```cpp
#include "toolbox/string/suffixarray.hpp"
#include "toolbox/string/lcp_array.hpp"

std::string s = "banana";
auto sa_n   = toolbox::string::suffixarray(s);

std::vector<int> sa_np1;
sa_np1.push_back((int)s.size());
sa_np1.insert(sa_np1.end(), sa_n.begin(), sa_n.end());

auto lcp = toolbox::string::lcp_array(s, sa_np1);
// sa_n  = {5, 3, 1, 0, 4, 2}
// lcp[1..5] = 接尾辞間のLCP
// lcp[1] = LCP("a", "ana")     = 1
// lcp[2] = LCP("ana", "anana") = 3
// lcp[3] = LCP("anana", "banana") = 0
// lcp[4] = LCP("banana", "na")    = 0
// lcp[5] = LCP("na", "nana")      = 2
```

## 実装上の注意

- `sa` の長さは `s.size() + 1` でなければならない。`suffixarray` の出力をそのまま渡すと配列外アクセスが発生する。
- LCP 配列の用途例: 最長重複部分文字列（`max(lcp)`）、異なる部分文字列の個数（$\binom{n}{2} + n - \sum \text{lcp}$）、文字列検索の高速化。

## 参考文献
