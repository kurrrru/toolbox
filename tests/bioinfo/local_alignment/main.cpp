#include <string>

#include "toolbox/bioinfo/alignment/local_alignment/smith_waterman.hpp"

#include "utils/test_util.hpp"

namespace {

// Verify a local alignment: aligned strings must have equal length, and removing gap
// characters from each side must yield a contiguous substring of the corresponding original
// sequence (unlike global alignment, the full sequences need not be recovered).
bool is_valid_local_alignment(const std::string &s1, const std::string &s2, const std::string &a1,
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
    return s1.find(r1) != std::string::npos && s2.find(r2) != std::string::npos;
}

// ---- Smith-Waterman ---------------------------------------------------------
// SW default: a=1 (match score), x=1 (mismatch penalty), g=1 (gap penalty).
// Scores reflect similarity (higher is better).

bool test_sw_identical() {
    const std::string s = "ACGT";
    std::string a1, a2;
    int score = toolbox::alignment::smith_waterman_all(s, s, a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == static_cast<int>(s.size()),
                                     "SW identical: score == length");
    ok &= toolbox::test_utils::check(is_valid_local_alignment(s, s, a1, a2),
                                     "SW identical: alignment valid");
    return ok;
}

bool test_sw_substring_match() {
    // s1 appears verbatim inside s2, surrounded by unrelated characters.
    const std::string s1 = "GATTACA";
    const std::string s2 = "TTTTGATTACATTTT";
    std::string a1, a2;
    int score = toolbox::alignment::smith_waterman_all(s1, s2, a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == static_cast<int>(s1.size()),
                                     "SW substring: score == len(s1)");
    ok &= toolbox::test_utils::check(a1 == s1, "SW substring: a1 == s1 (no gaps needed)");
    ok &= toolbox::test_utils::check(is_valid_local_alignment(s1, s2, a1, a2),
                                     "SW substring: alignment valid");
    return ok;
}

bool test_sw_no_similarity() {
    // Disjoint alphabets: every cell floors to 0, so the best score stays 0.
    const std::string s1 = "AAAA";
    const std::string s2 = "TTTT";
    std::string a1, a2;
    int score = toolbox::alignment::smith_waterman_all(s1, s2, a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 0, "SW no similarity: score==0");
    ok &= toolbox::test_utils::check(a1.empty() && a2.empty(), "SW no similarity: empty alignment");
    return ok;
}

bool test_sw_empty_input() {
    std::string a1, a2;
    int score = toolbox::alignment::smith_waterman_all("", "ACGT", a1, a2, 1, 1, 1);
    bool ok = true;
    ok &= toolbox::test_utils::check(score == 0, "SW empty vs ACGT: score==0");
    ok &= toolbox::test_utils::check(a1.empty() && a2.empty(), "SW empty vs ACGT: empty alignment");
    return ok;
}

bool test_sw_general() {
    const std::string s1 = "ACACACTA";
    const std::string s2 = "AGCACACA";
    std::string a1, a2;
    int score = toolbox::alignment::smith_waterman_all(s1, s2, a1, a2, 2, 1, 2);
    bool ok = true;
    ok &= toolbox::test_utils::check(score > 0, "SW general: score > 0");
    ok &= toolbox::test_utils::check(is_valid_local_alignment(s1, s2, a1, a2),
                                     "SW general: alignment valid");
    return ok;
}

}  // namespace

int main() {
    toolbox::test_utils::Test tests[] = {
        {"sw_identical", test_sw_identical},
        {"sw_substring_match", test_sw_substring_match},
        {"sw_no_similarity", test_sw_no_similarity},
        {"sw_empty_input", test_sw_empty_input},
        {"sw_general", test_sw_general},
    };
    return toolbox::test_utils::run_tests(tests, sizeof(tests) / sizeof(tests[0]));
}
