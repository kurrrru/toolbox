# Implicit Treap

インデックスをキーとして暗黙的に管理する Treap。列に対する区間更新・区間クエリ・挿入・削除・反転・ローテーションを $O(\log n)$ で実現する。遅延伝播（lazy propagation）を組み込んだ汎用的な列データ構造である。

## アル�omリズム

通常の Treap では明示的なキーを持つが、Implicit Treap では「左部分木のサイズ」を暗黙のキーとして扱う。split・merge 時にサイズを使って位置を決定するため、挿入・削除後もインデックスが自動的に更新される。

遅延更新は `_lazy` フィールドで管理し、split/merge のたびに `pushdown` で子へ伝播させる。反転フラグ `_rev` も同様に遅延伝播する。

## 計算量

| 操作 | 時間計算量（期待値） |
|---|---|
| `query(l, r)` | $O(\log n)$ |
| `update(l, r, val)` | $O(\log n)$ |
| `operator[](pos)` | $O(\log n)$ |
| `insert(pos, val)` | $O(\log n)$ |
| `erase(pos)` | $O(\log n)$ |
| `reverse(l, r)` | $O(\log n)$ |
| `rotate(l, m, r)` | $O(\log n)$ |
| `size()` | $O(1)$ |

構築（$n$ 要素）: $O(n \log n)$、空間: $O(n)$

## インターフェース

```cpp
#include "toolbox/datastructure/implicit_treap/implicit_treap.hpp"

toolbox::datastructure::implicit_treap<S, operations> t;
S    t.query(int l, int r);         // [l, r) の集約値
void t.update(int l, int r, S val); // [l, r) に val を作用させる
S    t.operator[](int pos);         // pos 番目の要素
void t.insert(int pos, S val);      // pos 番目に val を挿入
void t.erase(int pos);              // pos 番目を削除
void t.reverse(int l, int r);       // [l, r) を反転
void t.rotate(int l, int m, int r); // [l,r) を [m,r) + [l,m) に並び替え
int  t.size();                      // 要素数
void t.clear();                     // 全要素を削除
```

### テンプレートパラメータ

| パラメータ | 説明 |
|---|---|
| `S` | 要素の型 |
| `operations` | モノイド定義（下記の定義済み実装から選択） |
| `seed` | 乱数シード（デフォルト `1`） |

### 定義済みモノイド

`toolbox::datastructure::monoid_implicit_treap` 名前空間に以下を用意している。

| モノイド名 | 更新操作 | クエリ操作 |
|---|---|---|
| `monoid_range_add_range_min` | 区間加算 | 区間最小値 |
| `monoid_range_add_range_max` | 区間加算 | 区間最大値 |
| `monoid_range_add_range_sum` | 区間加算 | 区間和 |
| `monoid_range_update_range_min` | 区間代入 | 区間最小値 |
| `monoid_range_update_range_max` | 区間代入 | 区間最大値 |
| `monoid_range_update_range_sum` | 区間代入 | 区間和 |

### カスタムモノイドの定義

以下の静的メンバ関数を持つ構造体を `operations` に渡す。

```cpp
struct my_monoid {
    static long long query_op(long long a, long long b);  // クエリの合成
    static long long query_id();                          // クエリの単位元
    static long long update_op(long long a, long long b); // 遅延値の合成
    static long long update_id();                         // 遅延値の単位元
    static long long apply(long long a, long long b, int len); // 遅延値 b を集約値 a（長さ len）に作用
};
```

## 使用例

```cpp
#include "toolbox/datastructure/implicit_treap/implicit_treap.hpp"

using namespace toolbox::datastructure;
using namespace monoid_implicit_treap;

std::vector<long long> v = {3, 1, 4, 1, 5};
implicit_treap<long long, monoid_range_add_range_min> t(v);

t.query(0, 5);    // 1  (全体の最小値)
t.update(1, 3, 10); // [1,3) に +10 → {3, 11, 14, 1, 5}
t.query(0, 5);    // 1

t.reverse(0, 3);  // [0,3) を反転 → {14, 11, 3, 1, 5}
t.insert(2, 99);  // インデックス 2 に 99 を挿入
t.erase(0);       // 先頭を削除
```

## 実装上の注意

- コピーコンストラクタ・代入演算子は削除されており、ムーブも不可。
- `operator[]` は要素を取り出すために split/merge を行うため、他の操作と同じく $O(\log n)$ かかる。

## 参考文献
