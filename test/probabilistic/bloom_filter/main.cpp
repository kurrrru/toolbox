#include "toolbox/probabilistic/bloom_filter/bloom_filter.hpp"
#include "test/test_util.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

bool test_membership() {
    toolbox::datastructure::BloomFilter<1000, 5> bf;
    std::vector<std::string> keys = {"apple", "orange", "banana", "grape", "melon",
                                      "cherry", "kiwi", "mango", "pear", "plum"};
    for (const auto &k : keys) bf.add(k);
    bool ok = true;
    for (const auto &k : keys)
        ok &= check(bf.contains(k), "contains after add: " + k);
    return ok;
}

bool test_non_member_unlikely() {
    // With 4096-bit filter and 7 hashes for 10 elements, false positive
    // rate is negligible. Allow at most 1 false positive in 20 checks.
    toolbox::datastructure::BloomFilter<4096, 7> bf;
    std::vector<std::string> inserted = {
        "aaa", "bbb", "ccc", "ddd", "eee",
        "fff", "ggg", "hhh", "iii", "jjj"
    };
    for (const auto &k : inserted) bf.add(k);

    std::vector<std::string> absent = {
        "zzz", "yyy", "xxx", "www", "vvv",
        "uuu", "ttt", "sss", "rrr", "qqq",
        "ppp", "ooo", "nnn", "mmm", "lll",
        "kkk", "123", "456", "789", "000"
    };
    int false_pos = 0;
    for (const auto &k : absent)
        if (bf.contains(k)) false_pos++;
    return check(false_pos <= 1, "false positive count <= 1 for 4096-bit filter");
}

bool test_repeated_add() {
    toolbox::datastructure::BloomFilter<1024, 4> bf;
    bf.add("duplicate");
    bf.add("duplicate");
    return check(bf.contains("duplicate"), "duplicate add: still contains");
}

bool test_large_insertion() {
    // Insert 200 items, verify all are found
    toolbox::datastructure::BloomFilter<65536, 7> bf;
    std::vector<std::string> keys;
    for (int i = 0; i < 200; i++)
        keys.push_back("key_" + std::to_string(i));
    for (const auto &k : keys) bf.add(k);
    bool ok = true;
    for (const auto &k : keys)
        ok &= check(bf.contains(k), "large insertion: " + k);
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"membership",          test_membership},
        {"non_member_unlikely", test_non_member_unlikely},
        {"repeated_add",        test_repeated_add},
        {"large_insertion",     test_large_insertion},
    };
    const std::size_t num = sizeof(tests) / sizeof(tests[0]);
    int pass = 0, fail = 0;
    for (std::size_t i = 0; i < num; i++) {
        bool ok = tests[i].fn();
        if (ok) { std::cout << toolbox::color::cyan  << "PASS " << tests[i].name << toolbox::color::reset << "\n"; ++pass; }
        else    { std::cout << toolbox::color::yellow << "FAIL " << tests[i].name << toolbox::color::reset << "\n"; ++fail; }
    }
    std::cout << "\n";
    if (fail == 0)
        std::cout << toolbox::color::green << "All " << pass << " tests passed!" << toolbox::color::reset << "\n";
    else
        std::cout << toolbox::color::red << fail << " out of " << (pass + fail) << " tests failed." << toolbox::color::reset << "\n";
    return fail == 0 ? 0 : 1;
}
