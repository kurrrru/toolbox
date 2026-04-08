# Tim Sort

TimSort は Python や Java 標準ライブラリで使用される安定ソートで、自然な実行（Run）を活かしたマージソートと二分挿入ソートのハイブリッドアルゴリズムである。

## アルゴリズム

1. **Run の検出**: `minRunLength(n)` で最小 Run 長を計算する（MIN_MERGE = 32 をベースに Galloping により 32〜64 の範囲）。連続する昇順列（または逆順列を反転させたもの）を Run として検出する。

2. **Run の拡張**: Run が最小長未満なら、**二分挿入ソート** で強制的に最小長まで拡張する。

3. **マージ**: Run をスタックに積み、Timsort のマージ規則（TimMerge: X > Y+Z かつ Y > Z）を満たすよう `std::inplace_merge` でマージする。

4. **強制マージ**: 最後に残ったすべての Run をマージして完了。

本実装では内部で `std::vector<T>` に一時コピーして処理し、最後に書き戻す。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n log n) | O(n) |

安定ソートである。実際のデータが部分的にソートされている場合に最良性能を発揮する。

## 依存

- `toolbox/sorting/binary_insertion_sort.hpp`

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::tim_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::tim_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/tim_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::tim_sort(v.begin(), v.end());
```
