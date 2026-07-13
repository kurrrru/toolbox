2本の配列の局所アラインメントを求める。大域アラインメント（[global_alignment.md](global_alignment.md)）とは異なり、スコアの最大化問題として定義する。マッチ $a$ に正の報酬、ミスマッチ $x$・ギャップ $g$ に減点を与え、各セルを $0$ で下限（floor）することで、不利な延長を打ち切って新しい局所アラインメントを開始できるようにする。

# Smith-Watermanアルゴリズム
```cpp
int smith_waterman_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &dp, int a = 1, int x = 1, int g = 1);

void smith_waterman_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &dp, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1);

int smith_waterman_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1);
```
Smith-Watermanアルゴリズムによって最良の局所アラインメントとそのスコアを求める。
- `s1`,`s2`: 入力文字列
- `dp`: スコアを計算するDP
- `a`,`x`,`g`: 編集操作に対するスコア（`a`は報酬、`x`,`g`は減点の大きさ）
- `s1_aligned`,`s2_aligned`: 最良の局所アラインメントに対応する `s1`,`s2` の部分文字列（ギャップ文字 `-` を除くと、それぞれ `s1`,`s2` の連続する部分文字列になる。大域アラインメントと異なり、入力全体を復元するものではない）

初期条件

$$
dp[i][0] = dp[0][j] = 0
$$

遷移式

$$
dp[i][j] = \max
\begin{cases}
0 \\
dp[i-1][j-1] + \delta(s1[i-1], s2[j-1]) \\
dp[i-1][j] - g \\
dp[i][j-1] - g
\end{cases}
$$

ただし、 $\delta(a,b)$ は $a=b$ のとき $a$（報酬）、それ以外のとき $-x$（ミスマッチの減点）を返す関数である。
`smith_waterman_dp`と`smith_waterman_all`は $dp$ 全体の最大値（最良の局所アラインメントスコア）を返す。

## 実装上の注意

- $a > 0$ でなければならない。$a \leq 0$ だと、マッチを積み重ねても利得が生まれず、自明な空アラインメント（スコア $0$）と常に同点かそれ以下になり、意味のある局所アラインメントを見つけられない。
- トレースバックは $dp$ 全体の最大値を持つセルから開始し、値が $0$ になったセルで打ち切る。

## 参考文献
