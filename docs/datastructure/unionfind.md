# Union-Find

$n$ 個の要素を素集合として管理し、集合の合併（unite）と同一集合判定（same）を高速に行うデータ構造。Union by rank とパス圧縮を組み合わせることで、各操作をほぼ定数時間で実現する。

## アルゴリズム

各要素の親を記録する配列 `_p` と、木の高さの上界を記録する `_r`（ランク）を保持する。

**`find(x)`**: 根まで再帰的に親を辿り、パス圧縮によって経路上の全ノードを根に直接つなぎ直す。

**`unite(x, y)`**: 両者の根を求め、ランクの小さい木をランクの大きい木の下に接続する（union by rank）。ランクが等しい場合は一方のランクをインクリメントする。

## 計算量

| 操作 | 時間計算量 | 空間計算量 |
|---|---|---|
| 構築 `unionfind(n)` | $O(n)$ | $O(n)$ |
| `find(x)` | $O(\alpha(n))$ | $O(1)$ |
| `unite(x, y)` | $O(\alpha(n))$ | $O(1)$ |
| `same(x, y)` | $O(\alpha(n))$ | $O(1)$ |

$\alpha(n)$ は逆アッカーマン関数であり、実用上は定数とみなせる。

## インターフェース

```cpp
#include "toolbox/datastructure/unionfind/unionfind.hpp"

toolbox::datastructure::unionfind uf(n);  // n 要素で初期化
int  uf.find(int x);                      // x の属する集合の代表元を返す
bool uf.unite(int x, int y);              // x と y の集合を合併。既に同じなら false
bool uf.same(int x, int y);              // x と y が同じ集合に属するか判定
```

### 制約

- 構築時: $n > 0$
- 各操作: $0 \le x, y < n$

## 使用例

```cpp
#include "toolbox/datastructure/unionfind/unionfind.hpp"

toolbox::datastructure::unionfind uf(5);
uf.unite(0, 1);
uf.unite(1, 2);
uf.same(0, 2); // true
uf.same(0, 3); // false
uf.unite(0, 2); // false（既に同一集合）
```

## 参考文献
