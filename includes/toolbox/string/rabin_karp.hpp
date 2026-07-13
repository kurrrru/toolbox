#pragma once

#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

namespace toolbox {

namespace string {

namespace hash {

/**
 * @brief Rolling hash over a fixed string, supporting O(1) hashing of any substring after
 * O(n) preprocessing.
 * @note Hashes are computed modulo the Mersenne prime 2^61 - 1, with a fixed base. This
 *       modulus is large enough that accidental collisions are astronomically unlikely for
 *       non-adversarial input; it is not randomized per instance, so results are reproducible
 *       across runs, but a party who knows the fixed base/modulus in advance could in
 *       principle construct a colliding input. Callers who need a correctness guarantee
 *       regardless of collisions (e.g. rabin_karp_search below) should verify a hash match
 *       with a direct comparison.
 * @note 128-bit integer multiplication (via __int128) is used to reduce modulo 2^61 - 1
 *       without overflow; __int128 is a GCC/Clang extension, matching the compilers this
 *       project targets (see README).
 * @note [Complexity]: O(n) to construct, O(1) per get() query.
 */
class RollingHash {
 public:
    explicit RollingHash(const std::string &s) : _hash(s.size() + 1, 0), _power(s.size() + 1, 1) {
        for (std::size_t i = 0; i < s.size(); ++i) {
            _hash[i + 1] = add_mod(mul_mod(_hash[i], kBase), static_cast<std::uint64_t>(s[i]));
            _power[i + 1] = mul_mod(_power[i], kBase);
        }
    }

    // Returns the hash of the substring [l, r) (0-based, half-open).
    std::uint64_t get(std::size_t l, std::size_t r) const {
        assert(l <= r && r < _hash.size());
        const std::uint64_t prefix_contribution = mul_mod(_hash[l], _power[r - l]);
        return add_mod(_hash[r], kMod - prefix_contribution);
    }

    // Total length of the underlying string.
    std::size_t size() const { return _hash.size() - 1; }

 private:
    static constexpr std::uint64_t kMod = (1ULL << 61) - 1;
    static constexpr std::uint64_t kBase = 999999937ULL;

    static std::uint64_t add_mod(std::uint64_t a, std::uint64_t b) {
        std::uint64_t sum = a + b;
        if (sum >= kMod) {
            sum -= kMod;
        }
        return sum;
    }

    static std::uint64_t mul_mod(std::uint64_t a, std::uint64_t b) {
        return static_cast<std::uint64_t>((static_cast<__uint128_t>(a) * b) % kMod);
    }

    std::vector<std::uint64_t> _hash;   // _hash[i] = hash of the prefix of length i
    std::vector<std::uint64_t> _power;  // _power[i] = kBase^i mod kMod
};

}  // namespace hash

/**
 * @brief Finds all occurrences of a pattern in a text using the Rabin-Karp algorithm.
 * @param text The text to search in.
 * @param pattern The pattern to search for.
 * @return 0-based start indices where pattern occurs in text (ascending order).
 * @note Every hash match is verified with a direct character comparison before being
 *       reported, so the result is always exactly correct regardless of hash collisions;
 *       see hash::RollingHash above for the (fixed base, non-adversarial) hash used.
 * @note [Complexity]: O(|text| + |pattern|) average time (assuming few hash collisions),
 *       O(|text| * |pattern|) worst case (if verification is repeatedly triggered by
 *       collisions), O(|text| + |pattern|) space.
 *       An empty pattern is treated as matching at every position 0..|text|.
 */
std::vector<int> rabin_karp_search(const std::string &text, const std::string &pattern) {
    std::vector<int> positions;
    const int n = static_cast<int>(text.size());
    const int m = static_cast<int>(pattern.size());
    if (m == 0) {
        positions.reserve(n + 1);
        for (int i = 0; i <= n; ++i) {
            positions.push_back(i);
        }
        return positions;
    }
    if (m > n) {
        return positions;
    }
    const hash::RollingHash text_hash(text);
    const hash::RollingHash pattern_hash(pattern);
    const std::uint64_t target = pattern_hash.get(0, m);
    for (int i = 0; i + m <= n; ++i) {
        if (text_hash.get(i, i + m) == target && text.compare(i, m, pattern) == 0) {
            positions.push_back(i);
        }
    }
    return positions;
}

}  // namespace string

}  // namespace toolbox
