# Bellman-Ford法

負辺を含む有向グラフにおける単一始点最短経路を求めるアルゴリズム。負閉路の検出も可能。辺の緩和を $V$ 回繰り返すことで $O(VE)$ を実現する。

## アルゴリズム

始点からの最短距離を管理する配列 `dist` を $\infty$ で初期化し、始点を $0$ とする。全辺の緩和を $V-1$ 回繰り返す。$V$ 回目にも更新が発生すれば、負閉路が存在すると判定する。

$$
\text{dist}[t] = \min(\text{dist}[t],\; \text{dist}[s] + \text{cost}(s, t))
$$

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(VE)$ | $O(V)$ |

## インターフェース

```cpp
#include "toolbox/graph/shortest_path/bellman_ford.hpp"

// 最短距離を返す。負閉路があれば空ベクトルを返す
std::vector<long long> toolbox::graph::bellman_ford(
    long long s,
    const std::vector<std::vector<std::pair<long long, long long>>> &cost
);

// 汎用版
template <typename Vertex, typename Cost, Cost INF, Cost Zero>
std::vector<Cost> toolbox::graph::bellman_ford(
    Vertex s,
    const std::vector<std::vector<std::pair<Vertex, Cost>>> &cost
);

// 負閉路の有無のみを返す
bool toolbox::graph::negative_cycle(
    long long s,
    const std::vector<std::vector<std::pair<long long, long long>>> &cost
);
```

- `s`: 始点頂点番号
- `cost[u]`: `{隣接頂点, 辺コスト}` のリスト
- 戻り値: 負閉路がなければ最短距離の配列（到達不可能は `INF`）、負閉路があれば空ベクトル

## 使用例

```cpp
#include "toolbox/graph/shortest_path/bellman_ford.hpp"

int n = 4;
std::vector<std::vector<std::pair<long long, long long>>> g(n);
g[0].push_back({1,  1});
g[1].push_back({2, -3});
g[2].push_back({3,  2});

auto dist = toolbox::graph::bellman_ford(0LL, g);
// dist が空でなければ負閉路なし
// dist = {0, 1, -2, 0}

// 負閉路の検出
g[2].push_back({1, -4}); // 1→2→1 が負閉路
auto dist2 = toolbox::graph::bellman_ford(0LL, g);
dist2.empty(); // true（負閉路あり）
```

## 実装上の注意

- 負閉路の検出は、始点から到達可能な負閉路に限られる。全頂点について検出したい場合は、仮想始点を追加してすべての頂点に辺を張る。
- Dijkstra法より低速だが、負辺を扱える点が利点。

## 参考文献
