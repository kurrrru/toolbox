#include "../../../toolbox/bioinfo/alignment/global_alignment/nw.hpp"
#include "../../../toolbox/bioinfo/alignment/global_alignment/nwg.hpp"
#include "../../../toolbox/bioinfo/alignment/global_alignment/diff.hpp"
#include "../../../toolbox/bioinfo/alignment/global_alignment/wavefront.hpp"

#include <iostream>
#include <string>

int main() {
    const std::string seq1 = "ACGTAGATGCATG";
    const std::string seq2 = "ACCTAGCATGCATC";
    std::string aligned_seq1, aligned_seq2;

    // Needleman-Wunsch Global Alignment
    auto alignment_nw = toolbox::alignment::needleman_wunsch_all(seq1, seq2, 
        aligned_seq1, aligned_seq2);
    std::cout << "Needleman-Wunsch Alignment:\n";
    std::cout << "Aligned Seq1: " << aligned_seq1 << "\n";
    std::cout << "Aligned Seq2: " << aligned_seq2 << "\n";
    std::cout << "Alignment Score: " << alignment_nw << "\n\n";

    // Needleman-Wunsch Global Alignment with Gaps
    auto alignment_nwg = toolbox::alignment::needleman_wunsch_gotoh_all(seq1, seq2, 
        aligned_seq1, aligned_seq2);
    std::cout << "Needleman-Wunsch with Gaps Alignment:\n";
    std::cout << "Aligned Seq1: " << aligned_seq1 << "\n";
    std::cout << "Aligned Seq2: " << aligned_seq2 << "\n";
    std::cout << "Alignment Score: " << alignment_nwg << "\n\n";

    // Diff Algorithm for Global Alignment
    auto alignment_diff = toolbox::alignment::diff_all(seq1, seq2, 
        aligned_seq1, aligned_seq2);
    std::cout << "Diff Algorithm Alignment:\n";
    std::cout << "Aligned Seq1: " << aligned_seq1 << "\n";
    std::cout << "Aligned Seq2: " << aligned_seq2 << "\n";
    std::cout << "Alignment Score: " << alignment_diff << "\n\n";

    // Wavefront Alignment
    auto alignment_wavefront = toolbox::alignment::wavefront_all(seq1, seq2, 
        aligned_seq1, aligned_seq2);
    std::cout << "Wavefront Alignment:\n";
    std::cout << "Aligned Seq1: " << aligned_seq1 << "\n";
    std::cout << "Aligned Seq2: " << aligned_seq2 << "\n";
    std::cout << "Alignment Score: " << alignment_wavefront << "\n";
    return 0;
}