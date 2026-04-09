# Bloom Filter

集合へのメンバーシップ判定を確率的に行うデータ構造。偽陰性（false negative）は発生しないが、偽陽性（false positive）が一定確率で発生する。メモリ効率が非常に高く、大規模データの存在確認に用いられる。

## アルゴリズム

サイズ `Size` のビット配列と `HashCount` 個のハッシュ関数を持つ。

**`add(item)`**: `item` のハッシュ値から `HashCount` 個のビット位置を計算し、それぞれを `1` にセットする。

**`contains(item)`**: 同様に `HashCount` 個のビット位置を計算し、全て `1` であれば `true`（多分存在する）、1つでも `0` であれば `false`（確実に存在しない）を返す。

ハッシュ関数の生成には SplitMix64 ベースのアバランシェミキサーを使用し、`std::hash<std::string>` の出力にインデックス $i$ を加算してミキシングすることで `HashCount` 個の独立したハッシュを得る。

### 偽陽性率の近似

$n$ 個の要素を挿入したときの偽陽性率は以下で近似される。

$$
p \approx \left(1 - e^{-k n / m}\right)^k
$$

ここで $k = \text{HashCount}$、$m = \text{Size}$、$n$ は挿入済み要素数。

最適なハッシュ数は $k = \frac{m}{n} \ln 2$ であり、このとき偽陽性率は $p \approx 0.6185^{m/n}$ となる。

## 計算量

| 操作 | 時間計算量 | 空間計算量 |
|---|---|---|
| `add(item)` | $O(\text{HashCount})$ | $O(1)$ |
| `contains(item)` | $O(\text{HashCount})$ | $O(1)$ |
| 構築 | $O(1)$ | $O(\text{Size})$ |

## インターフェース

```cpp
#include "toolbox/probabilistic/bloom_filter/bloom_filter.hpp"

toolbox::datastructure::BloomFilter<Size, HashCount> bf;
void bf.add(const std::string &item);      // item を追加
bool bf.contains(const std::string &item); // item が存在するか（偽陽性あり）
```

### テンプレートパラメータ

| パラメータ | 説明 |
|---|---|
| `Size` | ビット配列のサイズ（大きいほど偽陽性率が下がる） |
| `HashCount` | ハッシュ関数の数（最適値は $\frac{m}{n} \ln 2$） |

## 使用例

```cpp
#include "toolbox/probabilistic/bloom_filter/bloom_filter.hpp"

// 1024ビット・4ハッシュ
toolbox::datastructure::BloomFilter<1024, 4> bf;
bf.add("apple");
bf.add("banana");

bf.contains("apple");   // true（確実に存在する）
bf.contains("grape");   // false または true（偽陽性の可能性あり）
```

## 実装上の注意

- `contains` が `true` を返しても、必ずしも要素が追加されているとは限らない（偽陽性）。
- `contains` が `false` を返した場合は、要素が追加されていないことが確実（偽陰性なし）。
- 削除操作はサポートしていない。削除が必要な場合は Counting Bloom Filter を検討すること。
- ハッシュの計算は `std::hash<std::string>` に依存するため、異なる実行環境間での互換性はない。

## 参考文献
