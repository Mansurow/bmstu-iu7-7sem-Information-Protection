#ifndef LAB_01_REFLECTOR_H
#define LAB_01_REFLECTOR_H

#include <cstdint>
#include <vector>
#include <string>

class Reflector {
public:
    explicit Reflector();
    explicit Reflector(const std::vector<uint8_t>& config);
    explicit Reflector(const std::string& config);

    uint8_t reflect(uint8_t symbol);

    void printf_config();
private:
    std::vector<uint8_t> _config;
};

#endif //LAB_01_REFLECTOR_H
