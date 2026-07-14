# Kruskal法

無向重み付きグラフの最小全域木(MST)の総コストを求めるアルゴリズム。辺をコストの昇順にソートし、閉路を作らない辺だけを貪欲に採用する。Union-Find([unionfind.md](../datastructure/unionfind.md)参照)を使って閉路判定を $O(\alpha(n))$ で行うことで、全体を $O(E \log E)$ で実現する。

## アルゴリズム

1. 全ての辺をコストの昇順にソートする。
2. コストの小さい辺から順に見て、その辺の両端点が既に同じ集合に属していなければ(閉路を作らなければ)採用し、Union-Findで両端点を併合する。属していれば(閉路を作るなら)採用しない。
3. 採用した辺が $n-1$ 本になれば全域木が完成。最後まで $n-1$ 本に届かなければ、グラフは連結ではなく全域木は存在しない。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(E \log E)$ | $O(V + E)$ |

## インターフェース

```cpp
#include "toolbox/graph/mst/kruskal.hpp"

// long long 特殊化
std::optional<long long> toolbox::graph::kruskal(
    int n,
    std::vector<std::tuple<long long, long long, long long>> edges
);

// 汎用版
template <typename Vertex, typename Cost>
std::optional<Cost> toolbox::graph::kruskal(
    int n,
    std::vector<std::tuple<Cost, Vertex, Vertex>> edges
);
```

- `n`: 頂点数
- `edges[i]`: `{コスト, 端点u, 端点v}` の辺
- 戻り値: 最小全域木の総コスト。グラフが連結でない場合は `std::nullopt`

### 制約

- `n > 0`

## 使用例

```cpp
#include "toolbox/graph/mst/kruskal.hpp"

int n = 4;
std::vector<std::tuple<long long, long long, long long>> edges = {
    {1, 0, 1}, {2, 1, 2}, {1, 2, 3}, {4, 0, 3}, {3, 0, 2},
};
auto total = toolbox::graph::kruskal(n, edges);
// total == 4 (辺 0-1(1) + 2-3(1) + 1-2(2))
```

## 実装上の注意

- 採用された辺そのもの(全域木の構成)は返さず、総コストのみを返す。これは本ライブラリの `dijkstra`/`bellman_ford` が最短経路そのものを復元しない(距離のみ返す)のと同じ方針。全域木の辺集合が必要な場合は、`unionfind::unite` が成功した辺を呼び出し側で記録すればよい。
- 同じ2頂点を結ぶ多重辺があっても正しく動作する(コストの小さい方が採用される)。

## 参考文献
