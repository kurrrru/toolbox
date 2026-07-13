#include <random>
#include <set>
#include <string>

#include "toolbox/string/patricia_trie.hpp"

#include "utils/test_util.hpp"

namespace {

bool test_insert_and_find() {
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.find("apple") == std::optional<int>(1), "find('apple')==1");
    ok &= toolbox::test_utils::check(!t.find("app").has_value(), "find('app') absent");
    ok &= toolbox::test_utils::check(!t.find("applesauce").has_value(),
                                     "find('applesauce') absent (superstring)");
    return ok;
}

bool test_edge_split_shorter_key_after() {
    // Insert "apple" (single edge from root), then "app" (a strict prefix of it): must split
    // the "apple" edge at "app" and mark the new intermediate node as terminal.
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    t.insert("app", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.contains("apple"), "'apple' still present after split");
    ok &= toolbox::test_utils::check(t.contains("app"), "'app' present after split");
    ok &= toolbox::test_utils::check(t.find("apple") == std::optional<int>(1),
                                     "'apple' value intact");
    ok &= toolbox::test_utils::check(t.find("app") == std::optional<int>(2),
                                     "'app' has its own value");
    return ok;
}

bool test_edge_split_longer_key_after() {
    // Insert "app" first, then "apple": "app" is fully consumed as a node boundary, then a
    // brand new leaf edge "le" is created below it.
    toolbox::string::PatriciaTrie<int> t;
    t.insert("app", 1);
    t.insert("apple", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.contains("app"), "'app' present");
    ok &= toolbox::test_utils::check(t.contains("apple"), "'apple' present");
    return ok;
}

bool test_edge_split_diverging_branch() {
    // "apple" then "applesauce" then "apply": exercises a three-way branch under a shared
    // "appl" edge prefix.
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    t.insert("applesauce", 2);
    t.insert("apply", 3);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.contains("apple"), "'apple' present");
    ok &= toolbox::test_utils::check(t.contains("applesauce"), "'applesauce' present");
    ok &= toolbox::test_utils::check(t.contains("apply"), "'apply' present");
    ok &= toolbox::test_utils::check(!t.contains("appl"), "'appl' itself not a stored key");
    ok &= toolbox::test_utils::check(t.starts_with("appl"), "'appl' is a valid prefix");
    return ok;
}

bool test_starts_with_ends_mid_edge() {
    // A single compressed edge "apple" from the root: "app" should still be a recognized
    // prefix even though there is no node boundary there.
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.starts_with("app"), "starts_with('app') mid-edge");
    ok &= toolbox::test_utils::check(t.starts_with("a"), "starts_with('a') mid-edge");
    ok &= toolbox::test_utils::check(t.starts_with("apple"), "starts_with('apple') exact");
    ok &= toolbox::test_utils::check(!t.starts_with("applesauce"), "!starts_with('applesauce')");
    ok &= toolbox::test_utils::check(!t.starts_with("b"), "!starts_with('b')");
    return ok;
}

bool test_empty_key() {
    toolbox::string::PatriciaTrie<int> t;
    bool ok = true;
    ok &= toolbox::test_utils::check(t.starts_with(""), "starts_with('') always true");
    ok &= toolbox::test_utils::check(!t.contains(""), "!contains('') before insert");
    t.insert("", 42);
    ok &= toolbox::test_utils::check(t.contains(""), "contains('') after insert");
    return ok;
}

bool test_overwrite() {
    toolbox::string::PatriciaTrie<int> t;
    t.insert("key", 1);
    t.insert("key", 2);
    return toolbox::test_utils::check(t.find("key") == std::optional<int>(2),
                                      "insert overwrites existing value");
}

bool test_erase_merges_single_child() {
    // "apple" and "applesauce" share the "apple" node with one child edge "sauce". Erasing
    // "apple" should merge "apple" node away, leaving a single edge "applesauce" from root.
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    t.insert("applesauce", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("apple"), "erase('apple') returns true");
    ok &= toolbox::test_utils::check(!t.contains("apple"), "'apple' gone");
    ok &= toolbox::test_utils::check(t.contains("applesauce"), "'applesauce' still present");
    ok &= toolbox::test_utils::check(t.starts_with("apple"), "'apple' still a valid prefix");
    return ok;
}

