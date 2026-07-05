#include "test/test_util.hpp"
#include "toolbox/probabilistic/bloom_filter/bloom_filter.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool test_membership() {
    toolbox::datastructure::BloomFilter<1000, 5> bf;
    std::vector<std::string> keys = {"apple",  "orange", "banana", "grape", "melon",
                                     "cherry", "kiwi",   "mango",  "pear",  "plum"};
    for (const auto &k : keys) bf.add(k);
    bool ok = true;
    for (const auto &k : keys)
        ok &= toolbox::test_utils::check(bf.contains(k), "contains after add: " + k);
    return ok;
}

bool test_non_member_unlikely() {
    // With 4096-bit filter and 7 hashes for 10 elements, false positive
    // rate is negligible. Allow at most 1 false positive in 20 checks.
    toolbox::datastructure::BloomFilter<4096, 7> bf;
    std::vector<std::string> inserted = {"aaa", "bbb", "ccc", "ddd", "eee",
                                         "fff", "ggg", "hhh", "iii", "jjj"};
    for (const auto &k : inserted) bf.add(k);

    std::vector<std::string> absent = {"zzz", "yyy", "xxx", "www", "vvv", "uuu", "ttt",
                                       "sss", "rrr", "qqq", "ppp", "ooo", "nnn", "mmm",
                                       "lll", "kkk", "123", "456", "789", "000"};
    int false_pos = 0;
    for (const auto &k : absent)
        if (bf.contains(k)) false_pos++;
    return toolbox::test_utils::check(false_pos <= 1,
                                      "false positive count <= 1 for 4096-bit filter");
}

bool test_repeated_add() {
    toolbox::datastructure::BloomFilter<1024, 4> bf;
    bf.add("duplicate");
    bf.add("duplicate");
    return toolbox::test_utils::check(bf.contains("duplicate"), "duplicate add: still contains");
}

bool test_large_insertion() {
    // Insert 200 items, verify all are found
    toolbox::datastructure::BloomFilter<65536, 7> bf;
    std::vector<std::string> keys;
    for (int i = 0; i < 200; i++) keys.push_back("key_" + std::to_string(i));
    for (const auto &k : keys) bf.add(k);
    bool ok = true;
    for (const auto &k : keys)
        ok &= toolbox::test_utils::check(bf.contains(k), "large insertion: " + k);
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"membership", test_membership},
        {"non_member_unlikely", test_non_member_unlikely},
        {"repeated_add", test_repeated_add},
        {"large_insertion", test_large_insertion},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
