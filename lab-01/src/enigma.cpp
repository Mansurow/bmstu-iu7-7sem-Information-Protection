#include <iostream>
#include "enigma.h"

Enigma::Enigma() {
    _counter = 0;
    _size_rotor = 0;
    _amount_rotors = 0;
}

Enigma::Enigma(uint64_t size_rotor, uint8_t amount_rotors) {
    this->_counter = 0;
    this->_size_rotor = (int) size_rotor;
    this->_amount_rotors = amount_rotors;
}

void Enigma::set_reflector(Reflector &reflector) {
    _reflector = std::make_unique<Reflector>(reflector);
}

Enigma::Enigma(const std::vector<uint8_t>& alphabet,
               uint8_t amount_rotors) {
    this->_counter = 0;
    this->_size_rotor = (int) alphabet.size();
    this->_amount_rotors = amount_rotors;

    Encoder encoder(alphabet);
    this->_encoder = std::make_unique<Encoder>(encoder);
}

void Enigma::set_commutator(Reflector &reflector) {
    _commutator = std::make_unique<Reflector>(reflector);
}

void Enigma::set_rotor(Rotor &rotor) {
    _rotors.push_back(std::make_shared<Rotor>(rotor));
}

uint8_t Enigma::encrypt(uint8_t symbol)
{
    int rotor_queue = 0;
    uint8_t new_symbol = symbol;

    new_symbol = _commutator->reflect(new_symbol);

    for (int i = 0; i < _amount_rotors; i++)
    {
        new_symbol = _rotors[i]->encrypt_left(new_symbol);
    }

    new_symbol = _reflector->reflect(new_symbol);

    // std::cout << (char) new_symbol;

    for (int i = _amount_rotors - 1; i >= 0; i--)
    {
        new_symbol = _rotors[i]->encrypt_right(new_symbol);
    }

    // std::cout << "\n";

    new_symbol = _commutator->reflect(new_symbol);

    rotor_queue = 1;
    this->_counter += 1;
    for (int i = 0; i < _amount_rotors; ++i) {
        if (_counter % rotor_queue == 0) {
            _rotors[i]->rotate();
        }
        rotor_queue *= _size_rotor;
    }

    return new_symbol;
}

void Enigma::reset_rotors() {
    for (int i = 0; i < _amount_rotors; i++)
    {
        this->_rotors[i]->reset_position();
    }
}

std::string Enigma::encrypt(const std::string& message) {
    std::string new_message;
    for (uint8_t symbol: message)
    {
        int flag;
        uint8_t enc_ch = this->_encoder->encode(symbol, flag);
        uint8_t newcode = this->encrypt(enc_ch);
        uint8_t dec_ch = this->_encoder->decode(newcode);

        new_message += static_cast<char>(encrypt(dec_ch));;
    }

    return new_message;
}

size_t Enigma::encrypt(FILE *fin, FILE *fout) {
    if (fin == nullptr || fout == nullptr)
    {
        return -1;
    }

    std::wstring message;

    char code;
    fseek(fin , 0, SEEK_END);
    size_t input_size = ftell(fin);
    fseek(fin , 0, SEEK_SET);
    size_t size = 0;

    while (size < input_size) {
        size += fread(&code, sizeof(uint8_t), 1, fin);
        fseek(fin , SEEK_SET, SEEK_CUR);

        int flag;
        uint8_t enc_ch = this->_encoder->encode(code, flag);
        if (flag) {
            uint8_t newcode = this->encrypt(enc_ch);
            uint8_t dec_ch = this->_encoder->decode(newcode);

            fwrite(&dec_ch, sizeof(uint8_t), 1, fout);
        } else
        {
            fwrite(&enc_ch, sizeof(uint8_t), 1, fout);
        }
    }


//    fseek(fout , 0, SEEK_SET);
//    fwrite(message.c_str(), sizeof(wchar_t), message.size(), fout);

    return size;
}

void Enigma::printf_config() {
    printf("\n");
    printf("Текущее состояние:\n");
    printf("Рефлектор   Ротор 1    Ротор 2    Ротор 3   Коммутатор\n");
    printf("Позиция:      %d           %d          %d\n", _rotors[0]->get_position(), _rotors[1]->get_position(), _rotors[2]->get_position());
//    for (int i = 0; i < _size_rotor; i++)
//    {
//        print_pair(i, _reflector->reflect(i));
//        printf("  ");
//        print_pair(i, _rotors[0]->encrypt_right(i));
//        printf("  ");
//        print_pair(i, _rotors[1]->encrypt_right(i));
//        printf("  ");
//        print_pair(i, _rotors[2]->encrypt_right(i));
//        printf("  ");
//        print_pair(i, _commutator->reflect(i));
//        printf("\n");
//    }
//    std::wcout << "\n";
}

char Enigma::normalize_sym(uint8_t symbol) {
    if (symbol > 0 && symbol < 21)
    {
        return '\u0007';
    }
    else
        return static_cast<char>(symbol);
}

void Enigma::print_pair(uint8_t s1, uint8_t s2) {
    std::cout << "{ " << normalize_sym(s1) << " = " << static_cast<char>(normalize_sym(s2)) << " }";
}





