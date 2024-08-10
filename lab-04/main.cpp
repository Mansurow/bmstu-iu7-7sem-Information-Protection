#include <iostream>
#include <fstream>
#include <sstream>

#include "rsa.h"
#include "sha1.h"


#define MENU "1. Generate keys\n2. Create signature\n3. Check signature\n0. Exit"


void print_vector64(std::vector<int64_t>& vector) {
    for (auto val : vector) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

void print_vector8(std::vector<uint8_t>& vector) {
    for (auto val : vector) {
        std::cout << val << " ";
    }
    std::cout << "\n";
}

std::vector<uint8_t> get_file_bytes(std::string filename) {
    std::ifstream input_file(filename, std::ios::binary);

    input_file.seekg(0, std::ios::end);
    std::streamsize fileSize = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> input(fileSize);
    input_file.read(reinterpret_cast<char*>(input.data()), fileSize); // считываем файл в вектор байт
    input_file.close();

    return input;
}

std::vector<int64_t> get_uint16_from_uint32_vec(const std::vector<uint32_t>& v) {


    std::vector<int64_t> result;
    for (uint32_t val : v) {
        uint16_t* val16_ptr = (uint16_t*)&val;
        int64_t val64 = *val16_ptr;

        result.push_back(val64);

        val16_ptr++;
        val64 = *val16_ptr;

        result.push_back(val64);
    }

    return result;
}


int main(int argc, char* argv[])
{
    RSA rsa;

    sha1 sha1;
    PublicKey public_key;
    PrivateKey private_key;
    bool is_gen_keys = false;

    std::cout << MENU << "\n\n";
    int command = -1;
    while (command != 0) {
        std::cout << "Choice:\n";
        std::cin >> command;

        switch (command) {
        case 1: {
            auto [priv, pub] = rsa.genPublicAndSecretKeys(8000, 10000);
            private_key = priv;
            public_key = pub;
            is_gen_keys = true;

            std::cout << "Public key: (" << public_key.e << ", " << public_key.n << ")\n";
            std::cout << "Private: (" << private_key.d << ", " << private_key.n << ")\n";

            break;
        }
        case 2: {
            if (!is_gen_keys) {
                std::cout << "Keys aren't generated\n";
                break;
            }

            std::cout << "Input file:\n";
            std::string input_filename;
            std::cin >> input_filename;

            std::vector<uint8_t> data = get_file_bytes(input_filename);

            ap_int<256> hash = sha1.calculate_hash(data);

            ap_int<256> esp = rsa.Encrypt(private_key.d, private_key.n, hash);

            std::cout << "Signature: " << esp << "\n";

            break;
        }
        case 3: {
            if (!is_gen_keys) {
                std::cout << "Keys are not generated\n";
            }

            std::cout << "Input file:\n";
            std::string input_filename;
            std::cin >> input_filename;

            std::vector<uint8_t> data = get_file_bytes(input_filename);

            auto hash = sha1.calculate_hash(data);

            std::cout << "Input signature:\n";
            ap_int<256> esp_v;
            std::cin >> esp_v;

            ap_int<256> check = rsa.Encrypt(public_key.e, public_key.n, esp_v);

            bool res = hash == check;

            if (res) {
                std::cout << "\nSignature is valid!\n";
            }
            else {
                std::cout << "\nSignature is invalid!\n";
            }
        }
        }
    }
}