bool test_erase_prunes_dead_branch() {
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("apple"), "erase('apple') returns true");
    ok &= toolbox::test_utils::check(!t.starts_with("a"), "root has no children left");
    return ok;
}

bool test_erase_not_present() {
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    return toolbox::test_utils::check(!t.erase("banana"), "erase of absent key returns false");
}

bool test_erase_three_way_branch() {
    toolbox::string::PatriciaTrie<int> t;
    t.insert("apple", 1);
    t.insert("applesauce", 2);
    t.insert("apply", 3);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("apply"), "erase('apply')");
    ok &= toolbox::test_utils::check(!t.contains("apply"), "'apply' gone");
    ok &= toolbox::test_utils::check(t.contains("apple"), "'apple' still present");
    ok &= toolbox::test_utils::check(t.contains("applesauce"), "'applesauce' still present");
    return ok;
}

// Randomized cross-check against std::set<std::string> as a reference model for
// insert/erase/contains/starts_with over a small alphabet, so keys frequently share
// prefixes and force edge splitting/merging/pruning.
bool test_randomized_against_set() {
    std::mt19937 rng(31415);
    std::uniform_int_distribution<int> char_dist(0, 2);  // alphabet {'a', 'b', 'c'}
    std::uniform_int_distribution<int> len_dist(0, 5);
    toolbox::string::PatriciaTrie<int> trie;
    std::set<std::string> reference;
    bool ok = true;
    for (int step = 0; step < 2000 && ok; ++step) {
        const int len = len_dist(rng);
        std::string key(len, 'a');
        for (char &c : key) {
            c = static_cast<char>('a' + char_dist(rng));
        }
        if (rng() % 2 == 0) {
            trie.insert(key, step);
            reference.insert(key);
        } else {
            const bool trie_erased = trie.erase(key);
            const bool ref_erased = reference.erase(key) > 0;
            ok &= toolbox::test_utils::check(trie_erased == ref_erased,
                                             "erase agreement at step " + std::to_string(step));
        }
        ok &= toolbox::test_utils::check(
            trie.contains(key) == (reference.find(key) != reference.end()),
            "contains agreement at step " + std::to_string(step));
    }
    // Final full cross-check over every possible short string in the alphabet, for both
    // contains() and starts_with() (checked against a linear scan of the reference set).
    for (int len = 0; len <= 5 && ok; ++len) {
        std::string key(len, 'a');
        int total = 1;
        for (int i = 0; i < len; ++i) {
            total *= 3;
        }
        for (int code = 0; code < total; ++code) {
            int c = code;
            for (int i = 0; i < len; ++i) {
                key[i] = static_cast<char>('a' + (c % 3));
                c /= 3;
            }
            const bool ref_contains = reference.find(key) != reference.end();
            ok &= toolbox::test_utils::check(trie.contains(key) == ref_contains,
                                             "final contains agreement for '" + key + "'");
            bool ref_starts_with = false;
            for (const std::string &s : reference) {
                if (s.compare(0, key.size(), key) == 0) {
                    ref_starts_with = true;
                    break;
                }
            }
            ok &= toolbox::test_utils::check(trie.starts_with(key) == ref_starts_with,
                                             "final starts_with agreement for '" + key + "'");
        }
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"insert_and_find", test_insert_and_find},
        {"edge_split_shorter_key_after", test_edge_split_shorter_key_after},
        {"edge_split_longer_key_after", test_edge_split_longer_key_after},
        {"edge_split_diverging_branch", test_edge_split_diverging_branch},
        {"starts_with_ends_mid_edge", test_starts_with_ends_mid_edge},
        {"empty_key", test_empty_key},
        {"overwrite", test_overwrite},
        {"erase_merges_single_child", test_erase_merges_single_child},
        {"erase_prunes_dead_branch", test_erase_prunes_dead_branch},
        {"erase_not_present", test_erase_not_present},
        {"erase_three_way_branch", test_erase_three_way_branch},
        {"randomized_against_set", test_randomized_against_set},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
