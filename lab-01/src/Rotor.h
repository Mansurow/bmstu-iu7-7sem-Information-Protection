#ifndef LAB_01_ROTOR_H
#define LAB_01_ROTOR_H

#include <memory>
#include <vector>
#include <iostream>

class Rotor {
public:
    explicit Rotor(const std::vector<uint8_t>& wiring, uint8_t pos);
    explicit Rotor(const std::string& wiring);
    explicit Rotor();

    uint8_t encrypt_left(uint8_t symbol);
    uint8_t encrypt_right(uint8_t symbol);

    void set_position(uint8_t position);
    void reset_position();
    void rotate();
    void loop_shift();
    int get_position();
private:
    uint8_t find_index(uint8_t letter);

    std::vector<uint8_t> _wiring;
    std::vector<uint8_t> _start;
    uint8_t _head;
    uint8_t _current;
    uint8_t _tail;

    size_t _size;
};


#endif //LAB_01_ROTOR_H
