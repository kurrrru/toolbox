# Intro Sort

イントロソートは QuickSort・HeapSort・InsertionSort を組み合わせたハイブリッドアルゴリズムで、`std::sort` の標準的な実装として使われる。

## アルゴリズム

再帰深度の上限を $\lfloor \log_2 n \rfloor$ とし、以下を行う：
- サイズが 32 以下なら **挿入ソート** で処理する（小配列で高速）
- 再帰深度が上限に達したら **ヒープソート** にフォールバックする（最悪計算量を保証）
- それ以外は **クイックソート** を実行する。ピボットは中央値3（先頭・中央・末尾の中央値）で選択する

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(log n) |
| 平均 | O(n log n) | O(log n) |
| 最悪 | O(n log n) | O(log n) |

不安定ソートである。QuickSort の高い平均性能とHeapSort の最悪計算量保証を両立する。

## 依存

- `toolbox/sorting/insertion_sort.hpp`
- `toolbox/sorting/heap_sort.hpp`
- `toolbox/sorting/quick_sort.hpp`（`IsLessThan` ファンクタ）

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::intro_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::intro_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/intro_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::intro_sort(v.begin(), v.end());
```
