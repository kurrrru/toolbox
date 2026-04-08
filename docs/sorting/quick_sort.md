# Quick Sort

クイックソートはピボットを基準に配列を分割して再帰的にソートする分割統治アルゴリズムである。

## アルゴリズム

末尾要素をピボットとする。`std::partition` を用いてピボット未満の要素を左に、それ以上を右に分ける（`IsLessThan<T, Compare>` ファンクタを使用）。ピボットを正しい位置に置き、左右を再帰的にソートする。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(log n) |
| 平均 | O(n log n) | O(log n) |
| 最悪 | O(n²) | O(n) |

不安定ソートである。ピボット選択が悪い場合（ソート済み入力など）に最悪ケースとなる。IntroSort はこの問題を回避するために考案された。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::quick_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::quick_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/quick_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::quick_sort(v.begin(), v.end());
```
