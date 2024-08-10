//
// Created by vladislav on 25.11.2023.
//

#ifndef LAB_03_CFB_H
#define LAB_03_CFB_H

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include "aes.h"

using namespace std;

const int block_size = 16;
// Режим обратной связи по шифротексту, режим гаммирования с обратной связью
class cfb {
public:
     explicit cfb (const vector<uint8_t>& vi);

     string crypt(string message, bool decrypt = false);

     vector<block> divideBlocks(const vector<uint8_t> &message);
     vector<uint8_t> mergeBlocks(const vector<block> &blocks);

     vector<uint8_t> encrypt(vector<uint8_t>& block128, const vector<uint8_t>& key);
     vector<uint8_t> decrypt(vector<uint8_t>& block128, const vector<uint8_t>& key);

     void setVI(const vector<uint8_t>& vi);

     void clear_tmp();

     void print_bloks(const vector<block> &blocks);
     void print_message(const vector<uint8_t> message);
private:

    // Вектор инициализации
    uint8_t VI[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    block _prevVI;
    bool first_round = false;
    unsigned int AmountPadding = 0;
    aes AES;
};


#endif //LAB_03_CFB_H
