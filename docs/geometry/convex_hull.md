# 凸包（Convex Hull）

平面上の点集合を包む最小の凸多角形（凸包）を求める。本ライブラリでは同じ入出力仕様で4つのアルゴリズムを提供する。

## 共通仕様

- 入力: `std::vector<Point<T>>`（座標型 `T` は厳密判定のため整数推奨）。
- 出力: 凸包頂点の**入力配列へのインデックス**を、**反時計回り(CCW)**順で返す(`std::vector<int>`)。始点は末尾に重複させない。
- `include_collinear`(既定 `false`): `true` のとき辺上に並ぶ共線点も境界に含める。`false` のとき凸包の頂点のみを返す。
- 座標が一致する点(重複点)はまとめ、代表インデックスを1つ返す。
- 退化: 相異なる点が0/1/2個ならそれらのインデックスをそのまま返す。全点共線なら、`include_collinear=false` で両端2点、`true` で共線上の全点(整列順)を返す。

`Point<T>` は `toolbox/geometry/point.hpp` で定義される、公開メンバ `x, y` と外積 `cross` / 内積 `dot` を持つ値型。

## アルゴリズムと計算量

| 関数 | アルゴリズム | 時間計算量 |
|---|---|---|
| `convex_hull_monotone_chain` | Andrew's monotone chain | $O(n \log n)$ |
| `convex_hull_graham_scan` | Graham scan | $O(n \log n)$ |
| `convex_hull_jarvis_march` | Jarvis march (gift wrapping) | $O(nh)$（$h$は凸包頂点数、最悪 $O(n^2)$） |
| `convex_hull_quickhull` | Quickhull | 平均 $O(n \log n)$、最悪 $O(n^2)$ |

いずれも同じ凸包(頂点集合・向き)を返す。Jarvis と Quickhull は厳密凸包を求めた後、`include_collinear=true` の場合に各辺上の点を挿入する共通処理を用いる。

## インターフェース

```cpp
#include "toolbox/geometry/convex_hull/monotone_chain.hpp"
#include "toolbox/geometry/convex_hull/graham_scan.hpp"
#include "toolbox/geometry/convex_hull/jarvis_march.hpp"
#include "toolbox/geometry/convex_hull/quickhull.hpp"

template <typename T>
std::vector<int> toolbox::geometry::convex_hull_monotone_chain(
    const std::vector<toolbox::geometry::Point<T>> &points, bool include_collinear = false);
// graham_scan / jarvis_march / quickhull も同じシグネチャ
```

## 使用例

```cpp
#include "toolbox/geometry/convex_hull/monotone_chain.hpp"
using toolbox::geometry::Point;

std::vector<Point<long long>> pts = {{0,0},{4,0},{4,4},{0,4},{2,2}};
auto hull = toolbox::geometry::convex_hull_monotone_chain(pts);
// hull は4隅のインデックスを反時計回りで返す（中心の (2,2) は含まれない）
```

## 実装上の注意

- 外積のみで判定するため整数座標で厳密。座標が大きい場合のオーバーフローは `Point` のドキュメント参照（`long long` で座標 ~1e9 まで安全）。
- 座標型に浮動小数を使うと `==` による重複判定・共線判定が誤差の影響を受ける。整数座標を推奨。

## 参考文献
