#include <random>
#include <set>
#include <string>

#include "toolbox/string/trie.hpp"

#include "utils/test_util.hpp"

namespace {

bool test_insert_and_find() {
    toolbox::string::Trie<int> t;
    t.insert("apple", 1);
    t.insert("app", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.find("apple") == std::optional<int>(1), "find('apple')==1");
    ok &= toolbox::test_utils::check(t.find("app") == std::optional<int>(2), "find('app')==2");
    ok &= toolbox::test_utils::check(!t.find("appl").has_value(), "find('appl') absent");
    return ok;
}

bool test_contains() {
    toolbox::string::Trie<int> t;
    t.insert("banana", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.contains("banana"), "contains('banana')");
    ok &= toolbox::test_utils::check(!t.contains("banan"), "!contains('banan') (only a prefix)");
    ok &= toolbox::test_utils::check(!t.contains("bananas"), "!contains('bananas') (superstring)");
    return ok;
}

bool test_starts_with() {
    toolbox::string::Trie<int> t;
    t.insert("banana", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.starts_with("ban"), "starts_with('ban')");
    ok &= toolbox::test_utils::check(t.starts_with("banana"), "starts_with('banana') (exact key)");
    ok &= toolbox::test_utils::check(!t.starts_with("bananas"), "!starts_with('bananas')");
    ok &= toolbox::test_utils::check(!t.starts_with("z"), "!starts_with('z')");
    return ok;
}

bool test_empty_key() {
    toolbox::string::Trie<int> t;
    bool ok = true;
    ok &= toolbox::test_utils::check(t.starts_with(""), "starts_with('') always true");
    ok &= toolbox::test_utils::check(!t.contains(""), "!contains('') before insert");
    t.insert("", 42);
    ok &= toolbox::test_utils::check(t.contains(""), "contains('') after insert");
    ok &= toolbox::test_utils::check(t.find("") == std::optional<int>(42), "find('')==42");
    return ok;
}

bool test_overwrite() {
    toolbox::string::Trie<int> t;
    t.insert("key", 1);
    t.insert("key", 2);
    return toolbox::test_utils::check(t.find("key") == std::optional<int>(2),
                                      "insert overwrites existing value");
}

bool test_erase_leaf() {
    toolbox::string::Trie<int> t;
    t.insert("apple", 1);
    t.insert("app", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("apple"), "erase('apple') returns true");
    ok &= toolbox::test_utils::check(!t.contains("apple"), "'apple' gone after erase");
    ok &=
        toolbox::test_utils::check(t.contains("app"), "'app' still present after erasing 'apple'");
    return ok;
}

bool test_erase_prunes_dead_branch() {
    toolbox::string::Trie<int> t;
    t.insert("apple", 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("apple"), "erase('apple') returns true");
    ok &= toolbox::test_utils::check(!t.contains("apple"), "'apple' gone");
    ok &= toolbox::test_utils::check(!t.starts_with("apple"),
                                     "no dangling prefix left after pruning");
    ok &= toolbox::test_utils::check(!t.starts_with("a"), "root has no children left");
    return ok;
}

bool test_erase_not_present() {
    toolbox::string::Trie<int> t;
    t.insert("apple", 1);
    return toolbox::test_utils::check(!t.erase("banana"), "erase of absent key returns false");
}

bool test_erase_internal_node_stays() {
    // Erasing "app" (a prefix of "apple") must not affect "apple".
    toolbox::string::Trie<int> t;
    t.insert("app", 1);
    t.insert("apple", 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase("app"), "erase('app') returns true");
    ok &= toolbox::test_utils::check(!t.contains("app"), "'app' gone");
    ok &= toolbox::test_utils::check(t.contains("apple"), "'apple' still present");
    ok &= toolbox::test_utils::check(t.starts_with("app"), "'app' still a valid prefix (of apple)");
    return ok;
}

// Randomized cross-check against std::set<std::string> as a reference model for
// insert/erase/contains/starts_with over a small alphabet (so keys frequently share
// prefixes, exercising branch creation/merging/pruning).
bool test_randomized_against_set() {
    std::mt19937 rng(2024);
    std::uniform_int_distribution<int> char_dist(0, 2);  // alphabet {'a', 'b', 'c'}
    std::uniform_int_distribution<int> len_dist(0, 5);
    toolbox::string::Trie<int> trie;
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
    // Final full cross-check over every possible short string in the alphabet.
    for (int len = 0; len <= 5 && ok; ++len) {
        std::string key(len, 'a');
        // Enumerate all 3^len combinations.
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
            ok &= toolbox::test_utils::check(
                trie.contains(key) == (reference.find(key) != reference.end()),
                "final contains agreement for '" + key + "'");
        }
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"insert_and_find", test_insert_and_find},
        {"contains", test_contains},
        {"starts_with", test_starts_with},
        {"empty_key", test_empty_key},
        {"overwrite", test_overwrite},
        {"erase_leaf", test_erase_leaf},
        {"erase_prunes_dead_branch", test_erase_prunes_dead_branch},
        {"erase_not_present", test_erase_not_present},
        {"erase_internal_node_stays", test_erase_internal_node_stays},
        {"randomized_against_set", test_randomized_against_set},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
