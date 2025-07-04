# FM-index

FM-indexは、Burrows-Wheeler Transform (BWT) と補助的なデータ構造を組み合わせることで、テキストの圧縮と高速なパターン検索を同時に実現する自己索引（self-index）である。テキストそのものを保持することなく、索引自身から元のテキストの任意の部分を復元することも可能である。

本実装は、クエリの実行速度を重視した設計となっており、以下の性質を持つ。

- **空間計算量**: $O(n\sigma)$ ワード（ $O(n\sigma\log n)$ ビット）。ただし $n$ はテキスト長、 $\sigma$ は文字の種類数。
- **構築時間**: $O(n\sigma)$ time（SA-ISなどの線形時間アルゴリズムを利用）。
- **`count` 操作**: $O(m)$ time（ $m$ はパターン長）。
- **`locate` 操作**: $O(m + k)$ time（ $k$ はパターンの出現回数）。

# サポートする主要な機能

実装において、以下の関数をサポートする。

- `count(P)`: パターン `P`（長さ $m$）の出現回数を $O(m)$ timeで数える。
    - `LF-mapping` の中核である `Occurrence` 操作が $O(1)$ timeで実行されるため、パターンの長さに比例した時間でカウントが完了する。

- `locate(P)`: パターン `P` の出現位置をすべて報告する。
    - `count` 操作の後、得られた範囲に対応する接尾辞配列の値を直接報告する。出現回数を `k` としたとき、計算時間は $O(m + k)$ となる。

# 実装に関する考察

本実装の性能は、以下の設計上の選択に基づいている。

まず、`Occurrence(i, c)`（`L[0, i)` における文字 `c` の出現回数）を高速に計算するため、文字ごとの出現回数を**累積和配列**として保持する。
具体的には、文字種 `c` と位置 `i` を指定すると、BWT文字列 `L` の接頭辞 `L[0, i)` における `c` の出現回数が即座に求まるような2次元配列を前計算しておく。これにより、`Occurrence` 操作は配列の参照のみで完結するため、 $O(1)$ time での実行が可能となる。この高速アクセスの代償として、 $O(n\sigma)$ ワードの空間を要する。

次に、`locate` 操作については、**完全な接尾辞配列を構築後も索引の一部としてメモリ上に保持**する。`count` で特定された範囲に対応する接尾辞配列のインデックスを直接参照することで、各出現位置を高速に報告できる。このアプローチにより、`locate` の速度は最適化されるが、索引全体の空間使用量は、接尾辞配列のサイズである $O(n)$ ワード（ $O(n \lg n)$ ビット）分、増加する。

総じて、本実装はクエリの応答速度を優先し、そのために空間効率をある程度犠牲にするトレードオフを選択した構成となっている。

# 参考文献
Genome-Scale Algorithm Design 2nd edition
