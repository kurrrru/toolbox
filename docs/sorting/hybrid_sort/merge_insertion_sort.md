# Merge-Insertion Sort (Ford-Johnson Algorithm / PmergeMe)

マージ挿入ソートは Ford と Johnson (1959) が提案した比較回数を最小化するソートアルゴリズムである。比較回数の下界に極めて近い比較回数でソートを行う。本実装は cpp-module-09 の PmergeMe 実装に忠実なポートである。

## アルゴリズム概要

5つのステップに分かれる。以下では配列のサイズを $n$、ペアの数を $k = \lfloor n/2 \rfloor$ とする。

### Step 1: cmpNeighbor（隣接比較）

隣接する 2 要素を比較してペアを作る。各ペアで大きい方を `bigger`、小さい方を `smaller` とする。さらに `bigger` 要素の列（元のペアインデックスを付加）を構築する。

### Step 2: 再帰ソート

`bigger` の列を再帰的に Ford-Johnson でソートする。

### Step 3: reorderPairs（ペア並び替え）

`bigger` がソートされた後、そのインデックス情報を使って `pairs` をソートされた `bigger` の順序に合わせて並び替える。

### Step 4: createMainChain（メインチェーン構築）

メインチェーンを構築する：
$$[b_1, a_1, a_2, \ldots, a_k]$$

ここで $a_i$ はソート済みの大きい方の要素（アンカー）、$b_1$ は $a_1$ とペアを組む小さい方の要素。$b_1 \le a_1$ が保証されているので先頭に置く。

### Step 5: insertIntoMainChain（挿入）

残りの小さい方の要素 $b_2, b_3, \ldots, b_k$（および奇数個の場合の端数要素）を **ヤコブスタール数列** の順序で二分探索によりメインチェーンに挿入する。

ヤコブスタール数列: $J(0)=0,\ J(1)=1,\ J(n) = J(n-1) + 2J(n-2)$

数列: 1, 3, 5, 11, 21, 43, ...

挿入グループ: $(b_3, b_2),\ (b_5, b_4),\ (b_{11}, \ldots, b_6),\ \ldots$

この順序で挿入することで、$b_i$ の二分探索範囲の上界を $a_i$（ペアの大きい方）に限定できる。

### Step 6: writeBack

メインチェーンの値を元の配列に書き戻す。

## 比較回数

$n$ 要素のソートに必要な比較回数の情報理論的下界は $\lceil \log_2(n!) \rceil$ 比較である。Ford-Johnson アルゴリズムはこの下界に極めて近い比較回数を達成する（$n \le 11$ では完全に下界を達成）。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n log n) | O(n) |

不安定ソートである（ペアリングで等値要素の相対順序が変わりうる）。再帰深度 O(log n)。

## 実装詳細

本実装は cpp-module-09 の `PmergeMeSort`（vector特化版）に忠実に基づく。内部では `vector<pair<T, size_t>>` でインデックスを付加し、再帰呼び出しでのペアインデックス追跡を行う。公開APIはランダムアクセスイテレータを受け取る。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::merge_insertion_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::merge_insertion_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/merge_insertion_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::merge_insertion_sort(v.begin(), v.end());
```

## 参考文献

- L. R. Ford Jr. and S. M. Johnson, "A Tournament Problem," *The American Mathematical Monthly*, 66(5), 1959.
- D. E. Knuth, *The Art of Computer Programming*, Vol. 3: Sorting and Searching, Section 5.3.1.
