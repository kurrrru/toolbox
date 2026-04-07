#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <functional>
#include <cstdint>

namespace toolbox
{

namespace datastructure
{

/**
 * @brief SplitMix64 based avalanche mixer.
 * Transforms a 64-bit input into a highly-scattered output.
 */
struct SplitMix64Mixer {
    inline uint64_t operator()(uint64_t x) const noexcept {
        x ^= x >> 30;
        x *= 0xbf58476d1ce4e5b9ULL;
        x ^= x >> 27;
        x *= 0x94d049bb133111ebULL;
        x ^= x >> 31;
        return x;
    }
};

template <std::size_t Size, std::size_t HashCount>
class BloomFilter {
 public:
    void add(const std::string& item) {
        uint64_t base_hash = std::hash<std::string>{}(item);
        for (std::size_t i = 0; i < HashCount; ++i) {
            bits.set(get_hash(base_hash, i));
        }
    }

    // [NOTE] THIS FUNCTION MAY RETURN TRUE FOR NON-EXISTING ITEMS (FALSE POSITIVE)
    //        BUT IT WILL NEVER RETURN FALSE FOR EXISTING ITEMS.
    bool contains(const std::string& item) const {
        uint64_t base_hash = std::hash<std::string>{}(item);
        for (std::size_t i = 0; i < HashCount; ++i) {
            if (!bits.test(get_hash(base_hash, i))) {
                return false;
            }
        }
        return true;
    }

 private:
    std::bitset<Size> bits;
    SplitMix64Mixer mixer;

    // [NOTE] ここの計算は工夫の余地があるかも?
    inline std::size_t get_hash(uint64_t base, std::size_t i) const noexcept {
        return mixer(base + i) % Size;
    }
};

}  // namespace datastructure

}  // namespace toolbox
