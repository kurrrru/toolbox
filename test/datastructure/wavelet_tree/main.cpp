#include "toolbox/datastructure/wavelet_tree/wavelet_tree.hpp"
#include "test/test_util.hpp"

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <functional>

namespace {
bool test_access(std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree> test_case) {
    const auto &arr = test_case.first;
    const auto &wt = test_case.second;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        if (wt.access(i) != arr[i]) {
            std::cerr << "Access failed at index "
                << i 
                << ": expected " 
                << arr[i] 
                << ", got " 
                << wt.access(i) 
                << std::endl;
            return false;
        }
    }
    return true;
}

bool test_rank(std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree> test_case) {
    const auto &arr = test_case.first;
    const auto &wt = test_case.second;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        if (wt.rank(i, arr[i]) != std::count(arr.begin(), arr.begin() + i, arr[i])) {
            std::cerr << "Rank failed at index "
                << i 
                << ": expected " 
                << std::count(arr.begin(), arr.begin() + i, arr[i])
                << ", got "
                << wt.rank(i, arr[i])
                << std::endl;
            return false;
        }
    }
    return true;
}

bool test_range_count(std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree> test_case) {
    const auto &arr = test_case.first;
    const auto &wt = test_case.second;
    uint32_t lower = 3, upper = 7;
    std::size_t i = 2, j = 8;
    uint32_t expected_count = std::count_if(arr.begin() + i, arr.begin() + j, [lower, upper](uint32_t v) {
        return v >= lower && v < upper;
    });
    if (wt.range_count(i, j, lower, upper) != expected_count) {
        std::cerr << "Range count failed: expected " 
            << expected_count 
            << ", got " 
            << wt.range_count(i, j, lower, upper) 
            << std::endl;
        return false;
    }
    return true;
}

bool test_range_list(std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree> test_case) {
    const auto &arr = test_case.first;
    const auto &wt = test_case.second;
    uint32_t lower = 3, upper = 7;
    std::size_t i = 2, j = 8;
    std::set<uint32_t> expected_set;
    for (std::size_t k = i; k < j; ++k) {
        if (arr[k] >= lower && arr[k] < upper) {
            expected_set.insert(arr[k]);
        }
    }
    std::vector<uint32_t> expected_list(expected_set.begin(), expected_set.end());
    auto list = wt.range_list(i, j, lower, upper);
    if (list != expected_list) {
        std::cerr << "Range list failed: expected [";
        for (const auto &v : expected_list) {
            std::cerr << v << " ";
        }
        std::cerr << "], got [";
        for (const auto &v : list) {
            std::cerr << v << " ";
        }
        std::cerr << "]" << std::endl;
        return false;
    }
    return true;
}
}

int main()
{
    {
        std::cout << "Running tests on wavelet tree..." << std::endl;
        std::vector<uint32_t> arr = {1, 3, 5, 7, 2, 4, 6, 8, 3, 5, 7, 9, 2, 4, 6, 8, 1, 3, 5, 7, 
                                    2, 4, 6, 8, 3, 5, 7, 9, 1, 3, 5, 7};
        toolbox::datastructure::wavelet_tree wt(arr);

        toolbox::test_utils::runTests<std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree>>({
            {test_access, "Access Test"},
            {test_rank, "Rank Test"},
            {test_range_count, "Range Count Test"},
            {test_range_list, "Range List Test"}
        }, std::make_pair(arr, wt));
    }

    {
        std::cout << "Running tests on empty wavelet tree..." << std::endl;
        std::vector<uint32_t> empty_arr;
        toolbox::datastructure::wavelet_tree empty_wt(empty_arr);
        toolbox::test_utils::runTests<std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree>>({
            {test_access, "Access Test on Empty"},
            {test_rank, "Rank Test on Empty"},
        }, std::make_pair(empty_arr, empty_wt));
    }

    {
        std::cout << "Running tests on single element wavelet tree..." << std::endl;
        std::vector<uint32_t> single_element_arr = {42};
        toolbox::datastructure::wavelet_tree single_element_wt(single_element_arr);
        toolbox::test_utils::runTests<std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree>>({
            {test_access, "Access Test on Single Element"},
            {test_rank, "Rank Test on Single Element"},
        }, std::make_pair(single_element_arr, single_element_wt));
    }

    {
        std::cout << "Running tests on wavelet tree with all same elements..." << std::endl;
        std::vector<uint32_t> same_elements_arr(10000, 5); // 100 elements, all 5
        toolbox::datastructure::wavelet_tree same_elements_wt(same_elements_arr);
        toolbox::test_utils::runTests<std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree>>({
            {test_access, "Access Test on Same Elements"},
            {test_rank, "Rank Test on Same Elements"},
            {test_range_count, "Range Count Test on Same Elements"},
            {test_range_list, "Range List Test on Same Elements"}
        }, std::make_pair(same_elements_arr, same_elements_wt));
    }

    {
        std::cout << "Running tests on wavelet tree with large range of elements..." << std::endl;
        std::vector<uint32_t> large_range_arr;
        for (uint32_t i = 0; i < 10000; ++i) {
            large_range_arr.push_back(rand() % 10000);
        }
        toolbox::datastructure::wavelet_tree large_range_wt(large_range_arr);
        toolbox::test_utils::runTests<std::pair<std::vector<uint32_t>, toolbox::datastructure::wavelet_tree>>({
            {test_access, "Access Test on Large Range"},
            {test_rank, "Rank Test on Large Range"},
            {test_range_count, "Range Count Test on Large Range"},
            {test_range_list, "Range List Test on Large Range"}
        }, std::make_pair(large_range_arr, large_range_wt));
    }

    return 0;
}