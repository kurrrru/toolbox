文字列のハッシュ値を使って部分文字列の一致を高速に判定するローリングハッシュと、それを使ったパターンマッチングアルゴリズム。

# RollingHash

```cpp
// toolbox/string/rabin_karp.hpp
class toolbox::string::hash::RollingHash {
public:
    explicit RollingHash(const std::string &s);
    std::uint64_t get(std::size_t l, std::size_t r) const;
    std::size_t size() const;
};
```

文字列 `s` を前処理し、任意の部分文字列 `s[l, r)`(半開区間)のハッシュ値を $O(1)$ で取得できるようにするクラス。

- `s`: ハッシュ化する文字列(長さ $n$)
- `get(l, r)`: `s[l, r)` のハッシュ値。 $0 \le l \le r \le n$
- `size()`: `s` の長さ

## アルゴリズム

法 $M = 2^{61}-1$(メルセンヌ素数)、固定の底 $B$ を用いて、接頭辞ハッシュ

$$
H[i] = (H[i-1] \times B + s[i-1]) \bmod M, \quad H[0] = 0
$$

と、底のべき乗 $B^i \bmod M$ を $O(n)$ で前計算する。区間 `[l, r)` のハッシュは

$$
\text{get}(l, r) = (H[r] - H[l] \times B^{r-l}) \bmod M
$$

で $O(1)$ に求まる。

$M$ は $61$ ビットの素数であり、非敵対的な入力に対しては衝突がほぼ起こらない強度を持つ。ただし底・法は固定(実行毎にランダム化しない)なので、再現性はあるが、底と法を事前に知っている相手が意図的に衝突する入力を作ることは理論上可能。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 構築 | $O(n)$ | $O(n)$ |
| `get` | $O(1)$ | $O(1)$ |

# Rabin-Karp アルゴリズム

```cpp
std::vector<int> toolbox::string::rabin_karp_search(
    const std::string &text,
    const std::string &pattern
);
```

`RollingHash` を使い、`pattern` のハッシュと一致するテキスト中の各区間を候補として検出したのち、**必ず直接比較で検証**してから結果に加える。これにより、ハッシュの衝突が起きても誤検出(false positive)を返すことはない。

- `text`: 探索対象の文字列(長さ $n$)
- `pattern`: 探索するパターン(長さ $m$)
- 戻り値: `pattern` が `text` 中に出現する開始位置(0-indexed)の昇順の列

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 平均(衝突がまれな場合) | $O(n+m)$ | $O(n+m)$ |
| 最悪(検証が繰り返し発生する場合) | $O(nm)$ | $O(n+m)$ |

## 使用例

```cpp
#include "toolbox/string/rabin_karp.hpp"

auto pos = toolbox::string::rabin_karp_search("ababab", "ab");
// pos = {0, 2, 4}

// RollingHash を直接使い、2つの部分文字列が等しいか O(1) で判定する例
toolbox::string::hash::RollingHash rh("abcabc");
bool same = rh.get(0, 3) == rh.get(3, 6); // "abc" == "abc" -> true
```

## 実装上の注意

- 空パターンは `text` の全位置(`0..|text|`)にマッチするものとして扱う。
- 乗算の $\bmod M$ 計算には `__int128` を使ってオーバーフローを回避している。`__int128` は GCC/Clang の拡張だが、本プロジェクトが対象とするコンパイラ(README参照)ではどちらも利用可能。

## 参考文献
