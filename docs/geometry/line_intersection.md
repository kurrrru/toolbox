# 線分交差判定（Line Intersection）

2本の線分が交わるかどうかを判定する。外積の符号と比較のみで判定するため、整数座標で厳密(浮動小数点誤差なし)に動作する。

## アルゴリズム

線分 $[p_1, p_2]$ と $[p_3, p_4]$ について、4つの外積 $d_1,\dots,d_4$ で相互の位置関係(どちら側にあるか)を求める。

- **一般の交差**: $p_1, p_2$ が直線 $p_3 p_4$ の反対側にあり、かつ $p_3, p_4$ が直線 $p_1 p_2$ の反対側にある(両方が厳密にまたぐ)なら交差。
- **端点接触・共線重なり**: いずれかの外積が $0$(共線)で、その端点が相手の線分上にあるなら交差。

端点での接触、共線での重なり、退化した線分(2端点が一致する=点)も正しく扱う。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| | $O(1)$ | $O(1)$ |

## インターフェース

```cpp
#include "toolbox/geometry/line_intersection.hpp"

// 点 p が線分 [a, b] 上にあるか
template <typename T>
bool toolbox::geometry::on_segment(
    const toolbox::geometry::Point<T> &p,
    const toolbox::geometry::Point<T> &a,
    const toolbox::geometry::Point<T> &b);

// 線分 [p1, p2] と [p3, p4] が交わるか（端点接触・共線重なりを含む）
template <typename T>
bool toolbox::geometry::segments_intersect(
    const toolbox::geometry::Point<T> &p1, const toolbox::geometry::Point<T> &p2,
    const toolbox::geometry::Point<T> &p3, const toolbox::geometry::Point<T> &p4);
```

- 戻り値: 2つの閉線分が少なくとも1点を共有するなら `true`

## 使用例

```cpp
#include "toolbox/geometry/line_intersection.hpp"
using toolbox::geometry::Point;

bool a = toolbox::geometry::segments_intersect(
    Point<long long>(0,0), Point<long long>(4,4),
    Point<long long>(0,4), Point<long long>(4,0));  // true（X字に交差）

bool b = toolbox::geometry::segments_intersect(
    Point<long long>(0,0), Point<long long>(1,1),
    Point<long long>(2,2), Point<long long>(3,3));  // false（共線だが離れている）
```

## 実装上の注意

- 交差の「有無」のみを返し、交点座標は返さない。交点座標は一般に有理数/浮動小数になるため、本関数の範囲外。
- 外積で座標の積を取るため、大きな座標ではオーバーフローに注意（`Point` のドキュメント参照）。

## 参考文献
