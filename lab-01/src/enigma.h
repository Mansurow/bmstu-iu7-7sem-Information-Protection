#ifndef LAB_01_ENIGMA_H
#define LAB_01_ENIGMA_H


#include <cstdint>
#include <vector>
#include "Reflector.h"
#include "Rotor.h"
#include "encoder.h"

class Enigma {
public:
    Enigma();
    Enigma(uint64_t size_rotor, uint8_t amount_rotors);
    Enigma(const std::vector<uint8_t>& alphabet, uint8_t amount_rotors);

    void set_reflector(Reflector& reflector);
    void set_commutator(Reflector& reflector);
    void set_rotor(Rotor& rotor);

    size_t encrypt(FILE *fin, FILE *fout);
    std::string encrypt(const std::string& message);
    uint8_t encrypt(uint8_t symbol);
    void reset_rotors();

    void printf_config();
private:
    void print_pair(uint8_t s1, uint8_t s2);
    char normalize_sym(uint8_t symbol);
    int _counter;
    int _size_rotor;
    uint8_t _amount_rotors;

    std::unique_ptr<Encoder> _encoder;
    std::unique_ptr<Reflector> _reflector;
    std::unique_ptr<Reflector> _commutator;
    std::vector<std::shared_ptr<Rotor>> _rotors;
};


#endif //LAB_01_ENIGMA_H
