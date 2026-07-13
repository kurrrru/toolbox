# Morris-Pratt アルゴリズム

Knuth-Morris-Pratt(KMP、[kmp.md](kmp.md)参照)の元になったパターンマッチングアルゴリズム。パターンの**素朴な**接頭辞関数(`kmp_failure`、Knuthの最適化を適用する前のもの)だけを使って $O(n+m)$ でマッチングする。

## アルゴリズム

`text` と `pattern` を左から順に比較し、不一致が起きたら `pattern` 側の接頭辞関数 `lps` を使って、それまでに一致していた部分を無駄にしないシフト先へ飛ぶ。手続きは KMP の探索ループと全く同じだが、シフト先に `kmp_optimized_failure` の代わりに `kmp_failure` を使う点だけが異なる。

このため、不一致を起こした文字 `pattern[j]` とシフト先の文字 `pattern[lps[j-1]]` がたまたま同じだった場合、シフトした直後にもう一度同じ文字で不一致になる可能性がある(Knuthの最適化はこれを防ぐ)。ただし、この再比較が起きても全体の比較回数は依然として $O(n+m)$ に収まることが償却解析で示せるため、漸近計算量はKMPと変わらない。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(n+m)$ | $O(m)$ |

## インターフェース

```cpp
#include "toolbox/string/morris_pratt.hpp"

std::vector<int> toolbox::string::morris_pratt_search(
    const std::string &text,
    const std::string &pattern
);
```

- `text`: 探索対象の文字列(長さ $n$)
- `pattern`: 探索するパターン(長さ $m$)
- 戻り値: `pattern` が `text` 中に出現する開始位置(0-indexed)の昇順の列

## 使用例

```cpp
#include "toolbox/string/morris_pratt.hpp"

auto pos = toolbox::string::morris_pratt_search("ababab", "ab");
// pos = {0, 2, 4}
```

## 実装上の注意

- 空パターンは `text` の全位置(`0..|text|`)にマッチするものとして扱う。
- 接頭辞関数自体は `kmp.hpp` の `kmp_failure` を再利用しており、独自には持たない。KMPとの違いは探索ループが使うシフト先テーブルだけ。

## 参考文献
