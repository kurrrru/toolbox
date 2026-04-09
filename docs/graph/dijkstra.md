# Dijkstra法

非負辺重みの有向グラフ（または無向グラフ）における単一始点最短経路を求めるアルゴリズム。優先度付きキューを用いることで $O((V + E) \log V)$ を実現する。

## アルゴリズム

始点から各頂点への最短距離を管理する配列 `dist` を $\infty$ で初期化し、始点を $0$ とする。最小コストの未確定頂点を優先度付きキューから取り出し、その隣接辺を緩和（relax）する。取り出した頂点の `dist` がキュー上の値より小さければスキップする（遅延削除）。

$$
\text{dist}[t] = \min(\text{dist}[t],\; \text{dist}[s] + \text{cost}(s, t))
$$

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O((V + E) \log V)$ | $O(V + E)$ |

## インターフェース

```cpp
#include "toolbox/graph/shortest_path/dijkstra.hpp"

// long long 特殊化（INF = 1LL<<60, Zero = 0）
std::vector<long long> toolbox::graph::dijkstra(
    long long s,
    const std::vector<std::vector<std::pair<long long, long long>>> &cost
);

// 汎用版
template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> toolbox::graph::dijkstra(
    Vertex s,
    const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost
);
```

- `s`: 始点頂点番号
- `cost[u]`: `{隣接頂点, 辺コスト}` のリスト
- 戻り値: 各頂点への最短距離（到達不可能な頂点は `INF`）

### 制約

- 全ての辺コストが非負

## 使用例

```cpp
#include "toolbox/graph/shortest_path/dijkstra.hpp"

int n = 4;
std::vector<std::vector<std::pair<long long, long long>>> g(n);
// 無向辺を追加
auto add_edge = [&](int u, int v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
};
add_edge(0, 1, 1);
add_edge(1, 2, 2);
add_edge(2, 3, 1);

auto dist = toolbox::graph::dijkstra(0LL, g);
// dist = {0, 1, 3, 4}
```

## 実装上の注意

- 最短経路の経路復元は実装していない。必要な場合は `dist` 更新時に前頂点を記録する処理を追加すること。

## 参考文献
