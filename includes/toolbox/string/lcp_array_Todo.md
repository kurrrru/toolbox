# TODO: lcp_array の再実装

## 問題

`lcp_array()` は **番兵付きの n+1 要素 SA** を受け取ることを前提に実装されているが、
`suffixarray(const std::string &)` は **n 要素 SA（番兵なし）** を返すため、
そのまま組み合わせると UB が発生する。

### 実装の前提（現状）

```cpp
std::vector<int> lcp_array(const std::string &s, const std::vector<int> &sa)
{
    int n = s.size();
    std::vector<int> isa(n + 1);
    for (int i = 0; i < n + 1; i++)  // sa[n] まで読む → n+1 要素を期待
        isa[sa[i]] = i;
    ...
    if (isa[i] == n)  // 「SA の最後の要素」= 番兵チェック
    ...
    int j = sa[isa[i] + 1];  // sa[n] まで参照しうる
```

- `sa[n]` へのアクセスが存在する → SA はサイズ n+1 必須
- `isa[i] == n` は「接尾辞 i が SA 末尾 = 次の要素がない」という意味で使われている

### 実際の挙動

n 要素の SA を渡した場合:
- `sa[n]` が範囲外読み出し（UB）
- "banana" や "mississippi" はたまたま正常動作するが、"aaaa" では `lcp[0]` が誤った値になる

## 再実装で整理すべき点

1. **入力 SA のサイズ規約を `suffixarray()` と統一する**
   - `suffixarray()` が n 要素を返すなら、`lcp_array` も n 要素 SA を受け取るよう書き直す
   - 番兵チェック (`isa[i] == n`) の代替ロジックを検討する

2. **`@param sa` のドキュメントに期待サイズを明記する**

3. **Kasai's algorithm ベースへの書き直し**
   - 番兵に依存しない n 要素 SA 対応の Kasai アルゴリズムへの置き換えを検討する

## 現在のワークアラウンド（テスト側）

`test/string/suffixarray/main.cpp` で番兵を手動付与してから渡している。
再実装後はこのワークアラウンドを除去すること。
