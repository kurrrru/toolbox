#include <toolbox/string/fm_index.hpp>

#include <iostream>
#include <string>

int main() {
    std::string seq = "ACGTACGTACGCAACCTCGATCGATCGTACGTACGTAGCTAGCTAGCTAGCTAGCTAGCTAGCTAGC";
    std::string order = "ACGT";
    toolbox::string::fm_index index(seq, order);
    std::string pattern = "GATCG";
    int count = index.count(pattern);
    std::vector<int> positions = index.locate(pattern);
    std::cout << "Count of pattern '" << pattern << "': " << count << std::endl;
    std::cout << "Positions of pattern '" << pattern << "': ";
    for (int pos : positions) {
        std::cout << pos << " ";
    }
    std::cout << std::endl;
    return 0;
}
