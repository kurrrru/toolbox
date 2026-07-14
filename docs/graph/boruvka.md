# Boruvka法

無向重み付きグラフの最小全域木(MST)の総コストを求めるアルゴリズム。各連結成分が同時に「自分から出る最小コストの辺」を選び、それらを一斉に併合する操作を繰り返す。1ラウンドで連結成分数が少なくとも半分になるため、$O(\log V)$ ラウンド・各ラウンド $O(E)$ で全体 $O(E \log V)$ を実現する。3つのMSTアルゴリズムの中で最古(1926年)で、各成分の最小辺探索が独立に行えるため並列化・分散処理と相性が良い。

## アルゴリズム

1. 各頂点を1つの連結成分とする(Union-Find で管理、[unionfind.md](../datastructure/unionfind.md)参照)。
2. 各ラウンドで、すべての辺を走査し、連結成分ごとに「その成分から別の成分へ出る最小コストの辺」を求める。
3. 求めた各辺を Union-Find で併合する(既に同じ成分同士なら閉路になるのでスキップ)。
4. 連結成分が1つになるまで繰り返す。あるラウンドで1本も併合できなければ、グラフは連結ではなく全域木は存在しない。

### 同コスト辺の扱い

複数の辺が同じコストを持つ場合、素朴に各成分の最小辺を選ぶと、同一ラウンドで選ばれた辺どうしが閉路を作り、正しくない結果になることがある。本実装では比較を「コスト → 辺のインデックス」の順に行い、全順序を与える(実質的にすべての辺のコストを相異なるものとして扱う)。これにより、同一ラウンドで選ばれる辺が互いに閉路を作らないことが保証される。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(E \log V)$ | $O(V + E)$ |

## インターフェース

```cpp
#include "toolbox/graph/mst/boruvka.hpp"

// long long 特殊化
std::optional<long long> toolbox::graph::boruvka(
    int n,
    const std::vector<std::tuple<long long, long long, long long>> &edges
);

// 汎用版
template <typename Vertex, typename Cost>
std::optional<Cost> toolbox::graph::boruvka(
    int n,
    const std::vector<std::tuple<Cost, Vertex, Vertex>> &edges
);
```

- `n`: 頂点数
- `edges[i]`: `{コスト, 端点u, 端点v}` の辺
- 戻り値: 最小全域木の総コスト。グラフが連結でない場合は `std::nullopt`

### 制約

- `n > 0`

## 使用例

```cpp
#include "toolbox/graph/mst/boruvka.hpp"

int n = 4;
std::vector<std::tuple<long long, long long, long long>> edges = {
    {1, 0, 1}, {2, 1, 2}, {1, 2, 3}, {4, 0, 3}, {3, 0, 2},
};
auto total = toolbox::graph::boruvka(n, edges);
// total == 4 (辺 0-1(1) + 2-3(1) + 1-2(2))
```

## 実装上の注意

- 辺リスト形式は Kruskal法([kruskal.md](kruskal.md))と共通。隣接リスト形式から MST を求めたい場合は Prim法([prim.md](prim.md))が使える。
- Kruskal/Prim と同様、採用された辺そのもの(全域木の構成)は返さず、総コストのみを返す。
- 多重辺・自己ループがあっても正しく動作する(自己ループは常にスキップされる)。

## 参考文献
