# Treap

ランダムな優先度を持つ二分探索木（BST）。キーに関する BST 性と優先度に関するヒープ性を同時に満たすことで、期待 $O(\log n)$ でバランスを保つ。`std::map` と同様のキー→値の連想配列として使用できる。

## アルゴリズム

各ノードはキー・値・乱数優先度の3つを持つ。挿入時にノードの優先度が親より高ければ split/merge で木を組み替え、ヒープ性を回復する。乱数源には xorshift を使用する。

**`insert(key, val)`**: 優先度に従い再帰的に挿入位置を決定する。キーの重複は許可しない。

**`erase(key)`**: 対象ノードを検索し、左右の部分木を merge して削除する。

**`split(t, key, l, r)`**: キーを境に木を左右に分割する（内部操作）。

**`merge(t, l, r)`**: 2つの部分木を優先度に従い合併する（内部操作）。

## 計算量

| 操作 | 時間計算量（期待値） | 空間計算量 |
|---|---|---|
| `insert(key, val)` | $O(\log n)$ | $O(1)$ |
| `erase(key)` | $O(\log n)$ | $O(1)$ |
| `operator[](key)` | $O(\log n)$ | $O(1)$ |
| `is_in(key)` | $O(\log n)$ | $O(1)$ |
| `size()` | $O(1)$ | $O(1)$ |
| `clear()` | $O(n)$ | $O(1)$ |
| `to_vector()` | $O(n)$ | $O(n)$ |

## インターフェース

```cpp
#include "toolbox/datastructure/treap/treap.hpp"

toolbox::datastructure::treap<Key, Value> t;
void   t.insert(Key key, Value val); // キーと値を挿入（重複不可）
bool   t.erase(Key key);             // キーを削除。存在しなければ false
Value& t.operator[](Key key);        // キーに対応する値を返す（存在しない場合は assert 失敗）
bool   t.is_in(Key key);             // キーが存在するか
size_t t.size();                     // 要素数
void   t.clear();                    // 全要素を削除
std::vector<std::pair<Key,Value>> t.to_vector();  // キー昇順のペア列
std::vector<Value>                t.to_value_vector(); // キー昇順の値列
```

### テンプレートパラメータ

| パラメータ | 説明 | デフォルト |
|---|---|---|
| `Key` | キーの型 | — |
| `Value` | 値の型 | — |
| `seed` | 乱数シード | `1` |

## 使用例

```cpp
#include "toolbox/datastructure/treap/treap.hpp"

toolbox::datastructure::treap<int, int> t;
t.insert(3, 30);
t.insert(1, 10);
t.insert(2, 20);
t[1];           // 10
t.is_in(4);     // false
t.erase(2);     // true
t.size();       // 2
auto v = t.to_vector(); // {{1,10},{3,30}}
```

## 実装上の注意

- コピーコンストラクタ・代入演算子は削除されており、ムーブも不可。
- `operator[]` は存在しないキーに対して `assert` で失敗する。事前に `is_in` で確認すること。

## 参考文献
