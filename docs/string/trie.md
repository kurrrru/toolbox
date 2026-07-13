# Trie(トライ木)

文字列の集合(またはキーと値の対応)を格納する木構造。各エッジが1文字に対応し、根からの経路が文字列を表す。共通の接頭辞を持つ文字列同士でノードを共有するため、接頭辞検索が高速に行える。

## アルゴリズム

各ノードは、次に続きうる文字から子ノードへの対応(`std::unordered_map<char, std::unique_ptr<Node>>`)と、そのノードが何らかのキーの終端かどうかを示すフラグ・値を持つ。

- **挿入**: 根から1文字ずつ、対応する子ノードが無ければ作りながら降りていき、最後のノードに値を記録する。
- **検索・前方一致検索**: 同様に1文字ずつ子ノードをたどる。たどれなくなったら不在。
- **削除**: 対象ノードの終端フラグを外したのち、子を持たず終端でもなくなったノードを根に向かって再帰的に刈り取る(プルーニング)。

固定長配列ではなく `unordered_map` で子を持つため、アルファベットを問わず(任意のバイト列を)キーにできる。

## 計算量

| | 時間 | 空間 |
|---|---|---|
| 挿入・検索・前方一致・削除 | $O(\|key\|)$ | 挿入は $O(\|key\|)$ 追加 |

## インターフェース

```cpp
#include "toolbox/string/trie.hpp"

template <typename Value>
class toolbox::string::Trie {
public:
    Trie();
    void insert(const std::string &key, const Value &value);
    std::optional<Value> find(const std::string &key) const;
    bool contains(const std::string &key) const;
    bool starts_with(const std::string &prefix) const;
    bool erase(const std::string &key);
};
```

### 主要な操作

- `insert(key, value)` — `key` に `value` を関連付ける。既存なら上書き。計算量 $O(|key|)$。
- `find(key)` — `key` に対応する値。無ければ `std::nullopt`。計算量 $O(|key|)$。
- `contains(key)` — `key` が挿入済みかどうか。計算量 $O(|key|)$。
- `starts_with(prefix)` — `prefix` から始まるキーが1つでも存在するか(`prefix` 自体を含む)。計算量 $O(|prefix|)$。
- `erase(key)` — `key` を削除し、不要になったノードを刈り取る。戻り値は削除できたかどうか。計算量 $O(|key|)$。
  - 制約: `Value` はデフォルト構築可能である必要がある。

## 使用例

```cpp
#include "toolbox/string/trie.hpp"

toolbox::string::Trie<int> t;
t.insert("apple", 1);
t.insert("app", 2);

t.contains("apple");   // true
t.starts_with("ap");   // true
t.find("app");         // std::optional<int>(2)

t.erase("apple");
t.contains("apple");   // false
t.contains("app");     // true (影響なし)
```

## 実装上の注意

- 圧縮版(エッジが複数文字をまとめて持つ)は [patricia_trie.md](patricia_trie.md) を参照。文字数の多いキーが多い場合はそちらの方がノード数・メモリ使用量が少なくなる。

## 参考文献
