# Odd-Even Sort (Brick Sort)

奇偶ソートはバブルソートの並列化向けバリアントで、奇数インデックスペアと偶数インデックスペアを交互に処理する。

## アルゴリズム

各イテレーションで以下の 2 フェーズを実施する：
1. 奇数フェーズ: `(arr[1], arr[2]), (arr[3], arr[4]), ...` の各ペアを比較・交換
2. 偶数フェーズ: `(arr[0], arr[1]), (arr[2], arr[3]), ...` の各ペアを比較・交換

両フェーズでスワップが一切発生しなければ終了。

並列環境では各フェーズを並行処理できる。逐次実装では通常のバブルソートと同等の性能である。

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
void toolbox::sorting::odd_even_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::odd_even_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/odd_even_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::odd_even_sort(v.begin(), v.end());
```
