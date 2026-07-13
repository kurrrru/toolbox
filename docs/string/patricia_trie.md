# Patricia Trie(パトリシア木・基数木 / Radix Tree)

[Trie](trie.md) を圧縮した木構造。子を1つしか持たないノードの連鎖を1本のエッジにまとめ、エッジのラベルを1文字ではなく文字列(部分文字列)にすることで、分岐点・キーの終端に対応するノードだけを持つ。

## アルゴリズム

各ノードは、親から自分へのエッジのラベル(文字列)と、子ノードへの対応(`std::unordered_map<char, std::unique_ptr<Node>>`、兄弟ノード同士はラベルの先頭文字で区別する)を持つ。

- **挿入**: 根から、子のエッジラベルとキーの残り部分の最長共通接頭辞(LCP)を求めながら降りる。
  - LCPがエッジラベル全体に一致すれば、そのままエッジを辿って次の文字へ進む。
  - LCPがエッジラベルの途中で終わる場合、その位置でエッジを分割し、新しい中間ノードを挿入する。
- **検索・前方一致検索**: 同様にLCPを見ながら降りる。`starts_with` は探索中の文字列がエッジの途中で終わっても(ノード境界と一致しなくても)真を返す。
- **削除**: 対象ノードの終端フラグを外したのち、子を持たず終端でもないノードを刈り取り、さらに子が1つだけになった非終端ノードはその子とエッジを合体(マージ)して、常に「分岐点または終端」以外のノードを持たないという圧縮トライの不変条件を保つ。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 挿入・検索・前方一致・削除 | $O(\|key\|)$(エッジの分割・合体は高々1回) | 挿入は $O(\|key\|)$ 追加 |

## インターフェース

```cpp
#include "toolbox/string/patricia_trie.hpp"

template <typename Value>
class toolbox::string::PatriciaTrie {
public:
    PatriciaTrie();
    void insert(const std::string &key, const Value &value);
    std::optional<Value> find(const std::string &key) const;
    bool contains(const std::string &key) const;
    bool starts_with(const std::string &prefix) const;
    bool erase(const std::string &key);
};
```

### 主要な操作

Trie([trie.md](trie.md))と同じ操作・シグネチャを提供する。挙動もキー・値の対応としては同一で、内部表現(エッジの圧縮)のみが異なる。

- 制約: `Value` はデフォルト構築可能である必要がある。

## 使用例

```cpp
#include "toolbox/string/patricia_trie.hpp"

toolbox::string::PatriciaTrie<int> t;
t.insert("apple", 1);
t.insert("applesauce", 2);
// 根から "apple" というラベルのエッジが伸び、その先に "sauce" というエッジが伸びる
// (中間に余分なノードは作られない)

t.starts_with("app");  // true (エッジの途中で終わっていても前方一致とみなす)
t.erase("apple");
t.contains("applesauce"); // true ("apple" ノードは "applesauce" と合体して消える)
```

## 実装上の注意

- 素朴な(1文字ごとにノードを持つ) Trie は [trie.md](trie.md) を参照。キーの数に対してノード数を抑えたい場合(長い共通接頭辞を持つキーが多い場合)にこちらを使う。
- ここでの「Patricia Trie」は、文字列のエッジラベルを扱う圧縮トライ(基数木)としての実装であり、Morrisonのオリジナルの PATRICIA(ビット単位で分岐する版)とは内部表現が異なる。

## 参考文献
