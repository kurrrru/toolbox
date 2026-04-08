# Sorting Algorithms

`toolbox/sorting/` にある21のソートアルゴリズムの概要。

インクルード方法:
```cpp
#include "toolbox/sorting/sorting.hpp"  // 全アルゴリズム
#include "toolbox/sorting/exchange_sort/quick_sort.hpp"  // 個別インクルードも可
```

すべての関数は `std::sort` と同じインターフェースを持つ:
```cpp
toolbox::sorting::xxx_sort(first, last);        // std::less<T> を使用
toolbox::sorting::xxx_sort(first, last, comp);  // カスタム比較関数
```

---

## アルゴリズム一覧

| アルゴリズム | 平均時間 | 最悪時間 | 空間 | 安定 | 特徴 |
|---|---|---|---|---|---|
| [bubble_sort](sorting/exchange_sort/bubble_sort.md) | O(n²) | O(n²) | O(1) | ✓ | 最も単純。スワップが発生しない場合は早期終了 |
| [insertion_sort](sorting/insertion_sort/insertion_sort.md) | O(n²) | O(n²) | O(1) | ✓ | ほぼソート済みで高速。IntroSort・TimSort の基底ケース |
| [binary_insertion_sort](sorting/insertion_sort/binary_insertion_sort.md) | O(n²) | O(n²) | O(1) | ✓ | 挿入位置を二分探索で求める。比較回数 O(n log n) |
| [selection_sort](sorting/selection_sort/selection_sort.md) | O(n²) | O(n²) | O(1) | ✗ | スワップ回数が正確に n−1 回 |
| [shell_sort](sorting/insertion_sort/shell_sort.md) | O(n^1.5) | O(n^1.5) | O(1) | ✗ | Tokuda ギャップ列を使用 |
| [comb_sort](sorting/exchange_sort/comb_sort.md) | O(n²/2^p) | O(n²) | O(1) | ✗ | バブルソートをギャップ縮小で改良 |
| [gnome_sort](sorting/exchange_sort/gnome_sort.md) | O(n²) | O(n²) | O(1) | ✓ | 実装が極めてシンプル |
| [shaker_sort](sorting/exchange_sort/shaker_sort.md) | O(n²) | O(n²) | O(1) | ✓ | バブルソートの双方向版。タートル問題を軽減 |
| [odd_even_sort](sorting/exchange_sort/odd_even_sort.md) | O(n²) | O(n²) | O(1) | ✓ | 並列化向けのバブルソート変種 |
| [cycle_sort](sorting/selection_sort/cycle_sort.md) | O(n²) | O(n²) | O(1) | ✗ | 書き込み回数が理論的最小値 O(n) |
| [merge_sort](sorting/merge_sort/merge_sort.md) | O(n log n) | O(n log n) | O(n) | ✓ | 分割統治。最悪計算量保証あり |
| [quick_sort](sorting/exchange_sort/quick_sort.md) | O(n log n) | O(n²) | O(log n) | ✗ | 末尾ピボット。平均的に高速 |
| [heap_sort](sorting/selection_sort/heap_sort.md) | O(n log n) | O(n log n) | O(1) | ✗ | 追加メモリ不要で最悪計算量保証 |
| [intro_sort](sorting/hybrid_sort/intro_sort.md) | O(n log n) | O(n log n) | O(log n) | ✗ | QuickSort + HeapSort + InsertionSort のハイブリッド。`std::sort` の標準実装 |
| [tim_sort](sorting/hybrid_sort/tim_sort.md) | O(n log n) | O(n log n) | O(n) | ✓ | Python・Java 標準。自然な Run を活かしたマージソート |
| [patience_sort](sorting/insertion_sort/patience_sort.md) | O(n log n) | O(n²) | O(n) | ✓ | トランプのソリティア由来。LIS 長の算出にも使える |
| [tournament_sort](sorting/selection_sort/tournament_sort.md) | O(n log n) | O(n log n) | O(n) | ✗ | トーナメントツリー（最大ヒープ）で最大値を順次抽出 |
| [tree_sort](sorting/selection_sort/tree_sort.md) | O(n log n) | O(n²) | O(n) | ✓ | 非平衡 BST。ソート済み入力で最悪ケース |
| [cartesian_tree_sort](sorting/selection_sort/cartesian_tree_sort.md) | O(n log n) | O(n log n) | O(n) | ✗ | ヒープ性と BST 性を持つカルテシアンツリーを利用 |
| [ternary_split_quick_sort](sorting/exchange_sort/ternary_split_quick_sort.md) | O(n log n) | O(n²) | O(log n) | ✗ | Dutch National Flag 分割。等値要素が多い場合に有効 |
| [merge_insertion_sort](sorting/hybrid_sort/merge_insertion_sort.md) | O(n log n) | O(n log n) | O(n) | ✗ | Ford-Johnson アルゴリズム。比較回数の理論下界に最も近い |

---

## 選び方の目安

- **汎用** → `intro_sort`（`std::sort` 相当）
- **安定ソートが必要** → `tim_sort` / `merge_sort`
- **ほぼソート済みのデータ** → `tim_sort` / `insertion_sort`
- **追加メモリを使いたくない** → `heap_sort` / `intro_sort`
- **等値要素が多い** → `ternary_split_quick_sort`
- **比較回数を最小化したい** → `merge_insertion_sort`
- **書き込み回数を最小化したい** → `cycle_sort`
