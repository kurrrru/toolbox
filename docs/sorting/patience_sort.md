# Patience Sort

ペイシェンスソートはトランプのソリティアゲームから着想を得たアルゴリズムで、最長増加部分列（LIS）の長さの計算にも応用される。

## アルゴリズム

**フェーズ1（パイル構築）**: 各要素について、先頭から順に「現在の要素より大きいトップを持つ最初のパイル」を探し、そこに積む。見つからなければ新しいパイルを作る。

**フェーズ2（k-way マージ）**: 各パイルのトップ（最小値）を優先度付きキューで管理し、最小値を順次抽出して結果を構築する。

本実装では優先度付きキューとして線形探索を使用しているため、マージ部分の計算量は O(n × k) となる（k = パイル数）。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 最良 | O(n log n) | O(n) |
| 平均 | O(n log n) | O(n) |
| 最悪 | O(n²) | O(n) |

安定ソートである。パイル数は LIS の長さに等しい。

## インターフェース

```cpp
template <typename RandomIt, typename Compare>
void toolbox::sorting::patience_sort(RandomIt first, RandomIt last, Compare comp);

template <typename RandomIt>
void toolbox::sorting::patience_sort(RandomIt first, RandomIt last);
```

## 使用例

```cpp
#include "toolbox/sorting/patience_sort.hpp"
std::vector<int> v = {5, 3, 1, 4, 2};
toolbox::sorting::patience_sort(v.begin(), v.end());
```
