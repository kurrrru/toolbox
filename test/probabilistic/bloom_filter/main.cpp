#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include "toolbox/probabilistic/bloom_filter/bloom_filter.hpp"
#include "test/test_util.hpp"

int main() {
    toolbox::datastructure::BloomFilter<1000, 5> bf;

    std::vector<std::string> keys = {"apple", "orange", "banana", "grape", "melon"};
    std::vector<std::string> others = {"peach", "strawberry", "blueberry", "kiwi"};

    for (const auto& key : keys) {
        bf.add(key);
    }

    for (const auto& key : keys) {
        assert(bf.contains(key) == true);
    }

    int false_positives = 0;
    for (const auto& other : others) {
        if (bf.contains(other)) {
            false_positives++;
        }
    }

    std::cout << "Keys added: " << keys.size() << std::endl;
    std::cout << "Verification passed for all keys." << std::endl;
    std::cout << "False positives found: " << false_positives << " / " << others.size() << std::endl;

    bf.add("12345");
    assert(bf.contains("12345") == true);

    std::cout << toolbox::color::green
                << "All tests passed successfully!"
                << toolbox::color::reset 
                << std::endl;

    return 0;
}
