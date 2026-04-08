# Tournament Sort

トーナメントソートはヒープを用いて、各ラウンドで「勝者（最大値）」を選出し続けるアルゴリズムである。

## アルゴリズム

1. 入力要素からトーナメントツリー（最大ヒープ）を構築する。
2. ヒープの根（最大値）を取り出し、末尾から順に配置する。
3. 取り出した位置を「無効」に設定し（負の無限大に相当）、ヒープ性質を `heapDecreaseKey` で修復する。
4. これを n 回繰り返す。

汎用型対応のため、`int` の `MINUS_INF` の代わりに `TNode<T>` 構造体の `valid` フラグを使用する。無効ノードは常に有効ノードより低い優先度を持つ。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n log n) | O(n) |

不安定ソートである。ヒープソートと本質的に同じだが、`heapDecreaseKey` の操作が明示的である点が異なる。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::tournament_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::tournament_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/tournament_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::tournament_sort(v.begin(), v.end());
```
