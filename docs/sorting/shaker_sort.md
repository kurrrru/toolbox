# Shaker Sort (Cocktail Sort)

シェーカーソート（カクテルソートとも呼ぶ）はバブルソートの双方向版で、左から右、右から左と交互に比較・交換を繰り返す。

## アルゴリズム

左端 `left` と右端 `right` のポインタを管理する。
1. `left` から `right` まで右に走査し、逆順ペアをスワップ。`right` を最後のスワップ位置に更新。
2. `right` から `left` まで左に走査し、逆順ペアをスワップ。`left` を最後のスワップ位置に更新。
3. `left == right` なら終了。

バブルソートより「タートル問題」（小さい値が先頭に到達するのが遅い）を軽減する。

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
void toolbox::sorting::shaker_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::shaker_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/shaker_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::shaker_sort(v.begin(), v.end());
```
