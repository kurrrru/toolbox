# TODO: suffixarray の再実装

## 問題

`suffixarray(const std::string &str)` は長さ n の SA（番兵なし）を返すが、
`lcp_array()` は長さ n+1 の SA（番兵付き）を前提に設計されており、
両者を組み合わせると `sa[n]` の範囲外読み出し（UB）が発生する。

### 現在の挙動

```cpp
// suffixarray("banana") → サイズ 6 の SA: [5, 3, 1, 0, 4, 2]
// lcp_array はサイズ 7 (n+1) の SA を期待しており、
// sa[6] へのアクセスが UB になる
```

デバッグ出力 (2026-04-09 時点):
- `suffixarray("banana")` → `[5, 3, 1, 0, 4, 2]`  (サイズ 6 = n)
- `suffixarray("mississippi")` → `[10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2]`  (サイズ 11 = n)

## 再実装で整理すべき点

1. **SA のサイズ規約を統一する**
   - 番兵を含む n+1 要素にするか、含まない n 要素にするかを決定する
   - `lcp_array` 側も同じ規約に合わせる

2. **ドキュメントに明記する**
   - `@return` に「n 要素 / n+1 要素」どちらかを明記する

3. **doubling と SAIS の出力形式を揃える**
   - `suffixarray_doubling()` も同じ形式を返すか確認する

## 現在のワークアラウンド（テスト側）

`test/string/suffixarray/main.cpp` では、番兵をプリペンドして n+1 SA を手動で
構築してから `lcp_array` に渡している:

```cpp
std::vector<int> sa_np1;
sa_np1.push_back((int)s.size()); // sentinel
sa_np1.insert(sa_np1.end(), sa_n.begin(), sa_n.end());
auto lcp = toolbox::string::lcp_array(s, sa_np1);
```
