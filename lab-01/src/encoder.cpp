#include "encoder.h"

Encoder::Encoder(const std::vector<uint8_t>& alphabet): _alphabet(alphabet) {
    this->_size = alphabet.size();
}

uint8_t Encoder::encode(uint8_t symbol, int &valid) {
    for (int i = 0; i < _size; ++i) {
        if (_alphabet[i] == symbol) {
            valid = 1;
            return i;
        }
    }

    valid = 0;
    return symbol;
}

uint8_t Encoder::decode(uint8_t symbol) {
    if (symbol >= _size) {
        return symbol;
    }

    return _alphabet[symbol];
}

