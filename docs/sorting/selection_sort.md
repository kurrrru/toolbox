# Selection Sort

選択ソートは、未ソート部分の最小値を選択し、未ソート部分の先頭と交換することを繰り返すアルゴリズムである。

## アルゴリズム

`i` を 0 から n-2 まで進める。各ステップで `arr[i..n-1]` の最小値の位置を `std::min_element` で求め、`arr[i]` と交換する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n²) | O(1) |
| 平均 | O(n²) | O(1) |
| 最悪 | O(n²) | O(1) |

不安定ソートである。スワップ回数が正確に n-1 回と少ないため、書き込みコストが高いメモリには有利。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::selection_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::selection_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/selection_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::selection_sort(v.begin(), v.end());
```
