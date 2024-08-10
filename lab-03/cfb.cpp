//
// Created by vladislav on 25.11.2023.
//


#include "cfb.h"

string cfb::crypt(string message, bool decrypt) {

}

vector<uint8_t> cfb::encrypt(vector<uint8_t>& block, const vector<uint8_t>& key) {

    if (_prevVI.size() != block.size())
        throw invalid_argument("Инит. вектор должен быть одинаковой длины с блоком");
    if (_prevVI.size() != 16)
        throw invalid_argument("_prevVI.size() != 128");

    // Делим сообщение на блоки

    vector<uint8_t> result(16);
    vector<uint8_t> res_aes;

    //AES.AddPadding(block);

    res_aes = AES.EncryptBlock(_prevVI, key);

    for (int i = 0; i < 16; i++)
    {
        result[i] = res_aes[i] ^ block[i];
    }

    _prevVI = result;

    return result;
}

vector<uint8_t> cfb::decrypt(vector<uint8_t>& block, const vector<uint8_t>& key) {

    // Делим сообщение на блоки

    vector<uint8_t> result(16);
    vector<uint8_t> res_aes;

    res_aes = AES.EncryptBlock(_prevVI, key);

    for (int i = 0; i < 16; i++)
    {
        result[i] = res_aes[i] ^ block[i];
    }

    _prevVI = block;

    return result;
}

cfb::cfb(const vector<uint8_t>& vi) {
    _prevVI = vi;
}

void cfb::clear_tmp() {
    first_round = false;
}

void cfb::setVI(const vector<uint8_t> &vi) {
    _prevVI = vi;
}

vector<block> cfb::divideBlocks(const vector<uint8_t> &message) {
    size_t numArrays = message.size() / 16;
    size_t remainder = message.size() % 16;

    vector<block> arrBlocks;
    for (size_t i = 0; i < numArrays; ++i) {
        block subArray(message.begin() + i * 16, message.begin() + (i + 1) * 16);
        arrBlocks.push_back(subArray);
    }

    if (remainder > 0)
    {
        block subArray(message.end() - remainder, message.end());

        size_t padding = 16 - (message.size() % 16);
        AmountPadding = padding;
        for (size_t i = 0; i < padding; ++i) {
            subArray.push_back(static_cast<uint8_t>(padding));
        }

        arrBlocks.push_back(subArray);
    }

    return arrBlocks;
}

vector<uint8_t> cfb::mergeBlocks(const vector<block> &blocks) {

    vector<uint8_t> message;

    for (int i = 0; i < blocks.size() - 1; i++) {
        message.insert(message.end(), blocks[i].begin(), blocks[i].end());
    }

    auto last = blocks.size() - 1;
    message.insert(message.end(), blocks[last].begin(), blocks[last].end() - AmountPadding);
    // AmountPadding = 0;

    return message;
}

void cfb::print_message(const vector<uint8_t> message) {
    cout << "Message: " << endl;
    for (auto s: message)
    {
        cout << s;
    }
    cout << endl;
}

void cfb::print_bloks(const vector<block> &blocks) {
    cout << "Blocks: " << endl;
    for (auto block: blocks)
    {
        for (auto s: block)
        {
            cout << s;
        }
        cout << endl;
    }
}
