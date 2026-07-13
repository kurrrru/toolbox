# Boyer-Moore アルゴリズム

パターンを**右から左**へ比較し、不一致が起きたときに bad character rule と good suffix rule の2つのヒューリスティックのうち大きい方のシフト量を採用することで、テキストの多くの文字を読み飛ばせるパターンマッチングアルゴリズム。最悪計算量は $O(nm)$ だが、実用上はテキストの全文字を読まずに済むことが多い。

## アルゴリズム

パターンの右端から比較し、`pattern[j]` で不一致が起きた(それより右、`pattern[j+1..m-1]` は一致済み)とき:

- **Bad character rule**: 不一致を起こしたテキスト側の文字 `c` について、`pattern[0..m-2]` 中の `c` の最後の出現位置 `last[c]` を使い、シフト量 `j - last[c]` だけパターンを右にずらす($c$ がパターンに存在しなければ `j+1` 分ずらす)。
- **Good suffix rule**: 一致していた接尾辞 `pattern[j+1..m-1]` が、パターン中の別の位置に(接頭辞として、または別の出現として)再度現れる位置までシフトする。

両方のシフト量のうち大きい方( `std::max(1, good_suffix, bad_character)` )を採用する。

### Good suffix rule の前処理

Good suffix のシフト表の構築は本アルゴリズムの中でも特に難しい部分であり、Charras & Lecroq, *"Handbook of Exact String Matching Algorithms"* の標準的な構成に従って実装している(`toolbox::string::helper::longest_common_suffix_lengths` → `good_suffix_shift`)。

1. `common_suffix[i]`: `pattern[0..i]` の接尾辞のうち、パターン全体の接尾辞でもある最長のものの長さ
2. `common_suffix` から、不一致位置 `i` ごとのシフト量 `good_suffix[i]` を導く

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 前処理(good suffix + bad character) | $O(m)$ (+ アルファベットサイズ) | $O(m)$ (+ アルファベットサイズ) |
| 探索 | 最悪 $O(nm)$、実用上は $O(n)$ 未満のことが多い | $O(1)$ 追加 |

## インターフェース

```cpp
#include "toolbox/string/boyer_moore.hpp"

std::vector<int> toolbox::string::boyer_moore_search(
    const std::string &text,
    const std::string &pattern
);
```

- `text`: 探索対象の文字列(長さ $n$)
- `pattern`: 探索するパターン(長さ $m$)
- 戻り値: `pattern` が `text` 中に出現する開始位置(0-indexed)の昇順の列

## 使用例

```cpp
#include "toolbox/string/boyer_moore.hpp"

auto pos = toolbox::string::boyer_moore_search("ababab", "ab");
// pos = {0, 2, 4}
```

## 実装上の注意

- 空パターンは `text` の全位置(`0..|text|`)にマッチするものとして扱う。
- Bad character の表は 256 バイト値全てに対応する固定長配列(`std::array<int, 256>`)で持つ。文字を配列添字に使う際は `static_cast<unsigned char>` を経由しており、`char` が符号付きの環境でも安全。
- Good suffix のシフト量は理論上常に $1$ 以上になるが、念のため `std::max(1, ...)` による下限を明示的に設けている(無限ループの防止)。

## 参考文献

- Charras, C. and Lecroq, T., *"Handbook of Exact String Matching Algorithms"*
