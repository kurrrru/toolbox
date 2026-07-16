# Prim法

無向重み付きグラフの最小全域木(MST)の総コストを求めるアルゴリズム。1つの頂点から始め、既に木に含まれる頂点集合と、まだ含まれない頂点集合の間を結ぶ辺のうち最小コストのものを繰り返し追加して木を成長させる。優先度付きキューを使うことで $O(E \log V)$ を実現する。

## アルゴリズム

1. 頂点 $0$ を始点として木に加える。
2. 木に含まれる頂点から出る辺を優先度付きキュー(コスト昇順)で管理し、最小コストの辺を取り出す。
3. その辺の行き先がまだ木に含まれていなければ、その頂点を木に加え、コストを加算し、その頂点から出る辺をキューに追加する。既に含まれていればスキップする(遅延削除)。
4. 全頂点が木に含まれれば完成。キューが空になっても全頂点に到達できなければ、グラフは連結ではなく全域木は存在しない。

木の成長を優先度付きキューで進める点はDijkstra法([dijkstra.md](dijkstra.md))とほぼ同じ構造で、キューに入れる値が「始点からの累積距離」ではなく「木に加えるための辺1本のコスト」である点だけが異なる。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(E \log V)$ | $O(V + E)$ |

## インターフェース

```cpp
#include "toolbox/graph/mst/prim.hpp"

// 総コストを返す（long long 特殊化 / 汎用版）
std::optional<long long> toolbox::graph::prim(
    const std::vector<std::vector<std::pair<long long, long long>>> &cost
);
template <typename Vertex, typename Cost>
std::optional<Cost> toolbox::graph::prim(
    const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost
);

// 具体的なMST（採用辺の集合）を返す（long long 特殊化 / 汎用版）
std::optional<std::vector<std::tuple<long long, long long, long long>>>
toolbox::graph::prim_edges(
    const std::vector<std::vector<std::pair<long long, long long>>> &cost
);
template <typename Vertex, typename Cost>
std::optional<std::vector<std::tuple<Cost, Vertex, Vertex>>> toolbox::graph::prim_edges(
    const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost
);
```

- `cost[u]`: 頂点 `u` から出る `{隣接頂点, 辺コスト}` のリスト(無向グラフなので両方向に辺を張る)
- `prim` の戻り値: 最小全域木の総コスト。グラフが連結でない場合は `std::nullopt`
- `prim_edges` の戻り値: 最小全域木を構成する辺の集合(各辺 `{コスト, u, v}`、木に追加した順)。連結な場合はちょうど $n-1$ 本、頂点1個なら空、非連結なら `std::nullopt`。`prim` は内部的に `prim_edges` の結果を合計している

### 制約

- `cost.size() > 0`(頂点数が1以上)

## 使用例

```cpp
#include "toolbox/graph/mst/prim.hpp"

int n = 4;
std::vector<std::vector<std::pair<long long, long long>>> g(n);
auto add_edge = [&](int u, int v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
};
add_edge(0, 1, 1);
add_edge(1, 2, 2);
add_edge(2, 3, 1);
add_edge(0, 3, 4);
add_edge(0, 2, 3);

auto total = toolbox::graph::prim(g);
// total == 4 (辺 0-1(1) + 2-3(1) + 1-2(2))
```

## 実装上の注意

- 隣接リスト表現(Dijkstraと同じ形式)を入力とする。辺リスト形式で最小全域木を求めたい場合は Kruskal法([kruskal.md](kruskal.md))が使える。
- Kruskalと同様、採用された辺そのもの(全域木の構成)は返さず、総コストのみを返す。
- 多重辺・自己ループがあっても正しく動作する。

## 参考文献
