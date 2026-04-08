# Cycle Sort

サイクルソートはメモリ書き込み回数を理論的最小値（n-1 回）に抑えるアルゴリズムである。フラッシュメモリなど書き込み寿命がある媒体に有効。

## アルゴリズム

各要素 `arr[cycle_start]` について、ソート後のあるべき位置 `pos`（自分より小さい要素の個数 + cycle_start）を求める。`pos != cycle_start` なら、`arr[cycle_start]` と `arr[pos]` をスワップしてサイクルを追跡し続ける。重複要素はスキップする。

汎用イテレータ版では `==` 演算子の代わりに `!comp(a,b) && !comp(b,a)` で等価性を判定する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n²) | O(1) |
| 平均 | O(n²) | O(1) |
| 最悪 | O(n²) | O(1) |

書き込み回数: O(n)（理論的最小）。不安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::cycle_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::cycle_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/cycle_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::cycle_sort(v.begin(), v.end());
```
