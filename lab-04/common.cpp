#include "common.h"

std::vector<std::uint8_t> get_slice(const std::vector<uint8_t>& v, int left, int right) {
    std::vector<uint8_t> res;
    res.reserve(right - left);

    for (int i = left; i < right; ++i) {
        res.push_back(v[i]);
    }

    return res;
}
