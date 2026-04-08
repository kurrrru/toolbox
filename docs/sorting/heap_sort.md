# Heap Sort

ヒープソートはヒープデータ構造を利用したソートアルゴリズムである。

## アルゴリズム

1. `std::make_heap` で入力をヒープ（最大ヒープ）に変換する: O(n)
2. `std::sort_heap` でヒープから要素を順次抽出してソートする: O(n log n)

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(1) |
| 平均 | O(n log n) | O(1) |
| 最悪 | O(n log n) | O(1) |

不安定ソートである。追加メモリが不要で最悪計算量が保証される。ただしキャッシュ局所性が低いため、実際の性能は QuickSort より劣ることが多い。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::heap_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::heap_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/heap_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::heap_sort(v.begin(), v.end());
```
