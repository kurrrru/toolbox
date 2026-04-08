# Shell Sort

シェルソートは挿入ソートの拡張で、ギャップ（間隔）を徐々に縮めながら挿入ソートを繰り返すアルゴリズムである。

## アルゴリズム

ギャップ列を事前に生成し（本実装では Tokuda 列: $h_k = \lceil (9(9/4)^k - 4) / 5 \rceil$）、最大ギャップから 1 まで縮めながら、各ギャップ幅の挿入ソートを実施する。ギャップが 1 のとき通常の挿入ソートと等価になる。

### ギャップ列（Tokuda 列）の生成

$$\text{gap}(k) = 4^k + 3 \cdot 2^{k-1} + 1 \quad (k = 1, 2, \ldots)$$

先頭に 1 を追加した列: 1, 5, 19, 41, 109, ...

## 計算量

ギャップ列に依存するが、Tokuda 列では平均 O(n^{1.5}) 程度。

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(1) |
| 平均 | O(n^{1.5}) | O(1) |
| 最悪 | O(n^{1.5}) | O(1) |

不安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::shell_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::shell_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/shell_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::shell_sort(v.begin(), v.end());
```
