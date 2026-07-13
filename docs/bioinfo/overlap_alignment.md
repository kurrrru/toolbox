2本の配列のオーバーラップアラインメントを求める。`s1` の**接尾辞**と `s2` の**接頭辞**の間で最良の大域アラインメントを行うスコアを求める問題として定義する。局所アラインメント（[local_alignment.md](local_alignment.md)）と同様、スコアの最大化問題として定式化する。

# Overlapアラインメント
```cpp
int overlap_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &dp, int a = 1, int x = 1, int g = 1);

void overlap_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &dp, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1);

int overlap_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1);
```
`s1` の接尾辞と `s2` の接頭辞の間の最良のオーバーラップアラインメントとそのスコアを求める。
- `s1`,`s2`: 入力文字列
- `dp`: スコアを計算するDP
- `a`,`x`,`g`: 編集操作に対するスコア（`a`は報酬、`x`,`g`は減点の大きさ）
- `s1_aligned`,`s2_aligned`: 最良のオーバーラップに対応する部分文字列。`s1_aligned`からギャップ文字`-`を除いたものは`s1`の接尾辞、`s2_aligned`から`-`を除いたものは`s2`の接頭辞になる。

初期条件

$$
\begin{aligned}
dp[i][0] &= 0 \\
dp[0][j] &= -j \times g
\end{aligned}
$$

遷移式

$$
dp[i][j] = \max
\begin{cases}
dp[i-1][j-1] + \delta(s1[i-1], s2[j-1]) \\
dp[i-1][j] - g \\
dp[i][j-1] - g
\end{cases}
$$

ただし、 $\delta(a,b)$ は $a=b$ のとき $a$（報酬）、それ以外のとき $-x$（ミスマッチの減点）を返す関数である。
`overlap_dp`と`overlap_all`は最終行 $dp[m][\ast]$ の最大値を返す。

## 実装上の注意

- 第0列 (`dp[i][0]`) を常に $0$ にリセットすることで、`s1` 自身の接頭辞は自由にスキップできる（`s1`は接尾辞だけを提供すればよい）。一方、第0行 (`dp[0][j]`) は通常のギャップコストを課す。`s2`は先頭文字から必ず使う接頭辞でなければならないため。
- `s1` の接尾辞は `s1` の末尾に必ず到達する必要がある（そのため最終行 $i=m$ のみを見る）が、`s2` の接頭辞はどこで終わってもよい（そのため最終行の中で $j$ を全探索して最大値を取る）。
- 局所アラインメントと同様、$a > 0$ でなければならない。$a \leq 0$ では自明な空オーバーラップ（スコア $0$）が常に最良になってしまう。
- トレースバックは最終行の最大値を持つ列から開始し、列が $0$ になったところで打ち切る。

## 参考文献
