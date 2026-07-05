#include "test/test_util.hpp"
#include "toolbox/sorting/sorting.hpp"

#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace {

// ---- Test data -----------------------------------------------------------

std::vector<int> make_random(std::size_t n, unsigned seed = 42) {
    std::vector<int> v(n);
    unsigned s = seed;
    for (std::size_t i = 0; i < n; ++i) {
        s = s * 1664525u + 1013904223u;
        v[i] = static_cast<int>(s % 10000u);
    }
    return v;
}

std::vector<int> make_sorted(std::size_t n) {
    std::vector<int> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = static_cast<int>(i);
    return v;
}

std::vector<int> make_reverse(std::size_t n) {
    std::vector<int> v = make_sorted(n);
    std::reverse(v.begin(), v.end());
    return v;
}

std::vector<int> make_duplicates(std::size_t n) {
    std::vector<int> v(n, 7);
    for (std::size_t i = 0; i < n; i += 3) v[i] = 3;
    for (std::size_t i = 0; i < n; i += 5) v[i] = 5;
    return v;
}

// ---- Generic sort tester -------------------------------------------------

typedef void (*SortFn)(std::vector<int>::iterator, std::vector<int>::iterator);
typedef void (*SortFnComp)(std::vector<int>::iterator, std::vector<int>::iterator,
                           std::greater<int>);

bool is_sorted_asc(const std::vector<int> &v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i] < v[i - 1]) return false;
    }
    return true;
}

bool is_sorted_desc(const std::vector<int> &v) {
    for (std::size_t i = 1; i < v.size(); ++i) {
        if (v[i] > v[i - 1]) return false;
    }
    return true;
}

bool is_permutation_of(std::vector<int> a, std::vector<int> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

struct SortTestCase {
    std::string name;
    std::vector<int> data;
};

template <typename SortFunc>
bool run_sort_test(const SortTestCase &tc, SortFunc sort_fn) {
    std::vector<int> original = tc.data;
    std::vector<int> v = original;
    sort_fn(v.begin(), v.end());
    if (!is_sorted_asc(v)) {
        std::cerr << "  [" << tc.name << "] Result is not sorted in ascending order\n";
        return false;
    }
    if (!is_permutation_of(v, original)) {
        std::cerr << "  [" << tc.name << "] Result is not a permutation of the input\n";
        return false;
    }
    return true;
}

template <typename SortFuncComp>
bool run_sort_test_desc(const SortTestCase &tc, SortFuncComp sort_fn) {
    std::vector<int> original = tc.data;
    std::vector<int> v = original;
    sort_fn(v.begin(), v.end(), std::greater<int>());
    if (!is_sorted_desc(v)) {
        std::cerr << "  [" << tc.name << "] Result is not sorted in descending order\n";
        return false;
    }
    if (!is_permutation_of(v, original)) {
        std::cerr << "  [" << tc.name << "] Result is not a permutation of the input\n";
        return false;
    }
    return true;
}

std::vector<SortTestCase> make_test_cases() {
    std::vector<SortTestCase> cases;

    {
        SortTestCase tc;
        tc.name = "empty";
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "single";
        tc.data = std::vector<int>(1, 42);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "two_elements";
        tc.data.push_back(2);
        tc.data.push_back(1);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "already_sorted_10";
        tc.data = make_sorted(10);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "reverse_sorted_10";
        tc.data = make_reverse(10);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "random_10";
        tc.data = make_random(10);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "duplicates_20";
        tc.data = make_duplicates(20);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "random_100";
        tc.data = make_random(100, 123);
        cases.push_back(tc);
    }
    {
        SortTestCase tc;
        tc.name = "random_1000";
        tc.data = make_random(10000, 999);
        cases.push_back(tc);
    }
    return cases;
}

// ---- Per-algorithm test wrappers -----------------------------------------

// Ascending (default comparator)
bool test_bubble_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::bubble_sort<std::vector<int>::iterator>);
}
bool test_insertion_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::insertion_sort<std::vector<int>::iterator>);
}
bool test_binary_insertion_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::binary_insertion_sort<std::vector<int>::iterator>);
}
bool test_selection_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::selection_sort<std::vector<int>::iterator>);
}
bool test_shell_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::shell_sort<std::vector<int>::iterator>);
}
bool test_comb_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::comb_sort<std::vector<int>::iterator>);
}
bool test_gnome_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::gnome_sort<std::vector<int>::iterator>);
}
bool test_shaker_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::shaker_sort<std::vector<int>::iterator>);
}
bool test_odd_even_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::odd_even_sort<std::vector<int>::iterator>);
}
bool test_cycle_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::cycle_sort<std::vector<int>::iterator>);
}
bool test_merge_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::merge_sort<std::vector<int>::iterator>);
}
bool test_quick_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::quick_sort<std::vector<int>::iterator>);
}
bool test_heap_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::heap_sort<std::vector<int>::iterator>);
}
bool test_intro_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::intro_sort<std::vector<int>::iterator>);
}
bool test_tim_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::tim_sort<std::vector<int>::iterator>);
}
bool test_patience_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::patience_sort<std::vector<int>::iterator>);
}
bool test_tournament_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::tournament_sort<std::vector<int>::iterator>);
}
bool test_tree_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::tree_sort<std::vector<int>::iterator>);
}
bool test_cartesian_tree_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::cartesian_tree_sort<std::vector<int>::iterator>);
}
bool test_ternary_split_quick_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc,
                         toolbox::sorting::ternary_split_quick_sort<std::vector<int>::iterator>);
}
bool test_merge_insertion_sort_asc(const SortTestCase &tc) {
    return run_sort_test(tc, toolbox::sorting::merge_insertion_sort<std::vector<int>::iterator>);
}

