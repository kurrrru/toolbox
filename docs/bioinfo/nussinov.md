# Nussinov アルゴリズム

RNA 二次構造予測のための動的計画法。塩基対の数を最大化する二次構造を $O(n^3)$ で求める。ステムループなどの実際の熱力学的安定性は考慮しない簡略モデルである。

## 塩基対の定義

本実装では以下の3種類の塩基対を許容する（Watson-Crick 対とWobble対）。

| 塩基対 | 種別 |
|---|---|
| A–U | Watson-Crick |
| C–G | Watson-Crick |
| G–U | Wobble対 |

## アルゴリズム

$dp[i][j]$ を「RNA 配列の区間 $[i, j]$ において形成できる塩基対の最大数」と定義する。

**初期条件**

$$
dp[i][i] = 0, \quad dp[i][i-1] = 0
$$

**遷移式**

$$
dp[i][j] = \max
\begin{cases}
dp[i+1][j-1] + \delta(s_i, s_j) & \text{(}i\text{ と }j\text{ を対合)} \\
\max_{i \le k < j} \left( dp[i][k] + dp[k+1][j] \right) & \text{(区間を分割)}
\end{cases}
$$

ここで $\delta(a, b)$ は $(a, b)$ が塩基対を形成するなら $1$、そうでなければ $0$。

トレースバックにより、最大塩基対数を達成する構造をドット括弧記法（`.`、`(`、`)`）で復元する。

## 計算量

| 操作 | 時間 | 空間 |
|---|---|---|
| `nussinov_dp` | $O(n^3)$ | $O(n^2)$ |
| `nussinov_traceback` | $O(n^2)$ | $O(n)$ |

## インターフェース

```cpp
#include "toolbox/bioinfo/rna_2d/nussinov.hpp"

// DP テーブルの計算。最大塩基対数を返す
int toolbox::rna_2d::nussinov_dp(
    const std::string &seq,
    std::vector<std::vector<int>> &dp
);

// DP テーブルからトレースバックして二次構造を返す
std::string toolbox::rna_2d::nussinov_traceback(
    const std::string &seq,
    const std::vector<std::vector<int>> &dp
);

// DP + トレースバックを一括実行
std::string toolbox::rna_2d::nussinov_all(const std::string &seq);
```

- `seq`: RNA 配列文字列（`A`・`U`・`G`・`C` で構成）
- 戻り値 (`nussinov_all`): ドット括弧記法の二次構造文字列

## 使用例

```cpp
#include "toolbox/bioinfo/rna_2d/nussinov.hpp"

std::string seq = "GGGAAAUCC";
std::string structure = toolbox::rna_2d::nussinov_all(seq);
// structure の例: "(((...)))"

// DP と トレースバックを分けて実行する場合
std::vector<std::vector<int>> dp;
int max_pairs = toolbox::rna_2d::nussinov_dp(seq, dp);
std::string s = toolbox::rna_2d::nussinov_traceback(seq, dp);
```

## 実装上の注意

- Nussinov モデルは塩基対の数を最大化するだけであり、実際の RNA 折り畳みエネルギーは考慮しない。より現実的な予測には Zuker アルゴリズム（MFE folding）を用いる。
- 最適な二次構造が一意でない場合、トレースバックはそのうちの1つを返す。
- 疑似結び目（pseudoknot）は扱えない。

## 参考文献
