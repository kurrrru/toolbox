# Merge Sort

マージソートは分割統治に基づく安定なソートアルゴリズムである。

## アルゴリズム

1. 配列を 2 分割する。
2. 各半分を再帰的にソートする。
3. ソートされた 2 つの半分を `std::merge` でマージする。

本実装はトップダウン方式で、補助配列としてヒープアロケートした `std::vector` を使用する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n log n) | O(n) |

安定ソートである。再帰深度は O(log n)。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::merge_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::merge_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/merge_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::merge_sort(v.begin(), v.end());
```
