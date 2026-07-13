#include <iostream>
#include <string>
#include <vector>

#include "toolbox/datastructure/treap/treap.hpp"

#include "utils/test_util.hpp"

namespace {

bool test_insert_lookup() {
    toolbox::datastructure::treap<int, int> t;
    t.insert(3, 30);
    t.insert(1, 10);
    t.insert(5, 50);
    t.insert(2, 20);
    bool ok = true;
    ok &= toolbox::test_utils::check(t[1] == 10, "t[1]==10");
    ok &= toolbox::test_utils::check(t[2] == 20, "t[2]==20");
    ok &= toolbox::test_utils::check(t[3] == 30, "t[3]==30");
    ok &= toolbox::test_utils::check(t[5] == 50, "t[5]==50");
    ok &= toolbox::test_utils::check(t.is_in(1), "is_in(1)");
    ok &= toolbox::test_utils::check(t.is_in(3), "is_in(3)");
    ok &= toolbox::test_utils::check(!t.is_in(4), "not is_in(4)");
    ok &= toolbox::test_utils::check(!t.is_in(0), "not is_in(0)");
    return ok;
}

bool test_to_vector() {
    toolbox::datastructure::treap<int, int> t;
    for (int k : {5, 2, 8, 1, 4, 7, 3, 6}) {
        t.insert(k, k * 10);
    }
    auto v = t.to_vector();
    bool ok = true;
    ok &= toolbox::test_utils::check(v.size() == 8, "to_vector size==8");
    for (std::size_t i = 0; i + 1 < v.size(); i++) {
        ok &= toolbox::test_utils::check(v[i].first < v[i + 1].first, "to_vector sorted by key");
    }
    for (auto &p : v) {
        ok &= toolbox::test_utils::check(p.second == p.first * 10, "to_vector value correct");
    }
    auto vv = t.to_value_vector();
    ok &= toolbox::test_utils::check(vv.size() == 8, "to_value_vector size==8");
    for (std::size_t i = 0; i < vv.size(); i++) {
        ok &= toolbox::test_utils::check(vv[i] == v[i].second, "to_value_vector matches to_vector");
    }
    return ok;
}

bool test_size() {
    toolbox::datastructure::treap<int, int> t;
    bool ok = true;
    ok &= toolbox::test_utils::check(t.size() == 0, "empty size==0");
    t.insert(1, 1);
    ok &= toolbox::test_utils::check(t.size() == 1, "size==1");
    t.insert(2, 2);
    ok &= toolbox::test_utils::check(t.size() == 2, "size==2");
    t.insert(3, 3);
    ok &= toolbox::test_utils::check(t.size() == 3, "size==3");
    return ok;
}

bool test_erase() {
    toolbox::datastructure::treap<int, int> t;
    t.insert(1, 10);
    t.insert(2, 20);
    t.insert(3, 30);
    bool ok = true;
    ok &= toolbox::test_utils::check(t.erase(2) == true, "erase existing key returns true");
    ok &= toolbox::test_utils::check(t.size() == 2, "size==2 after erase");
    ok &= toolbox::test_utils::check(t[1] == 10, "t[1] intact after erase(2)");
    ok &= toolbox::test_utils::check(t[3] == 30, "t[3] intact after erase(2)");
    ok &= toolbox::test_utils::check(t.erase(9) == false, "erase non-existent returns false");
    return ok;
}

bool test_update_value() {
    toolbox::datastructure::treap<int, int> t;
    t.insert(1, 100);
    t[1] = 999;
    return toolbox::test_utils::check(t[1] == 999, "value updated via operator[]");
}

bool test_clear() {
    toolbox::datastructure::treap<int, int> t;
    t.insert(1, 1);
    t.insert(2, 2);
    t.clear();
    bool ok = true;
    ok &= toolbox::test_utils::check(t.size() == 0, "size==0 after clear");
    return ok;
}

bool test_many_inserts() {
    toolbox::datastructure::treap<int, int> t;
    const int N = 100;
    for (int i = 0; i < N; i++) {
        t.insert(i, i * 2);
    }
    bool ok = true;
    ok &= toolbox::test_utils::check(t.size() == static_cast<std::size_t>(N),
                                     "size==N after N inserts");
    for (int i = 0; i < N; i++) {
        ok &= toolbox::test_utils::check(t[i] == i * 2, "t[i] correct");
    }
    // Erase all even keys
    for (int i = 0; i < N; i += 2) {
        t.erase(i);
    }
    ok &= toolbox::test_utils::check(t.size() == static_cast<std::size_t>(N / 2),
                                     "size==N/2 after erasing evens");
    for (int i = 1; i < N; i += 2) {
        ok &= toolbox::test_utils::check(t[i] == i * 2, "odd keys remain");
    }
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"insert_lookup", test_insert_lookup},
        {"to_vector", test_to_vector},
        {"size", test_size},
        {"erase", test_erase},
        {"update_value", test_update_value},
        {"clear", test_clear},
        {"many_inserts", test_many_inserts},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
