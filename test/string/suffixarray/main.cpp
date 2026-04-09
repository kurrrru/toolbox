#include "toolbox/string/suffixarray.hpp"
#include "toolbox/string/lcp_array.hpp"
#include "test/test_util.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// Verify a suffix array of size n (no sentinel):
//   - each index in [0, n) appears exactly once
//   - suffixes are in strictly increasing lexicographic order
bool is_valid_sa(const std::string &s, const std::vector<int> &sa) {
    int n = s.size();
    if ((int)sa.size() != n) return false;
    std::vector<bool> seen(n, false);
    for (int x : sa) {
        if (x < 0 || x >= n) return false;
        if (seen[x]) return false;
        seen[x] = true;
    }
    for (int i = 0; i + 1 < n; i++) {
        if (s.substr(sa[i]) >= s.substr(sa[i + 1])) return false;
    }
    return true;
}

// Verify LCP array computed against the SA directly:
//   lcp[i] == LCP length of suffix sa[i] and suffix sa[i+1].
// (lcp_array() expects an n+1 SA with sentinel; since suffixarray()
//  returns n elements without sentinel, we validate by recomputing LCP
//  from the SA independently and comparing against the implementation.)
// Verify LCP array.
// sa_n   : n-element SA (no sentinel)
// lcp    : output of lcp_array(), computed with an (n+1)-element SA that
//          has sentinel at index 0. As a result, lcp[i] corresponds to the
//          pair (sa_n+1[i], sa_n+1[i+1]) where sa_n+1 = [sentinel] + sa_n.
//          The first element lcp[0] is always 0 (LCP of sentinel with next).
//          The pairs among the n real suffixes occupy lcp[1..n-1].
//          So: brute_lcp(sa_n[i], sa_n[i+1]) == lcp[i+1] for i in [0, n-2).
bool verify_lcp(const std::string &s, const std::vector<int> &sa_n,
                const std::vector<int> &lcp) {
    int n = s.size();
    if ((int)lcp.size() != n) return false;
    for (int i = 0; i + 1 < n; i++) {
        int a = sa_n[i], b = sa_n[i + 1];
        int l = 0;
        while (a + l < n && b + l < n && s[a + l] == s[b + l]) l++;
        // lcp[i+1] corresponds to the pair (sa_n[i], sa_n[i+1])
        if (lcp[i + 1] != l) {
            std::cerr << "  FAIL: lcp[" << (i+1) << "] expected " << l
                      << " got " << lcp[i + 1] << "\n";
            return false;
        }
    }
    return true;
}

// ---- suffix array ----------------------------------------------------------

bool test_sa_basic() {
    std::string s = "banana";
    auto sa = toolbox::string::suffixarray(s);
    return check(is_valid_sa(s, sa), "suffixarray basic: banana");
}

bool test_sa_all_same() {
    std::string s = "aaaa";
    auto sa = toolbox::string::suffixarray(s);
    return check(is_valid_sa(s, sa), "suffixarray all same: aaaa");
}

bool test_sa_single_char() {
    std::string s = "a";
    auto sa = toolbox::string::suffixarray(s);
    bool ok = true;
    ok &= check((int)sa.size() == 1, "single char SA size==1");
    ok &= check(sa[0] == 0,          "single char SA[0]==0");
    return ok;
}

bool test_sa_no_repeat() {
    std::string s = "abcdef";
    auto sa = toolbox::string::suffixarray(s);
    return check(is_valid_sa(s, sa), "suffixarray no repeat: abcdef");
}

bool test_sa_mississippi() {
    std::string s = "mississippi";
    auto sa = toolbox::string::suffixarray(s);
    return check(is_valid_sa(s, sa), "suffixarray mississippi");
}

bool test_sa_vs_doubling() {
    // Compare SAIS output with doubling: both should sort suffixes identically.
    std::string s = "abracadabra";
    int n = s.size();
    std::vector<int> sv(n);
    for (int i = 0; i < n; i++) sv[i] = s[i];
    auto sa_sais     = toolbox::string::suffixarray(s);
    auto sa_doubling = toolbox::string::helper::suffixarray_doubling(sv);
    bool ok = true;
    ok &= check(is_valid_sa(s, sa_sais), "SAIS valid for abracadabra");
    ok &= check((int)sa_sais.size() == n && (int)sa_doubling.size() == n,
                "both SA have size n");
    ok &= check(sa_sais == sa_doubling, "SAIS and doubling produce same SA");
    return ok;
}

// ---- lcp array -------------------------------------------------------------

// Build n+1 SA (with sentinel prepended) for lcp_array compatibility
std::vector<int> make_sa_np1(const std::string &s) {
    auto sa_n = toolbox::string::suffixarray(s);
    std::vector<int> sa_np1;
    sa_np1.push_back((int)s.size()); // sentinel position = n
    sa_np1.insert(sa_np1.end(), sa_n.begin(), sa_n.end());
    return sa_np1;
}

bool test_lcp_basic() {
    std::string s = "banana";
    auto sa_n   = toolbox::string::suffixarray(s);
    auto sa_np1 = make_sa_np1(s);
    auto lcp    = toolbox::string::lcp_array(s, sa_np1);
    return check(verify_lcp(s, sa_n, lcp), "lcp_array basic: banana");
}

bool test_lcp_all_same() {
    std::string s = "aaaa";
    auto sa_n   = toolbox::string::suffixarray(s);
    auto sa_np1 = make_sa_np1(s);
    auto lcp    = toolbox::string::lcp_array(s, sa_np1);
    return check(verify_lcp(s, sa_n, lcp), "lcp_array all same: aaaa");
}

bool test_lcp_mississippi() {
    std::string s = "mississippi";
    auto sa_n   = toolbox::string::suffixarray(s);
    auto sa_np1 = make_sa_np1(s);
    auto lcp    = toolbox::string::lcp_array(s, sa_np1);
    return check(verify_lcp(s, sa_n, lcp), "lcp_array mississippi");
}

bool test_lcp_no_repeat() {
    std::string s = "abcdef";
    auto sa_n   = toolbox::string::suffixarray(s);
    auto sa_np1 = make_sa_np1(s);
    auto lcp    = toolbox::string::lcp_array(s, sa_np1);
    // No shared prefixes -> all LCP values between non-sentinel pairs should be 0
    bool ok = true;
    for (int i = 1; i < (int)s.size(); i++)
        ok &= check(lcp[i] == 0, "lcp[" + std::to_string(i) + "]==0 for no-repeat");
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"sa_basic",       test_sa_basic},
        {"sa_all_same",    test_sa_all_same},
        {"sa_single_char", test_sa_single_char},
        {"sa_no_repeat",   test_sa_no_repeat},
        {"sa_mississippi", test_sa_mississippi},
        {"sa_vs_doubling", test_sa_vs_doubling},
        {"lcp_basic",      test_lcp_basic},
        {"lcp_all_same",   test_lcp_all_same},
        {"lcp_mississippi",test_lcp_mississippi},
        {"lcp_no_repeat",  test_lcp_no_repeat},
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
