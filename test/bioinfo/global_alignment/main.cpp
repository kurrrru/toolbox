#include "toolbox/bioinfo/alignment/global_alignment/nw.hpp"
#include "toolbox/bioinfo/alignment/global_alignment/nwg.hpp"
#include "toolbox/bioinfo/alignment/global_alignment/diff.hpp"
#include "toolbox/bioinfo/alignment/global_alignment/wavefront.hpp"
#include "test/test_util.hpp"

#include <iostream>
#include <string>

namespace {

bool check(bool cond, const std::string &label) {
    if (!cond) std::cerr << "  FAIL: " << label << "\n";
    return cond;
}

// Verify an alignment: aligned strings must have equal length, and when
// gap characters are removed, the original sequences are recovered.
bool is_valid_alignment(const std::string &s1, const std::string &s2,
                         const std::string &a1, const std::string &a2) {
    if (a1.size() != a2.size()) return false;
    std::string r1, r2;
    for (char c : a1) if (c != '-') r1 += c;
    for (char c : a2) if (c != '-') r2 += c;
    return r1 == s1 && r2 == s2;
}

// ---- Needleman-Wunsch -------------------------------------------------------
// NW default: a=1 (match cost), x=1 (mismatch cost), g=1 (gap cost).
// Scores reflect total cost (lower is better).

bool test_nw_identical() {
    // With a=0 (matches free), identical sequence costs 0.
    const std::string s = "ACGT";
    std::string a1, a2;
    // Use a=0 so match cost is zero, allowing meaningful score check.
    int score = toolbox::alignment::needleman_wunsch_all(s, s, a1, a2, 0, 1, 1);
    bool ok = true;
    ok &= check(score == 0, "NW identical (a=0): score==0");
    ok &= check(is_valid_alignment(s, s, a1, a2), "NW identical: alignment valid");
    return ok;
}

bool test_nw_empty_vs_string() {
    const std::string s2 = "ACG";
    std::string a1, a2;
    // gap cost=1 per character, 3 gaps needed
    int score = toolbox::alignment::needleman_wunsch_all("", s2, a1, a2, 0, 1, 1);
    bool ok = true;
    ok &= check(score == 3, "NW empty vs ACG (a=0): score==3");
    ok &= check(is_valid_alignment("", s2, a1, a2), "NW empty vs string: alignment valid");
    return ok;
}

bool test_nw_one_mismatch() {
    // "ACGT" vs "ACGA": 3 matches (free) + 1 mismatch (cost 1)
    const std::string s1 = "ACGT";
    const std::string s2 = "ACGA";
    std::string a1, a2;
    int score = toolbox::alignment::needleman_wunsch_all(s1, s2, a1, a2, 0, 1, 1);
    bool ok = true;
    ok &= check(score == 1, "NW one mismatch (a=0): score==1");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "NW one mismatch: alignment valid");
    return ok;
}

bool test_nw_general() {
    const std::string s1 = "ACGTAGATGCATG";
    const std::string s2 = "ACCTAGCATGCATC";
    std::string a1, a2;
    int score = toolbox::alignment::needleman_wunsch_all(s1, s2, a1, a2, 0, 1, 1);
    bool ok = true;
    ok &= check(score >= 0, "NW general: score >= 0");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "NW general: alignment valid");
    return ok;
}

bool test_nw_completely_different() {
    // "AAAA" vs "TTTT": 4 mismatches -> score == 4
    const std::string s1 = "AAAA";
    const std::string s2 = "TTTT";
    std::string a1, a2;
    int score = toolbox::alignment::needleman_wunsch_all(s1, s2, a1, a2, 0, 1, 1);
    bool ok = true;
    ok &= check(score == 4, "NW completely different (a=0): score==4");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "NW completely different: alignment valid");
    return ok;
}

// ---- Needleman-Wunsch-Gotoh ------------------------------------------------
// NWG default: a=1, x=1, o=0 (gap open), e=1 (gap extend).

bool test_nwg_identical() {
    const std::string s = "ACGT";
    std::string a1, a2;
    int score = toolbox::alignment::needleman_wunsch_gotoh_all(s, s, a1, a2, 0, 1, 0, 1);
    bool ok = true;
    ok &= check(score == 0, "NWG identical (a=0): score==0");
    ok &= check(is_valid_alignment(s, s, a1, a2), "NWG identical: alignment valid");
    return ok;
}

bool test_nwg_general() {
    const std::string s1 = "ACGTAGATGCATG";
    const std::string s2 = "ACCTAGCATGCATC";
    std::string a1, a2;
    int score = toolbox::alignment::needleman_wunsch_gotoh_all(s1, s2, a1, a2, 0, 1, 0, 1);
    bool ok = true;
    ok &= check(score >= 0, "NWG general: score >= 0");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "NWG general: alignment valid");
    return ok;
}

