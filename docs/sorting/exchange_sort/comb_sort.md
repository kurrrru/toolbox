# Comb Sort

コムソートはバブルソートの改良版で、ギャップを縮小因子 10/13 で小さくしながら比較・交換を繰り返すアルゴリズムである。

## アルゴリズム

初期ギャップを n とし、各イテレーションで gap = max(1, gap × 10/13) に更新する。ギャップが 1 かつスワップが発生しなくなったら終了する。縮小因子 10/13（≈ 0.769）は「タートル問題」（末尾付近の小さな値の移動が遅い）を緩和するために経験的に選ばれている。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(1) |
| 平均 | O(n²/2^p) | O(1) |
| 最悪 | O(n²) | O(1) |

不安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::comb_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::comb_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/comb_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::comb_sort(v.begin(), v.end());
```
