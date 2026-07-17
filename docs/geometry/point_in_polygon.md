# 点の多角形内包含判定（Point in Polygon）

点が多角形の内部・境界上・外部のいずれにあるかを判定する。一般の単純多角形向けの $O(n)$ 版と、凸多角形専用の $O(\log n)$ 版を提供する。いずれも整数座標で厳密に動作する(3値を返す)。

## 戻り値

```cpp
enum class toolbox::geometry::PolygonPosition {
    outside,   // 外部
    boundary,  // 境界（辺上・頂点上）
    inside,    // 内部
};
```

## アルゴリズム

- **一般多角形 (`point_in_polygon`)**: クロッシング数(レイキャスティング)法。まず各辺について点が辺上にあるかを判定(あれば `boundary`)し、その後、点の $y$ 座標をまたぐ辺を外積の符号で数え、奇偶で内外を決める。実際に半直線を飛ばすのではなく外積で交差の左右を判定するため、浮動小数点のレイ処理を避け整数で厳密。凸・非凸を問わず単純多角形に対応。
- **凸多角形 (`point_in_convex_polygon`)**: 頂点0を扇の要とする三角形分割とみなし、二分探索で点が属する扇形を求め、外積2回で内外・境界を判定する。$O(\log n)$。入力は反時計回りの凸多角形で、連続3頂点が共線でないことを要求する。

## 計算量

| 関数 | 時間 | 空間 |
|---|---|---|
| `point_in_polygon` | $O(n)$ | $O(1)$ |
| `point_in_convex_polygon` | $O(\log n)$ | $O(1)$ |

## インターフェース

```cpp
#include "toolbox/geometry/point_in_polygon.hpp"

// 一般の単純多角形（時計回り・反時計回りどちらでも可）
template <typename T>
toolbox::geometry::PolygonPosition toolbox::geometry::point_in_polygon(
    const std::vector<toolbox::geometry::Point<T>> &polygon,
    const toolbox::geometry::Point<T> &p);

// 凸多角形（反時計回り、連続3頂点が共線でないこと）
template <typename T>
toolbox::geometry::PolygonPosition toolbox::geometry::point_in_convex_polygon(
    const std::vector<toolbox::geometry::Point<T>> &polygon,
    const toolbox::geometry::Point<T> &p);
```

- `polygon`: 多角形の頂点列(最後の頂点から最初の頂点への辺は暗黙)
- 戻り値: `p` が内部 / 境界 / 外部のいずれか

## 使用例

```cpp
#include "toolbox/geometry/point_in_polygon.hpp"
using toolbox::geometry::Point;
using toolbox::geometry::PolygonPosition;

std::vector<Point<long long>> square = {{0,0},{4,0},{4,4},{0,4}};
auto r1 = toolbox::geometry::point_in_polygon(square, Point<long long>(2,2)); // inside
auto r2 = toolbox::geometry::point_in_polygon(square, Point<long long>(0,2)); // boundary
auto r3 = toolbox::geometry::point_in_polygon(square, Point<long long>(5,2)); // outside
```

## 実装上の注意

- `point_in_polygon` は一般の単純多角形(自己交差なし、頂点数 3 以上)を前提とする。時計回り・反時計回りのどちらでもよい。
- `point_in_convex_polygon` は**反時計回りの凸多角形**を前提とする。前提を満たさない入力では結果は未定義。凸包([convex_hull.md](convex_hull.md))の出力(CCW)をそのまま渡せる。
- 外積で座標の積を取るため、大きな座標ではオーバーフローに注意（`Point` のドキュメント参照）。

## 参考文献
