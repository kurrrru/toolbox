# Insertion Sort

挿入ソートは、未ソート部分から要素を取り出し、ソート済み部分の正しい位置に挿入することを繰り返すアルゴリズムである。

## アルゴリズム

`i` を 1 から n-1 まで進め、`key = arr[i]` を取り出す。次に、ソート済み部分 `arr[0..i-1]` の中から `key` を挿入すべき位置 `j` を線形探索し、`std::rotate` で挿入する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n) | O(1) |
| 平均 | O(n²) | O(1) |
| 最悪 | O(n²) | O(1) |

安定ソートである。小さな配列やほぼソート済みのデータに対して効率的。IntroSort や TimSort の基底ケースとして使われる。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::insertion_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::insertion_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/insertion_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::insertion_sort(v.begin(), v.end());
```
