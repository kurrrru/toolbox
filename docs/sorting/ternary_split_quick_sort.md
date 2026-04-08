# Ternary-Split Quick Sort (Dutch National Flag Quick Sort)

3方向分割クイックソートはダイクストラのDutch National Flag アルゴリズムを利用し、等値要素が多い場合に特に効率的なクイックソートの変種である。

## アルゴリズム

ピボットとして配列中央付近の要素を選ぶ。3 つのポインタ `lt`・`i`・`gt` を用いてDutch National Flag 分割を行う：
- `arr[i] < pivot` (`comp` が真): `arr[lt]` と `arr[i]` を交換し、`lt++, i++`
- `arr[i] == pivot`（等値: `!comp(i, pivot) && !comp(pivot, i)`）: `i++`
- `arr[i] > pivot`: `arr[i]` と `arr[gt]` を交換し、`gt--`

分割後、`arr[lt..gt]` はすべてピボット等値なので再帰の対象外。`arr[0..lt-1]` と `arr[gt+1..n-1]` のみ再帰ソートする。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(log n) |
| 平均 | O(n log n) | O(log n) |
| 最悪 | O(n²) | O(n) |

等値要素が多い場合は O(n) に近づく。不安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::ternary_split_quick_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::ternary_split_quick_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/ternary_split_quick_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::ternary_split_quick_sort(v.begin(), v.end());
```
