# Cartesian Tree Sort

カルテシアンツリーソートは、カルテシアンツリー（ヒープ性と BST 性の両方を持つ木）を構築し、優先度付きキューで走査することでソートするアルゴリズムである。

## アルゴリズム

**カルテシアンツリーの構築**: スタックを用いた O(n) 構築。
- 各要素を順番に処理し、`comp(stack.top(), current)` が真の間スタックをポップする。
- ポップした最後の要素を current の左の子とし、current が空になったスタックのトップの右の子になる。
- これにより、ヒープ性（根が最大）と BST 性（左の子は配列で前の要素）を両立する。

**ソート**: 優先度付きキュー `std::priority_queue` に根を積み、ポップのたびに左右の子を追加する。このBFS的な走査で最大値から順に取り出せる。結果を逆順にすると昇順列が得られる。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n log n) | O(n) |

不安定ソートである。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::cartesian_tree_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::cartesian_tree_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/cartesian_tree_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::cartesian_tree_sort(v.begin(), v.end());
```
