#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <cstdint>

std::vector<std::uint8_t> get_slice(const std::vector<uint8_t>& v, int left, int right);

#endif // COMMON_H
