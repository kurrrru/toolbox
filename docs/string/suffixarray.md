# 接尾辞配列（Suffix Array）

文字列 $S$ の全接尾辞を辞書順にソートした際のインデックス列。SA-IS アルゴリズムにより $O(n)$ で構築する。文字列検索・LCP 配列の構築・最長重複部分文字列の発見などに用いられる。

## アルゴリズム

### SA-IS（Suffix Array Induced Sorting）

接尾辞を S-type（辞書順で次の接尾辞以下）と L-type（より大きい）に分類し、LMS（Leftmost S-type）接尾辞を再帰的に整列することで線形時間を達成する。

主なステップ:

1. 各文字を S-type / L-type に分類（`sais_classify_sl`）
2. LMS 部分文字列の位置を特定（`sais_search_lms`）
3. Induced Sorting で仮の SA を構築（`sais_induce`）
4. LMS 部分文字列の順序を解決し再帰呼び出し（`sais_resolve`）

小さい入力（$n < 100$）では $O(n \log n)$ の倍加法（Doubling）にフォールバックする。

### 倍加法（Doubling）

各接尾辞の先頭 $1, 2, 4, \ldots, 2^k$ 文字のランクを順次精緻化して SA を構築する。

## 計算量

| アルゴリズム | 時間 | 空間 |
|---|---|---|
| SA-IS | $O(n)$ | $O(n)$ |
| 倍加法 | $O(n \log n)$ | $O(n)$ |

## インターフェース

```cpp
#include "toolbox/string/suffixarray.hpp"

// 文字列から構築（SA-IS、O(n)）
std::vector<int> toolbox::string::suffixarray(const std::string &str);

// 整数列から構築（文字の最大値を自動検出）
std::vector<int> toolbox::string::suffixarray(const std::vector<int> &s);

// 整数列から構築（文字の最大値を指定）
std::vector<int> toolbox::string::suffixarray(const std::vector<int> &s, int max_s);
```

- 戻り値: 長さ $n$ の接尾辞配列。`sa[i]` は辞書順 $i$ 番目の接尾辞の開始位置

## 使用例

```cpp
#include "toolbox/string/suffixarray.hpp"

std::string s = "banana";
auto sa = toolbox::string::suffixarray(s);
// sa = {5, 3, 1, 0, 4, 2}
// 対応する接尾辞（辞書順）:
//   sa[0]=5 → "a"
//   sa[1]=3 → "ana"
//   sa[2]=1 → "anana"
//   sa[3]=0 → "banana"
//   sa[4]=4 → "na"
//   sa[5]=2 → "nana"

// パターン検索: SA 上で二分探索
std::string pat = "an";
// lower_bound / upper_bound で O(m log n) の検索が可能
```

## 実装上の注意

- `suffixarray(string)` の内部では各文字を `int` に変換し、`max_s = 255` として SA-IS を呼び出す。
- `lcp_array` と組み合わせて使う場合は、`lcp_array` の仕様に合わせてセンチネル（最小文字 `$`）付きの $n+1$ 要素 SA を作成する必要がある（詳細は [lcp_array.md](lcp_array.md) 参照）。

## 参考文献
