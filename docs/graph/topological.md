# トポロジカルソート

有向非巡回グラフ（DAG）の頂点を、全ての辺 $(u \to v)$ について $u$ が $v$ より前に来るよう並べる。Kahn のアルゴリズム（入次数ベースの BFS）を使用する。閉路が存在する場合は空ベクトルを返す。

## アルゴリズム

各頂点の入次数を計算し、入次数が $0$ の頂点をスタックに積む。スタックから頂点 $v$ を取り出して順序に追加し、$v$ から出る辺を取り除いて（隣接頂点の入次数を減らして）、入次数が $0$ になった頂点を新たにスタックに積む。最終的な順序の長さが $V$ でなければ閉路が存在する。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(V + E)$ | $O(V)$ |

## インターフェース

```cpp
#include "toolbox/graph/directed/topological.hpp"

// long long 特殊化
std::vector<long long> toolbox::graph::topological(
    const std::vector<std::vector<long long>> &edge
);

// 汎用版
template <typename Vertex>
std::vector<Vertex> toolbox::graph::topological(
    const std::vector<std::vector<Vertex>> &edge
);
```

- `edge[u]`: 頂点 $u$ の隣接頂点リスト（有向辺 $u \to v$）
- 戻り値: トポロジカル順序。閉路が存在する場合は空ベクトル

## 使用例

```cpp
#include "toolbox/graph/directed/topological.hpp"

// 0→1, 0→2, 1→3, 2→3
std::vector<std::vector<long long>> edge(4);
edge[0].push_back(1); edge[0].push_back(2);
edge[1].push_back(3); edge[2].push_back(3);

auto order = toolbox::graph::topological(edge);
// order の例: {0, 1, 2, 3} または {0, 2, 1, 3}（複数のトポロジカル順序が存在しうる）

// 閉路がある場合
std::vector<std::vector<long long>> cycle(3);
cycle[0].push_back(1); cycle[1].push_back(2); cycle[2].push_back(0);
toolbox::graph::topological(cycle).empty(); // true
```

## 実装上の注意

- スタックを使用しているため、辞書順最小のトポロジカル順序は得られない。辞書順最小が必要な場合は `std::stack` を `std::priority_queue`（最小ヒープ）に変更する。

## 参考文献
