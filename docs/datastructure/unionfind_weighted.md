# Weighted Union-Find

重み付き Union-Find。各要素にポテンシャル（重み）を持たせ、同一集合内の任意の2要素間の重み差を $O(\alpha(n))$ で求めることができる。グラフの差分制約問題などに応用できる。

## アルゴリズム

通常の Union-Find に加えて、各ノードの親との重み差 `_diff_weight[x]` を管理する。

**重みの定義**: `unite(x, y, w)` は「$\text{weight}(y) - \text{weight}(x) = w$」という関係を登録する。

**`weight(x)`**: パス圧縮を行いながら根までの重み差を累積し、根からの絶対重みを返す。

**`diff(x, y)`**: $\text{weight}(y) - \text{weight}(x)$ を返す。異なる集合に属する場合は $10^9$ を返す。

**`find(x)`**: パス圧縮時に `_diff_weight` も同時に更新することで、重みの一貫性を保つ。

## 計算量

| 操作 | 時間計算量 | 空間計算量 |
|---|---|---|
| 構築 `unionfind_weighted(n)` | $O(n)$ | $O(n)$ |
| `find(x)` | $O(\alpha(n))$ | $O(1)$ |
| `unite(x, y, w)` | $O(\alpha(n))$ | $O(1)$ |
| `same(x, y)` | $O(\alpha(n))$ | $O(1)$ |
| `weight(x)` | $O(\alpha(n))$ | $O(1)$ |
| `diff(x, y)` | $O(\alpha(n))$ | $O(1)$ |

## インターフェース

```cpp
#include "toolbox/datastructure/unionfind_weighted/unionfind_weighted.hpp"

toolbox::datastructure::unionfind_weighted uf(n);
bool uf.unite(int x, int y, int w); // weight(y) - weight(x) = w を登録。同一集合なら false
bool uf.same(int x, int y);         // 同一集合か判定
int  uf.weight(x);                  // x の根からの重み（絶対ポテンシャル）
int  uf.diff(int x, int y);         // weight(y) - weight(x)。異集合なら 1e9
```

### 制約

- 構築時: $n > 0$
- 各操作: $0 \le x, y < n$
- `diff` の戻り値が意味を持つのは `same(x, y) == true` のときのみ

## 使用例

```cpp
#include "toolbox/datastructure/unionfind_weighted/unionfind_weighted.hpp"

toolbox::datastructure::unionfind_weighted uf(3);
uf.unite(0, 1, 3);  // weight(1) - weight(0) = 3
uf.unite(1, 2, 5);  // weight(2) - weight(1) = 5
uf.diff(0, 2);      // 8  (= 3 + 5)
uf.diff(2, 0);      // -8
```

## 参考文献
