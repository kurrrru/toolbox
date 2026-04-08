# Tree Sort

ツリーソートは二分探索木（BST）に全要素を挿入し、中順走査（in-order traversal）で取り出すアルゴリズムである。

## アルゴリズム

1. 全要素を BST に挿入する。比較関数 `comp(node->value, insert_value)` が真なら右の子へ、偽なら左の子へ進む。
2. BST の中順走査で左→根→右の順に要素を収集すると、昇順（または `comp` に応じた順序）の列が得られる。

本実装の BST は平衡化を行わない単純な BST であるため、ソート済み入力では深さが O(n) になる可能性がある。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n²) | O(n) |

安定ソートである（等値要素は左の子に挿入される）。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::tree_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::tree_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/tree_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::tree_sort(v.begin(), v.end());
```
