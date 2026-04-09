# Z アルゴリズム

文字列 $S$ に対して Z 配列を $O(|S|)$ で構築するアルゴリズム。Z 配列はパターンマッチングや文字列解析に広く応用される。

## Z 配列の定義

$$
Z[i] = \text{文字列 } S \text{ と } S[i:] \text{ の最長共通接頭辞の長さ}
$$

- $Z[0] = |S|$（定義による）
- $Z[i]$ は $S[0], S[1], \ldots$ と $S[i], S[i+1], \ldots$ を先頭から比較したときの一致長

例: $S = \text{"aabxaa"}$ のとき $Z = [6, 1, 0, 0, 2, 1]$

## アルゴリズム

$[l, r)$ を「既に計算済みの最も右端まで延びる Z-box（$S[l:r] = S[0:r-l]$ を満たす区間）」として管理する。

各位置 $i$ を処理する際、既知の Z-box を利用してナイーブな比較を避ける。$S[i:r)$ は $S[i-l:r-l)$ と一致することが保証されているため、$Z[i-l]$ を初期値として使い、必要な場合のみ文字列を延長比較する。全文字を高々2回しか参照しないため $O(|S|)$ となる。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(\|S\|)$ | $O(\|S\|)$ |

## インターフェース

```cpp
#include "toolbox/string/zalgorithm.hpp"

std::vector<int> toolbox::string::zalgorithm(const std::string &S);
```

- `S`: 入力文字列
- 戻り値: Z 配列（長さ `|S|`）。`result[0] == |S|`

## 使用例

```cpp
#include "toolbox/string/zalgorithm.hpp"

// Z 配列の計算
auto z = toolbox::string::zalgorithm("aabxaa");
// z = {6, 1, 0, 0, 2, 1}

// パターンマッチング: テキスト text 中のパターン pat の出現位置を列挙
std::string pat = "ab", text = "ababab";
std::string s = pat + "$" + text; // セパレータで連結
auto zz = toolbox::string::zalgorithm(s);
int plen = pat.size(), prefix = plen + 1;
for (int i = prefix; i < (int)s.size(); i++)
    if (zz[i] >= plen)
        // text[i - prefix] が出現位置
        ;
```

## 参考文献
