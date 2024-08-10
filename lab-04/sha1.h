#ifndef sha1_H
#define sha1_H

#include <vector>
#include <tuple>
#include <cstdint>
#include "ap.hpp"

class sha1
{
public:
    sha1();

    ap_int<256> calculate_hash(std::vector<uint8_t>& data);
};

#endif // sha1_H