// ---- Diff ------------------------------------------------------------------
// Diff: x=1 (mismatch), g=1 (gap). Matches are always free.

bool test_diff_identical() {
    const std::string s = "ABCDE";
    std::string a1, a2;
    int score = toolbox::alignment::diff_all(s, s, a1, a2);
    bool ok = true;
    ok &= check(score == 0, "Diff identical: score==0");
    ok &= check(is_valid_alignment(s, s, a1, a2), "Diff identical: alignment valid");
    return ok;
}

bool test_diff_empty() {
    std::string a1, a2;
    int score = toolbox::alignment::diff_all("", "ABC", a1, a2);
    bool ok = true;
    ok &= check(score == 3, "Diff empty vs ABC: score==3");
    ok &= check(is_valid_alignment("", "ABC", a1, a2), "Diff empty vs ABC: alignment valid");
    return ok;
}

bool test_diff_general() {
    const std::string s1 = "ACGTAGATGCATG";
    const std::string s2 = "ACCTAGCATGCATC";
    std::string a1, a2;
    int score = toolbox::alignment::diff_all(s1, s2, a1, a2);
    bool ok = true;
    ok &= check(score >= 0, "Diff general: score >= 0");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "Diff general: alignment valid");
    return ok;
}

// ---- Wavefront -------------------------------------------------------------
// Wavefront: x=1, o=0, e=1. Matches are always free.

bool test_wavefront_identical() {
    const std::string s = "ACGT";
    std::string a1, a2;
    int score = toolbox::alignment::wavefront_all(s, s, a1, a2);
    bool ok = true;
    ok &= check(score == 0, "Wavefront identical: score==0");
    ok &= check(is_valid_alignment(s, s, a1, a2), "Wavefront identical: alignment valid");
    return ok;
}

bool test_wavefront_general() {
    const std::string s1 = "ACGTAGATGCATG";
    const std::string s2 = "ACCTAGCATGCATC";
    std::string a1, a2;
    int score = toolbox::alignment::wavefront_all(s1, s2, a1, a2);
    bool ok = true;
    ok &= check(score >= 0, "Wavefront general: score >= 0");
    ok &= check(is_valid_alignment(s1, s2, a1, a2), "Wavefront general: alignment valid");
    return ok;
}

// ---- Cross-algorithm consistency -------------------------------------------
// With matching parameters (a=0 for NW/NWG, matches free for diff/wavefront),
// all algorithms should agree on edit distance for simple linear gap costs.

bool test_score_consistency() {
    const std::string s1 = "AGCTGATCGA";
    const std::string s2 = "AGCTGATCGT";  // 1 mismatch at end
    std::string a1, a2;
    // Use a=0, x=1, g=1 for NW; a=0, x=1, o=0, e=1 for NWG
    int nw  = toolbox::alignment::needleman_wunsch_all(s1, s2, a1, a2, 0, 1, 1);
    int nwg = toolbox::alignment::needleman_wunsch_gotoh_all(s1, s2, a1, a2, 0, 1, 0, 1);
    int df  = toolbox::alignment::diff_all(s1, s2, a1, a2);
    int wf  = toolbox::alignment::wavefront_all(s1, s2, a1, a2);
    bool ok = true;
    ok &= check(nw == 1,  "NW one-mismatch score==1");
    ok &= check(df == 1,  "Diff one-mismatch score==1");
    ok &= check(wf == 1,  "Wavefront one-mismatch score==1");
    ok &= check(nw == df, "NW score == Diff score (linear gap)");
    ok &= check(nw == wf, "NW score == Wavefront score (linear gap)");
    ok &= check(nwg <= nw, "NWG score <= NW score (affine gap never worse)");
    return ok;
}

struct Test { std::string name; bool (*fn)(); };

} // namespace

int main() {
    Test tests[] = {
        {"nw_identical",           test_nw_identical},
        {"nw_empty_vs_string",     test_nw_empty_vs_string},
        {"nw_one_mismatch",        test_nw_one_mismatch},
        {"nw_general",             test_nw_general},
        {"nw_completely_different",test_nw_completely_different},
        {"nwg_identical",          test_nwg_identical},
        {"nwg_general",            test_nwg_general},
        {"diff_identical",         test_diff_identical},
        {"diff_empty",             test_diff_empty},
        {"diff_general",           test_diff_general},
        {"wavefront_identical",    test_wavefront_identical},
        {"wavefront_general",      test_wavefront_general},
        {"score_consistency",      test_score_consistency},
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
