#include <toolbox/string/fm_index.hpp>
#include <test/test_util.hpp>

#include <iostream>
#include <string>
#include <algorithm>

namespace {
bool test_count(const std::pair<std::string, std::vector<std::string>> &test_case) {
    const std::string &text = test_case.first;
    const std::vector<std::string> &patterns = test_case.second;
    bool all_passed = true;
    toolbox::string::fm_index index(text);
    for (const auto &pattern : patterns) {
        int count = index.count(pattern);
        int expected_count = 0;
        size_t pos = text.find(pattern);
        while (pos != std::string::npos) {
            expected_count++;
            pos = text.find(pattern, pos + 1);
        }
        if (count != expected_count) {
            std::cerr << "Count test failed for\n- text    '" << text
                    << "'\n- pattern '" << pattern << "'\n- expected " 
                    << expected_count << "\n- got      " << count << std::endl;
            all_passed = false;
        }
    }
    return all_passed;
}

bool test_locate(const std::pair<std::string, std::vector<std::string>> &test_case) {
    const std::string &text = test_case.first;
    const std::vector<std::string> &patterns = test_case.second;
    bool all_passed = true;
    toolbox::string::fm_index index(text);
    for (const auto &pattern : patterns) {
        std::vector<int> positions = index.locate(pattern);
        std::vector<int> expected_positions;
        size_t pos = text.find(pattern);
        while (pos != std::string::npos) {
            expected_positions.push_back(pos);
            pos = text.find(pattern, pos + 1);
        }
        std::sort(expected_positions.begin(), expected_positions.end());
        std::sort(positions.begin(), positions.end());
        if (positions != expected_positions) {
            std::cerr << "Locate test failed for \n- text    '" << text
                    << "'\n- pattern '" << pattern << "'\n- expected [";
            for (std::size_t i = 0; i < expected_positions.size(); ++i) {
                std::cerr << expected_positions[i];
                if (i < expected_positions.size() - 1) {
                    std::cerr << " ";
                }
            }
            std::cerr << "]\n- got      [";
            for (std::size_t i = 0; i < positions.size(); ++i) {
                std::cerr << positions[i];
                if (i < positions.size() - 1) {
                    std::cerr << " ";
                }
            }
            std::cerr << "]" << std::endl;
            all_passed = false;
        }
    }
    return all_passed;
}

bool test_count_order(const std::tuple<std::string, std::vector<std::string>, std::string> &test_case) {
    const std::string &text = std::get<0>(test_case);
    const std::vector<std::string> &patterns = std::get<1>(test_case);
    const std::string &order = std::get<2>(test_case);
    bool all_passed = true;
    toolbox::string::fm_index index(text, order);
    for (const auto &pattern : patterns) {
        int count = index.count(pattern);
        int expected_count = 0;
        size_t pos = text.find(pattern);
        while (pos != std::string::npos) {
            expected_count++;
            pos = text.find(pattern, pos + 1);
        }
        if (count != expected_count) {
            std::cerr << "Count test failed for\n- text    '" << text
                    << "'\n- pattern '" << pattern << "'\n- expected " 
                    << expected_count << "\n- got      " << count << std::endl;
            all_passed = false;
        }
    }
    return all_passed;
}

bool test_locate_order(const std::tuple<std::string, std::vector<std::string>, std::string> &test_case) {
    const std::string &text = std::get<0>(test_case);
    const std::vector<std::string> &patterns = std::get<1>(test_case);
    const std::string &order = std::get<2>(test_case);
    bool all_passed = true;
    toolbox::string::fm_index index(text, order);
    for (const auto &pattern : patterns) {
        std::vector<int> positions = index.locate(pattern);
        std::vector<int> expected_positions;
        size_t pos = text.find(pattern);
        while (pos != std::string::npos) {
            expected_positions.push_back(pos);
            pos = text.find(pattern, pos + 1);
        }
        std::sort(expected_positions.begin(), expected_positions.end());
        std::sort(positions.begin(), positions.end());
        if (positions != expected_positions) {
            std::cerr << "Locate test failed for \n- text    '" << text
                    << "'\n- pattern '" << pattern << "'\n- expected [";
            for (std::size_t i = 0; i < expected_positions.size(); ++i) {
                std::cerr << expected_positions[i];
                if (i < expected_positions.size() - 1) {
                    std::cerr << " ";
                }
            }
            std::cerr << "]\n- got      [";
            for (std::size_t i = 0; i < positions.size(); ++i) {
                std::cerr << positions[i];
                if (i < positions.size() - 1) {
                    std::cerr << " ";
                }
            }
            std::cerr << "]" << std::endl;
            all_passed = false;
        }
    }
    return all_passed;
}

}



int main() {
    std::string text = "ACGTACGTACGCAACCTCGATCGATCGTACGTACGTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGC";
    std::vector<std::string> patterns = {"", "A", "C", "GT", "ACGT", "GCA", "CTAG", "TACG", "GATCG", "AGCTAGC", "AAA"};
    toolbox::test_utils::runTests<std::pair<std::string, std::vector<std::string>>>({
        {test_count, "Count Test"},
        {test_locate, "Locate Test"}
    }, std::make_pair(text, patterns));

    std::string order = "GTCA";
    toolbox::test_utils::runTests<std::tuple<std::string, std::vector<std::string>, std::string>>({
        {test_count_order, "Count Test with Order"},
        {test_locate_order, "Locate Test with Order"}
    }, std::make_tuple(text, patterns, order));

    std::string empty_string = "";
    std::vector<std::string> empty_patterns = {"A", "C", "G", "T"};
    toolbox::test_utils::runTests<std::pair<std::string, std::vector<std::string>>>({
        {test_count, "Count Test on Empty String"},
        {test_locate, "Locate Test on Empty String"}
    }, std::make_pair(empty_string, empty_patterns));


    text = "AAAAhorgh sacace";
    patterns = {"A", "AA", "AAA", "AAAA", "B"};
    order = "CGT";
    toolbox::test_utils::runTests<std::tuple<std::string, std::vector<std::string>, std::string>>({
        {test_count_order, "Count Test on Repeated Characters"},
        {test_locate_order, "Locate Test on Repeated Characters"}
    }, std::make_tuple(text, patterns, order));


    return 0;
}
