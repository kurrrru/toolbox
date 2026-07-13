#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace toolbox {

namespace string {

/**
 * @brief A trie (prefix tree) mapping string keys to values of type Value.
 * @tparam Value The type of value associated with each key. Must be default-constructible.
 * @note Each node owns its children via std::unique_ptr, and holds them in an
 *       unordered_map keyed by the next character, so any byte alphabet is supported (not
 *       just, e.g., lowercase letters) without paying for a fixed-size array per node.
 * @note This implementation supports: insert, find, contains, starts_with, erase.
 */
template <typename Value>
class Trie {
 public:
    Trie() : _root(std::make_unique<Node>()) {}

    /**
     * @brief Inserts key with the given value, overwriting any existing value for that key.
     * @note [Complexity]: O(|key|) time, O(|key|) additional space in the worst case.
     */
    void insert(const std::string &key, const Value &value) {
        Node *node = _root.get();
        for (char c : key) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                it = node->children.emplace(c, std::make_unique<Node>()).first;
            }
            node = it->second.get();
        }
        node->is_end = true;
        node->value = value;
    }

    /**
     * @brief Looks up the value associated with an exact key.
     * @return The value if key was inserted, std::nullopt otherwise.
     * @note [Complexity]: O(|key|) time.
     */
    std::optional<Value> find(const std::string &key) const {
        const Node *node = locate(key);
        if (node == nullptr || !node->is_end) {
            return std::nullopt;
        }
        return node->value;
    }

    /**
     * @brief Checks whether an exact key was inserted.
     * @note [Complexity]: O(|key|) time.
     */
    bool contains(const std::string &key) const {
        const Node *node = locate(key);
        return node != nullptr && node->is_end;
    }

    /**
     * @brief Checks whether any inserted key starts with prefix (prefix itself included).
     * @note [Complexity]: O(|prefix|) time.
     */
    bool starts_with(const std::string &prefix) const { return locate(prefix) != nullptr; }

    /**
     * @brief Removes an exact key, pruning any nodes left with no children and no other key
     * passing through them.
     * @return true if key was present and removed, false if it was not present.
     * @note [Complexity]: O(|key|) time.
     */
    bool erase(const std::string &key) {
        if (!contains(key)) {
            return false;
        }
        erase_impl(_root.get(), key, 0);
        return true;
    }

 private:
    struct Node {
        std::unordered_map<char, std::unique_ptr<Node>> children;
        bool is_end = false;
        Value value{};
    };

    const Node *locate(const std::string &key) const {
        const Node *node = _root.get();
        for (char c : key) {
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                return nullptr;
            }
            node = it->second.get();
        }
        return node;
    }

    // Removes `key` starting at `depth` into `node`. Assumes the key is known to exist
    // (checked by erase() before calling this). Returns true if `node` became childless and
    // non-terminal, signaling the caller to erase its link to `node`.
    bool erase_impl(Node *node, const std::string &key, std::size_t depth) {
        if (depth == key.size()) {
            node->is_end = false;
            node->value = Value{};
        } else {
            auto it = node->children.find(key[depth]);
            if (erase_impl(it->second.get(), key, depth + 1)) {
                node->children.erase(it);
            }
        }
        return !node->is_end && node->children.empty();
    }

    std::unique_ptr<Node> _root;
};

}  // namespace string

}  // namespace toolbox
