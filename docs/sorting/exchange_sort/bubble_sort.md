# Bubble Sort

バブルソートは、隣接する要素を比較・交換することを繰り返す最も基本的なソートアルゴリズムである。

## アルゴリズム

外側のループで `i` を 0 から n-2 まで進める。内側のループでは末尾から `j` を逆向きに走査し、`container[j] < container[j-1]` であれば交換する。一度もスワップが発生しなかった場合は早期終了（最適化）する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n) | O(1) |
| 平均 | O(n²) | O(1) |
| 最悪 | O(n²) | O(1) |

安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::bubble_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::bubble_sort(RandomIt first, RandomIt last);
```

- `first`, `last`: ランダムアクセスイテレータ（std::sort と同じ形式）
- `comp`: 厳密弱順序を満たす比較関数オブジェクト（省略時は `std::less<T>`）

## 使用例

```cpp
#include "toolbox/sorting/bubble_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::bubble_sort(v.begin(), v.end());             // 昇順
toolbox::sorting::bubble_sort(v.begin(), v.end(), std::greater<int>());  // 降順
```
