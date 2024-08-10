//
// Created by vladislav on 07.10.2023.
//

#ifndef LAB_01_ENCODER_H
#define LAB_01_ENCODER_H


#include <cstddef>
#include <vector>
#include <cstdint>

class Encoder {
public:
    explicit Encoder(const std::vector<uint8_t>& alphabet);

    uint8_t encode(uint8_t symbol, int &valid);
    uint8_t decode(uint8_t symbol);
private:
    size_t _size;
    std::vector<uint8_t> _alphabet;
};


#endif //LAB_01_ENCODER_H