// Descending (custom comparator)
bool test_bubble_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::bubble_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_insertion_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::insertion_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_binary_insertion_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::binary_insertion_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_selection_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::selection_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_shell_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::shell_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_comb_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::comb_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_gnome_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::gnome_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_shaker_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::shaker_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_odd_even_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::odd_even_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_cycle_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::cycle_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_merge_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::merge_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_quick_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::quick_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_heap_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::heap_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_intro_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::intro_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_tim_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::tim_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_patience_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::patience_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_tournament_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::tournament_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_tree_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::tree_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_cartesian_tree_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::cartesian_tree_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_ternary_split_quick_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc,
        toolbox::sorting::ternary_split_quick_sort<std::vector<int>::iterator, std::greater<int>>);
}
bool test_merge_insertion_sort_desc(const SortTestCase &tc) {
    return run_sort_test_desc(
        tc, toolbox::sorting::merge_insertion_sort<std::vector<int>::iterator, std::greater<int>>);
}

// ---- Algorithm test runner -----------------------------------------------

struct AlgoTest {
    std::string name;
    bool (*fn_asc)(const SortTestCase &);
    bool (*fn_desc)(const SortTestCase &);
};

bool run_algorithm(const AlgoTest &algo, const std::vector<SortTestCase> &cases) {
    bool all_ok = true;
    for (std::size_t i = 0; i < cases.size(); ++i) {
        if (!algo.fn_asc(cases[i])) {
            std::cerr << "  FAIL asc  [" << cases[i].name << "]\n";
            all_ok = false;
        }
        if (!algo.fn_desc(cases[i])) {
            std::cerr << "  FAIL desc [" << cases[i].name << "]\n";
            all_ok = false;
        }
    }
    return all_ok;
}

}  // namespace

int main() {
    std::vector<SortTestCase> cases = make_test_cases();

    AlgoTest algos[] = {
        {"bubble_sort", test_bubble_sort_asc, test_bubble_sort_desc},
        {"insertion_sort", test_insertion_sort_asc, test_insertion_sort_desc},
        {"binary_insertion_sort", test_binary_insertion_sort_asc, test_binary_insertion_sort_desc},
        {"selection_sort", test_selection_sort_asc, test_selection_sort_desc},
        {"shell_sort", test_shell_sort_asc, test_shell_sort_desc},
        {"comb_sort", test_comb_sort_asc, test_comb_sort_desc},
        {"gnome_sort", test_gnome_sort_asc, test_gnome_sort_desc},
        {"shaker_sort", test_shaker_sort_asc, test_shaker_sort_desc},
        {"odd_even_sort", test_odd_even_sort_asc, test_odd_even_sort_desc},
        {"cycle_sort", test_cycle_sort_asc, test_cycle_sort_desc},
        {"merge_sort", test_merge_sort_asc, test_merge_sort_desc},
        {"quick_sort", test_quick_sort_asc, test_quick_sort_desc},
        {"heap_sort", test_heap_sort_asc, test_heap_sort_desc},
        {"intro_sort", test_intro_sort_asc, test_intro_sort_desc},
        {"tim_sort", test_tim_sort_asc, test_tim_sort_desc},
        {"patience_sort", test_patience_sort_asc, test_patience_sort_desc},
        {"tournament_sort", test_tournament_sort_asc, test_tournament_sort_desc},
        {"tree_sort", test_tree_sort_asc, test_tree_sort_desc},
        {"cartesian_tree_sort", test_cartesian_tree_sort_asc, test_cartesian_tree_sort_desc},
        {"ternary_split_quick_sort", test_ternary_split_quick_sort_asc,
         test_ternary_split_quick_sort_desc},
        {"merge_insertion_sort", test_merge_insertion_sort_asc, test_merge_insertion_sort_desc},
    };
    const std::size_t num_algos = sizeof(algos) / sizeof(algos[0]);

    int pass = 0, fail = 0;
    for (std::size_t a = 0; a < num_algos; ++a) {
        bool ok = run_algorithm(algos[a], cases);
        if (ok) {
            std::cout << toolbox::color::cyan << "PASS " << algos[a].name << toolbox::color::reset
                      << "\n";
            ++pass;
        } else {
            std::cout << toolbox::color::yellow << "FAIL " << algos[a].name << toolbox::color::reset
                      << "\n";
            ++fail;
        }
    }

    std::cout << "\n";
    if (fail == 0) {
        std::cout << toolbox::color::green << "All " << pass << " algorithms passed all tests!"
                  << toolbox::color::reset << "\n";
    } else {
        std::cout << toolbox::color::red << fail << " out of " << (pass + fail)
                  << " algorithms had failures." << toolbox::color::reset << "\n";
    }
    return fail == 0 ? 0 : 1;
}
