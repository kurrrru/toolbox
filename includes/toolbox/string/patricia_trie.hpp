#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace toolbox {

namespace string {

/**
 * @brief A Patricia trie (a.k.a. compressed trie / radix tree) mapping string keys to values
 * of type Value.
 * @tparam Value The type of value associated with each key. Must be default-constructible.
 * @note Unlike Trie (see trie.hpp), each edge is labeled with a whole substring rather than a
 *       single character: chains of single-child nodes are merged into one edge, so the tree
 *       has at most one node per branching point or stored key, not per input character. Each
 *       node owns its children via std::unique_ptr, held in an unordered_map keyed by the
 *       first character of the child's edge label (siblings always have distinct first
 *       characters, by construction).
 * @note This implementation supports: insert, find, contains, starts_with, erase.
 */
template <typename Value>
class PatriciaTrie {
 public:
    PatriciaTrie() : _root(std::make_unique<Node>()) {}

    /**
     * @brief Inserts key with the given value, overwriting any existing value for that key.
     * @note [Complexity]: O(|key|) time: an edge is split at most once per insertion.
     */
    void insert(const std::string &key, const Value &value) {
        Node *node = _root.get();
        std::size_t pos = 0;
        while (pos < key.size()) {
            const char c = key[pos];
            auto it = node->children.find(c);
            if (it == node->children.end()) {
                auto leaf = std::make_unique<Node>();
                leaf->edge_label = key.substr(pos);
                leaf->is_end = true;
                leaf->value = value;
                node->children.emplace(c, std::move(leaf));
                return;
            }
            Node *child = it->second.get();
            const std::size_t lcp = common_prefix_length(child->edge_label, key, pos);
            if (lcp == child->edge_label.size()) {
                pos += lcp;
                node = child;
                continue;
            }
            // The key diverges partway through child's edge label: split that edge by
            // inserting a new intermediate node at the point of divergence.
            std::unique_ptr<Node> old_child = std::move(it->second);
            auto mid = std::make_unique<Node>();
            mid->edge_label = old_child->edge_label.substr(0, lcp);
            old_child->edge_label = old_child->edge_label.substr(lcp);
            const char old_child_key = old_child->edge_label[0];
            mid->children.emplace(old_child_key, std::move(old_child));
            if (pos + lcp == key.size()) {
                mid->is_end = true;
                mid->value = value;
            } else {
                auto leaf = std::make_unique<Node>();
                leaf->edge_label = key.substr(pos + lcp);
                leaf->is_end = true;
                leaf->value = value;
                const char leaf_key = leaf->edge_label[0];
                mid->children.emplace(leaf_key, std::move(leaf));
            }
            node->children[c] = std::move(mid);
            return;
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
        const Node *node = locate_exact(key);
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
        const Node *node = locate_exact(key);
        return node != nullptr && node->is_end;
    }

    /**
     * @brief Checks whether any inserted key starts with prefix (prefix itself included).
     * @note [Complexity]: O(|prefix|) time.
     */
    bool starts_with(const std::string &prefix) const {
        const Node *node = _root.get();
        std::size_t pos = 0;
        while (pos < prefix.size()) {
            auto it = node->children.find(prefix[pos]);
            if (it == node->children.end()) {
                return false;
            }
            const Node *child = it->second.get();
            const std::size_t lcp = common_prefix_length(child->edge_label, prefix, pos);
            if (lcp == prefix.size() - pos) {
                return true;  // prefix ends within this edge
            }
            if (lcp != child->edge_label.size()) {
                return false;  // diverges mid-edge
            }
            pos += lcp;
            node = child;
        }
        return true;
    }

    /**
     * @brief Removes an exact key, pruning dead-end nodes and merging any node left with a
     * single child back into a single edge, preserving the compressed-trie invariant.
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
        std::string edge_label;  // label of the edge from this node's parent to this node
        bool is_end = false;
        Value value{};
    };

    enum class PruneAction { kNone, kRemove, kMerge };

    static std::size_t common_prefix_length(const std::string &edge_label, const std::string &key,
                                            std::size_t pos) {
        std::size_t i = 0;
        const std::size_t max_len = std::min(edge_label.size(), key.size() - pos);
        while (i < max_len && edge_label[i] == key[pos + i]) {
            ++i;
        }
        return i;
    }

    const Node *locate_exact(const std::string &key) const {
        const Node *node = _root.get();
        std::size_t pos = 0;
        while (pos < key.size()) {
            auto it = node->children.find(key[pos]);
            if (it == node->children.end()) {
                return nullptr;
            }
            const Node *child = it->second.get();
            const std::size_t lcp = common_prefix_length(child->edge_label, key, pos);
            if (lcp != child->edge_label.size()) {
                return nullptr;  // diverges mid-edge
            }
            pos += lcp;
            node = child;
        }
        return node;
    }

    // Removes `key` starting at `depth` into `node`. Assumes the key is known to exist
    // (checked by erase() before calling this). Returns what the caller should do about its
    // link to `node`: prune it entirely, splice it out and merge its single remaining child
    // into that link, or leave it alone.
    PruneAction erase_impl(Node *node, const std::string &key, std::size_t depth) {
        if (depth == key.size()) {
            node->is_end = false;
            node->value = Value{};
        } else {
            auto it = node->children.find(key[depth]);
            Node *child = it->second.get();
            const PruneAction action = erase_impl(child, key, depth + child->edge_label.size());
            if (action == PruneAction::kRemove) {
                node->children.erase(it);
            } else if (action == PruneAction::kMerge) {
                auto grand_it = child->children.begin();
                std::unique_ptr<Node> grandchild = std::move(grand_it->second);
                grandchild->edge_label = child->edge_label + grandchild->edge_label;
                it->second = std::move(grandchild);
            }
        }
        if (node->children.empty()) {
            return node->is_end ? PruneAction::kNone : PruneAction::kRemove;
        }
        if (node->children.size() == 1 && !node->is_end) {
            return PruneAction::kMerge;
        }
        return PruneAction::kNone;
    }

    std::unique_ptr<Node> _root;
};

}  // namespace string

}  // namespace toolbox
