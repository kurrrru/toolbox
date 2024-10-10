# Global alignment
2本の配列の大域アラインメントを求める。コストの最小化問題として定義する。コストは、マッチ $a$、ミスマッチ $x$、ギャップ $g$とする。affine-gapで考える場合は、マッチ $a$、ミスマッチ $x$、ギャップ開始 $o$、ギャップ延長 $e$とする。
## Needleman-Wunschアルゴリズム
```cpp
int needleman_wunsch_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &dp, int a = 0, int x = 1, int g = 1);

void needleman_wunsch_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &dp, std::string &s1_aligned, std::string &s2_aligned, int a = 0, int x = 1, int g = 1);

int needleman_wunsch_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int g = 1);
```
Needleman-Wunschアルゴリズムによって編集距離とアラインメントを求める。
- `s1`,`s2`: 入力文字列
- `dp`: 編集距離を計算するDP
- `a`,`x`,`g`: 編集操作に対するコスト
- `s1_aligned`,`s2_aligned`: `s1`,`s2`のアラインメントの結果

初期条件

$$
\begin{aligned}
dp[i][0] &= i \times g \\
dp[0][j] &= j \times g
\end{aligned}
$$

遷移式

$$
dp[i][j] = \min
\begin{cases}
dp[i-1][j-1] + \delta(s1[i-1], s2[j-1]) \\
dp[i-1][j] + g \\
dp[i][j-1] + g
\end{cases}
$$

ただし、 $\delta(a,b)$は $a=b$のとき $a$、それ以外のとき $x$を返す関数である。  
コストに特に制約はない。`needleman_wunsch_dp`と`needleman_wunsch_all`は最適なアラインメントにおける総コストを返す。

## Needleman-Wunsch-Gotohアルゴリズム
```cpp
int needleman_wunsch_gotoh_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &M, std::vector<std::vector<int>> &D, std::vector<std::vector<int>> &I, int a = 0, int x = 1, int o = 0, int e = 1);

void needleman_wunsch_gotoh_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &M, const std::vector<std::vector<int>> &D, const std::vector<std::vector<int>> &I, std::string &s1_aligned, std::string &s2_aligned, int a = 0, int x = 1, int o = 0, int e = 1);

int needleman_wunsch_gotoh_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int a = 1, int x = 1, int o = 0, int e = 1);
```
Needleman-Wunsch-Gotohアルゴリズムによってaffine-gapにおける編集距離とアラインメントを求める。
- `s1`,`s2`: 入力文字列
- `M`,`D`,`I`: 編集距離を計算するDP
- `a`,`x`,`o`,`e`: 編集操作に対するコスト
- `s1_aligned`,`s2_aligned`: `s1`,`s2`のアラインメントの結果

初期条件

$$
\begin{aligned}
M[i][0] &= i \times e + o \\
M[0][j] &= j \times e + o \\
D[i][0] &= i \times e + o \\
D[0][j] &= \infty \\
I[i][0] &= \infty \\
I[0][j] &= j \times e + o
\end{aligned}
$$

遷移式

$$
\begin{aligned}
D[i][j] &= \min
\begin{cases}
M[i-1][j] + o + e \\
D[i-1][j] + e
\end{cases} \\
I[i][j] &= \min
\begin{cases}
M[i][j-1] + o + e \\
I[i][j-1] + e
\end{cases} \\
M[i][j] &= \min
\begin{cases}
M[i-1][j-1] + \delta(s1[i-1], s2[j-1]) \\
D[i][j] \\
I[i][j]
\end{cases}
\end{aligned}
$$

ただし、 $\delta(a,b)$は $a=b$のとき $a$、それ以外のとき $x$を返す関数である。  
コストに特に制約はない。`needleman_wunsch_gotoh_dp`と`needleman_wunsch_gotoh_all`は最適なアラインメントにおける総コストを返す。
`M`,`D`,`I`はそれぞれmatch/mismatch、deletion、insertionのDPテーブルである。

## diffアルゴリズム
```cpp
int diff_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &M, int x = 1, int g = 1);

void diff_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &M, std::string &s1_aligned, std::string &s2_aligned, int x = 1, int g = 1);

int diff_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int x = 1, int g = 1);
```
diffアルゴリズムによって編集距離とアラインメントを求める。
- `s1`,`s2`: 入力文字列
- `M`: 編集距離を計算するDP
- `x`,`g`: 編集操作に対するコスト
- `s1_aligned`,`s2_aligned`: `s1`,`s2`のアラインメントの結果

初期条件

$$
M[0][0] = lcp(s_1,s_2) \\
$$

遷移式

$$
\begin{align*}
X&=\max\begin{cases}
M[s-g][k-1]+1 \\
M[s-g][k+1] \\
M[s-x][k]+1
\end{cases} \\
M[s][k]&=X+lcp(s_1[X-k..m],s_2[X..n])
\end{align*}
$$

$a=0$, $x>0$, $g>0$が制約となる。`diff_dp`と`diff_all`は最適なアラインメントにおける総コストを返す。
diagonal $k$は、 $k=j-i$を満たすセルの集合として定義される。
2本の配列が類似している場合、効率良くアラインメントを求めることができる。

## Wavefrontアルゴリズム
```cpp
int wavefront_dp(const std::string &s1, const std::string &s2, std::vector<std::vector<int>> &M, std::vector<std::vector<int>> &I, std::vector<std::vector<int>> &D, int x = 1, int o = 0, int e = 1);

void wavefront_traceback(const std::string &s1, const std::string &s2, const std::vector<std::vector<int>> &M, const std::vector<std::vector<int>> &I, const std::vector<std::vector<int>> &D, std::string &s1_aligned, std::string &s2_aligned, int x = 1, int o = 0, int e = 1);

int wavefront_all(const std::string &s1, const std::string &s2, std::string &s1_aligned, std::string &s2_aligned, int x = 1, int o = 0, int e = 1);
```
Wavefrontアルゴリズムによって編集距離とアラインメントを求める。
- `s1`,`s2`: 入力文字列
- `M`,`I`,`D`: 編集距離を計算するDP
- `x`,`o`,`e`: 編集操作に対するコスト
- `s1_aligned`,`s2_aligned`: `s1`,`s2`のアラインメントの結果

初期条件

$$
\begin{aligned}
M[0][0] &= lcp(s_1,s_2) \\
I[0][0] &= -1 \\
D[0][0] &= -1 \\
\end{aligned}
$$

遷移式

$$
\begin{align*}
I[s][k] &= \max
\begin{cases}
M[s - o - e][k - 1] + 1 \\
I[s - e][k - 1] + 1
\end{cases} \\
D[s][k] &= \max
\begin{cases}
M[s - o - e][k + 1] \\
D[s - e][k + 1]
\end{cases} \\
X &= \max
\begin{cases}
M[s - x][k] + 1 \\
I[s][k] \\
D[s][k]
\end{cases} \\
M[s][k] &= X + lcp(s_1[X-k..m], s_2[X..n])
\end{align*}
$$

$a=0$, $x>0$, $o\geq 0$, $e>0$が制約となる。`wavefront_dp`と`wavefront_all`は最適なアラインメントにおける総コストを返す。
diagonal $k$は、 $k=j-i$を満たすセルの集合として定義される。
2本の配列が類似している場合、効率良くアラインメントを求めることができる。
