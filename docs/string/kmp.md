文字列 `text` の中でパターン `pattern` が出現する位置を全て $O(n+m)$ で求めるアルゴリズム群。いずれもパターン自身の**接頭辞関数**(failure function)を前計算し、不一致が起きたときに文字を再走査せずにシフトする点は共通しているが、その接頭辞関数の作り方が異なる。

# 接頭辞関数(prefix function / LPS 配列)

```cpp
std::vector<int> kmp_failure(const std::string &s);
```
- `s`: 対象の文字列(長さ $m$)
- 戻り値: `lps`(長さ $m$)。 `lps[i]` は `s[0..i]` の**真の接頭辞かつ接尾辞**になる最長の長さ

$$
lps[i] = \max \{\, k < i+1 \mid s[0..k-1] = s[i-k+1..i] \,\}
$$

これは Morris-Pratt・Knuth-Morris-Pratt の両方が共有する基礎データで、標準的な定義(CLRS の $\pi$ 関数などと同じ)。

# Knuth-Morris-Pratt(KMP)アルゴリズム

```cpp
std::vector<int> kmp_optimized_failure(const std::string &s);

std::vector<int> kmp_search(const std::string &text, const std::string &pattern);
```
- `kmp_optimized_failure`: `kmp_failure` をもとに、Knuthの最適化を施した「シフト先」配列 `opt` を作る
- `kmp_search`: `text` の中で `pattern` が出現する開始位置を昇順に列挙する

Knuthの最適化: 素朴な `lps` をそのままシフト先に使うと、不一致が起きた文字 `pattern[i+1]` と、シフト先の文字 `pattern[lps[i]]` が実は同じ文字だった場合、シフトした直後にまた同じ理由で不一致になることが事前にわかる。そこで、

$$
opt[i] = \begin{cases}
opt[lps[i]-1] & (\,i+1 < m \text{ かつ } s[i+1] = s[lps[i]] \text{ かつ } lps[i] > 0\,) \\
0 & (\,i+1 < m \text{ かつ } s[i+1] = s[lps[i]] \text{ かつ } lps[i] = 0\,) \\
lps[i] & (\text{それ以外})
\end{cases}
$$

というように、無駄になるとわかっているシフト先をさらに先読みしてスキップする。`opt` は左から右へ1回のパスで計算できる( $lps[i]-1 < i$ より、必要な `opt[lps[i]-1]` は既に計算済み)。

`kmp_search` はこの `opt` を使って不一致時のシフト先を決める。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| `kmp_failure` / `kmp_optimized_failure` | $O(m)$ | $O(m)$ |
| `kmp_search` | $O(n+m)$ | $O(m)$ |

## 使用例

```cpp
#include "toolbox/string/kmp.hpp"

auto lps = toolbox::string::kmp_failure("aabaaab");
// lps = {0,1,0,1,2,2,3}
auto opt = toolbox::string::kmp_optimized_failure("aabaaab");
// opt = {0,1,0,0,2,1,3} (lps[3]=1だがopt[3]=0まで飛ばせる)

auto pos = toolbox::string::kmp_search("ababab", "ab");
// pos = {0, 2, 4}
```

## 実装上の注意

- 空パターンは `text` の全位置(`0..|text|`)にマッチするものとして扱う。
- 素朴な(Knuthの最適化なしの)接頭辞関数ベースの探索は `morris_pratt.hpp` の `morris_pratt_search` を参照。漸近計算量は同じ $O(n+m)$ だが、定数倍で `kmp_search` に劣ることがある。

## 参考文献
