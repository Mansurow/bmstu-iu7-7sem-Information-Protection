#include <stdexcept>
#include <iostream>
#include "Reflector.h"

Reflector::Reflector()
{
    for (uint8_t i = 127; i >= 0; i--)
    {
        _config.push_back(i);
    }
}

Reflector::Reflector(const std::vector<uint8_t>& config): _config(config) { }

uint8_t Reflector::reflect(uint8_t symbol) {
    return _config[symbol];
}

Reflector::Reflector(const std::string &config) {
    for (uint8_t c: config)
    {
        this->_config.push_back(c);
    }
}

void Reflector::printf_config() {
    printf("Размер рефлектора: %d\n", (int) _config.size());
    for (int i = 0; i < _config.size(); i++)
    {
        if (i > 0 && i < 21)
        {
            std::cout << "{ " << '\u0007' << " = ";
        }
        else
            std::cout << "{ " << static_cast<char>(i) << " = ";

        if (_config[i] > 0 && _config[i] < 21)
        {
            std::cout << '\u0007' << " }\n";
        }
        else
            std::cout << static_cast<char>(_config[i]) << " }\n";
    }
    std::cout << "\n";
}


