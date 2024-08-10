#include "Rotor.h"

Rotor::Rotor() {
    this->_current = 0;
    this->_head = 0;

    for (int i = 0; i < 256; i++)
    {
        _wiring.push_back(i);
    }

    this->_size = _wiring.size();
    this->_tail = _size - 1;
}

Rotor::Rotor(const std::vector<uint8_t>& wiring, uint8_t pos): _wiring(wiring), _start(_wiring) {
    this->_current = pos;
    this->_head = 0;

    this->_size = this->_wiring.size();
}

Rotor::Rotor(const std::string& wiring) {
    for (int c: wiring)
    {
        this->_wiring.push_back(c);
    }

    this->_current = 0;
    this->_head = 0;
    this->_size = this->_wiring.size();
    this->_tail = this->_size - 1;
}

uint8_t Rotor::encrypt_right(uint8_t symbol) {
    uint8_t symbolIndex = symbol;
    if (symbol > _size - 1)
    {
        symbolIndex = symbolIndex - _size - 1 + _current;
    }
    else
    {
        symbolIndex += _current;
    }

    return _wiring[symbolIndex];
}

uint8_t Rotor::encrypt_left(uint8_t symbol) {
    if (find_index(symbol) < _current)
    {
        return find_index(symbol) - _current + _size;
    }

    return find_index(symbol) - _current;
}

uint8_t Rotor::find_index(uint8_t letter) {
    for (int i = 0; i < _size; i++)
    {
        if (_wiring[i] == letter)
        {
            return i;
        }
    }

    return -1;
}

void Rotor::set_position(uint8_t position)
{
    this->_current = position;
}

void Rotor::reset_position() {
    _wiring = _start;
}

void Rotor::rotate()
{
    uint8_t temp = _wiring[_size - 1];
    for (int i = _size - 1; i > 0; --i) {
        _wiring[i] = _wiring[i - 1];
    }
    _wiring[0] = temp;
}

void Rotor::loop_shift() { }

int Rotor::get_position()
{
    return static_cast<int>(_wiring[0]);
}
