# Binary Insertion Sort

挿入ソートの挿入位置探索を線形探索から二分探索に置き換えたバリアントである。

## アルゴリズム

`i` を 1 から n-1 まで進め、`key = arr[i]` を取り出す。ソート済み部分 `arr[0..i-1]` に対して `std::lower_bound` で挿入位置を O(log i) で求め、`std::rotate` で挿入する。

比較回数は O(n log n) に削減されるが、要素の移動回数は通常の挿入ソートと変わらないため、実際の実行時間は O(n²) のままである。

## 計算量

| | 時間 | 空間 | 比較回数 |
|---|---|---|---|
| 最良 | O(n log n) | O(1) | O(n log n) |
| 平均 | O(n²) | O(1) | O(n log n) |
| 最悪 | O(n²) | O(1) | O(n log n) |

安定ソートである。TimSort の小ランク拡張に使用される。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::binary_insertion_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::binary_insertion_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/binary_insertion_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::binary_insertion_sort(v.begin(), v.end());
```
