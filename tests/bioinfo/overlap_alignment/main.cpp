#include <string>

#include "toolbox/bioinfo/alignment/overlap_alignment/overlap.hpp"

#include "utils/test_util.hpp"

namespace {

// Verify an overlap alignment: aligned strings must have equal length, removing gap
// characters from a1 must yield a suffix of s1, and removing gap characters from a2 must
// yield a prefix of s2.
bool is_valid_overlap_alignment(const std::string &s1, const std::string &s2, const std::string &a1,
                                const std::string &a2) {
    if (a1.size() != a2.size()) {
        return false;
    }
    std::string r1, r2;
    for (char c : a1) {
        if (c != '-') {
            r1 += c;
        }
    }
    for (char c : a2) {
        if (c != '-') {
            r2 += c;
        }
    }
    bool suffix_ok =
        r1.size() <= s1.size() && s1.compare(s1.size() - r1.size(), r1.size(), r1) == 0;
    bool prefix_ok = r2.size() <= s2.size() && s2.compare(0, r2.size(), r2) == 0;
    return suffix_ok && prefix_ok;
}

// ---- Overlap alignment -------------------------------------------------------
// Overlap default: a=1 (match score), x=1 (mismatch penalty), g=1 (gap penalty).
// Scores reflect similarity (higher is better).

bool test_overlap_basic() {
    // Suffix "CCGT" of s1 matches prefix "CCGT" of s2 exactly.
    const std::string s1 = "AAACCGT";
    const std::string s2 = "CCGTGGG";
    std::string a1, a2;
    int score = toolbox::alignment::overlap_all(s1, s2, a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 4, "Overlap basic: score==4");
    ok &= toolbox::test_utils::check(a1 == "CCGT", "Overlap basic: a1 == CCGT");
    ok &= toolbox::test_utils::check(a2 == "CCGT", "Overlap basic: a2 == CCGT");
    ok &= toolbox::test_utils::check(is_valid_overlap_alignment(s1, s2, a1, a2),
                                     "Overlap basic: alignment valid");
    return ok;
}

bool test_overlap_no_overlap() {
    // Disjoint alphabets: any non-empty overlap only loses score, so the trivial empty
    // overlap (score 0) wins.
    const std::string s1 = "AAAA";
    const std::string s2 = "TTTT";
    std::string a1, a2;
    int score = toolbox::alignment::overlap_all(s1, s2, a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 0, "Overlap no overlap: score==0");
    ok &=
        toolbox::test_utils::check(a1.empty() && a2.empty(), "Overlap no overlap: empty alignment");
    return ok;
}

bool test_overlap_empty_s1() {
    std::string a1, a2;
    int score = toolbox::alignment::overlap_all("", "ACGT", a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 0, "Overlap empty s1: score==0");
    ok &= toolbox::test_utils::check(a1.empty() && a2.empty(), "Overlap empty s1: empty alignment");
    return ok;
}

bool test_overlap_empty_s2() {
    std::string a1, a2;
    int score = toolbox::alignment::overlap_all("ACGT", "", a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 0, "Overlap empty s2: score==0");
    ok &= toolbox::test_utils::check(a1.empty() && a2.empty(), "Overlap empty s2: empty alignment");
    return ok;
}

bool test_overlap_general() {
    const std::string s1 = "TAGCTAGGA";
    const std::string s2 = "AGGATCCGA";
    std::string a1, a2;
    int score = toolbox::alignment::overlap_all(s1, s2, a1, a2, 2, 1, 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(score > 0, "Overlap general: score > 0");
    ok &= toolbox::test_utils::check(is_valid_overlap_alignment(s1, s2, a1, a2),
                                     "Overlap general: alignment valid");
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"overlap_basic", test_overlap_basic},
        {"overlap_no_overlap", test_overlap_no_overlap},
        {"overlap_empty_s1", test_overlap_empty_s1},
        {"overlap_empty_s2", test_overlap_empty_s2},
        {"overlap_general", test_overlap_general},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
