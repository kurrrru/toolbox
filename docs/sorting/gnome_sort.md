# Gnome Sort

ノーム（Gnome）ソートは挿入ソートに似た単純なアルゴリズムで、正しい位置にある間は前進し、逆順なら交換して後退することを繰り返す。

## アルゴリズム

インデックス `i` を 1 から開始する。`arr[i-1] <= arr[i]`（正しい順序）なら `i++`、そうでなければ `arr[i-1]` と `arr[i]` を交換して `i--`（ただし `i > 1` のとき）を繰り返す。`i` が末尾に達したら終了。

本実装では `!comp(*it, *(it-1))`（逆順でない）を判定条件として使用しており、これは元の `comp(a, b) || a == b` と等価である。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n) | O(1) |
| 平均 | O(n²) | O(1) |
| 最悪 | O(n²) | O(1) |

安定ソートである。実装が極めてシンプルだが実用的な性能は低い。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::gnome_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::gnome_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/gnome_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::gnome_sort(v.begin(), v.end());
```
