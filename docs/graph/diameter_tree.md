# 木の直径

辺重み付き木における最遠頂点対の距離（直径）を求める。BFS を2回行うことで $O(n)$ を実現する。

## アルゴリズム

木の直径は以下の手順で求まる。

1. 任意の頂点 $r$ を始点として BFS を行い、最も遠い頂点 $u$ を求める。
2. $u$ を始点として BFS を行い、最も遠い頂点 $v$ を求める。
3. $u$ から $v$ への距離が木の直径。

頂点 $u$ は必ず直径の一端であることが証明できるため、この2回の BFS で直径が正確に求まる。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(n)$ | $O(n)$ |

## インターフェース

```cpp
#include "toolbox/graph/other/diameter_tree.hpp"

// long long 特殊化
long long toolbox::graph::diameter_tree(
    const std::vector<std::vector<std::pair<long long, long long>>> &adjacent
);

// 汎用版
template <typename T>
T toolbox::graph::diameter_tree(
    const std::vector<std::vector<std::pair<long long, T>>> &adjacent
);
```

- `adjacent[u]`: `{隣接頂点, 辺コスト}` のリスト（無向木として構築すること）
- 戻り値: 木の直径（最遠頂点対の辺重みの和）

## 使用例

```cpp
#include "toolbox/graph/other/diameter_tree.hpp"

// パスグラフ: 0 -1- 1 -2- 2 -3- 3、直径 = 6
int n = 4;
std::vector<std::vector<std::pair<long long, long long>>> g(n);
auto add_edge = [&](int u, int v, long long w) {
    g[u].push_back({v, w});
    g[v].push_back({u, w});
};
add_edge(0, 1, 1);
add_edge(1, 2, 2);
add_edge(2, 3, 3);

long long d = toolbox::graph::diameter_tree(g); // 6
```

## 実装上の注意

- 入力は木（連結かつ閉路なし）であることを前提とする。一般のグラフには使用できない。
- 直径の両端点の頂点番号は返さない。端点が必要な場合は BFS 内で最遠頂点を記録する処理を追加する。

## 参考文献
