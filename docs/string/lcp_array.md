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
- `sa`: 長さ $n$ の接尾辞配列（**センチネルなし**。`suffixarray(s)` の出力をそのまま渡す）
- 戻り値: 長さ $n$ の LCP 配列。`lcp[0]` は `0`（先頭には比較対象がないため）

## 使用例

```cpp
#include "toolbox/string/suffixarray.hpp"
#include "toolbox/string/lcp_array.hpp"

std::string s = "banana";
auto sa  = toolbox::string::suffixarray(s);
auto lcp = toolbox::string::lcp_array(s, sa);
// sa  = {5, 3, 1, 0, 4, 2}
// lcp[0] = 0（先頭）
// lcp[1] = LCP("a", "ana")     = 1
// lcp[2] = LCP("ana", "anana") = 3
// lcp[3] = LCP("anana", "banana") = 0
// lcp[4] = LCP("banana", "na")    = 0
// lcp[5] = LCP("na", "nana")      = 2
```

## 実装上の注意

- `sa` の長さは `s.size()` と一致していなければならない（`assert` で検査）。
- LCP 配列の用途例: 最長重複部分文字列（`max(lcp)`）、異なる部分文字列の個数（$\binom{n}{2} + n - \sum \text{lcp}$）、文字列検索の高速化。

## 参考文献
