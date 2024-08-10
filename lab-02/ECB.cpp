#include <algorithm>
#include "ECB.h"

string ECB::cypher(string input, string key, bool decypher)
{
    std::vector<char> idata(input.begin(), input.end());
    std::vector<char> keydata(key.begin(), key.end());

    auto result = cypher(idata, keydata, decypher);

    std::string str(result.begin(), result.end());

    return str;
}

vector<char> ECB::cypher(vector<char> input, vector<char> key, bool decypher)
{
    vector<char> buffer = {};
    vector<char> result = {};

    int last_cnt = 0;

    if (decypher) {
        last_cnt = input.back();
        input.pop_back();
    }

    auto key_b = vchar_to_bitset64(key);


    for (auto sym : input) {
        if (buffer.size() < 8) {
            buffer.push_back(sym);
        }

        if (buffer.size() == 8) {
            // buffer size is 8 -> can be cyphered
            auto buf_b = vchar_to_bitset64(buffer);

            auto tmp_b_1 = _des.process_block(buf_b, key_b, decypher);
            auto tmp_b_2 = _des.process_block(tmp_b_1, key_b, decypher);
            auto tmp_b_3 = _des.process_block(tmp_b_2, key_b, decypher);

            auto tmp_res = bitset64_to_vchar(tmp_b_3);

            result.insert(result.end(), tmp_res.begin(), tmp_res.end());

            buffer.clear();
        }
    }

    if (buffer.size() > 0 && buffer.size() < 8) {
        while (buffer.size() < 8) {
            buffer.push_back((char)0);
            last_cnt += 1;
        }
        auto buf_b = vchar_to_bitset64(buffer);

        auto tmp_b_1 = _des.process_block(buf_b, key_b, decypher);
        auto tmp_b_2 = _des.process_block(tmp_b_1, key_b, decypher);
        auto tmp_b_3 = _des.process_block(tmp_b_2, key_b, decypher);

        auto tmp_res = bitset64_to_vchar(tmp_b_3);

        result.insert(result.end(), tmp_res.begin(), tmp_res.end());
    }

    if (!decypher)
        result.push_back((char)last_cnt);

    if (decypher) {
        for (int i = 0; i < last_cnt; i++) {
            result.pop_back();
        }
    }

    return result;
}

bitset<64> ECB::vchar_to_bitset64(vector<char> input) {
    bitset<64> val = {0};

    reverse(input.begin(), input.end());

    for (auto sym : input) {
        val <<= 8;
        bitset<8> tmp = sym;

        for (int i = 0; i < 8; i++) {
            val[i] = tmp[7 - i];
        }
    }

    return val;
}

vector<char> ECB::bitset64_to_vchar(bitset<64> input) {
    vector<char> val = { };

    for (int i = 0; i < 64; i += 8) {
        char tmp = 0;
        for (int j = i; j < i + 8; j++) {
            tmp <<= 1;
            tmp |= input[j];
        }

        val.push_back(tmp);
    }

    return val;
}

vector<char> ECB::cypher(vector<char> input, vector<vector<char>> keys, bool decypher) {
    vector<char> buffer = {};
    vector<char> result = {};

    int last_cnt = 0;

    if (decypher) {
        last_cnt = input.back();
        input.pop_back();
    }

    auto key_b1 = vchar_to_bitset64(keys[0]);
    auto key_b2 = vchar_to_bitset64(keys[1]);
    auto key_b3 = vchar_to_bitset64(keys[2]);

    for (auto sym : input) {
        if (buffer.size() < 8) {
            buffer.push_back(sym);
        }

        if (buffer.size() == 8) {
            // buffer size is 8 -> can be cyphered
            auto buf_b = vchar_to_bitset64(buffer);

            bitset<64> tmp_b_3;
            if (decypher)
            {
                auto tmp_b_1 = _des.process_block(buf_b, key_b3, decypher);
                auto tmp_b_2 = _des.process_block(tmp_b_1, key_b2, decypher);
                tmp_b_3 = _des.process_block(tmp_b_2, key_b1, decypher);
            } else
            {
                auto tmp_b_1 = _des.process_block(buf_b, key_b1, decypher);
                auto tmp_b_2 = _des.process_block(tmp_b_1, key_b2, decypher);
                tmp_b_3 = _des.process_block(tmp_b_2, key_b3, decypher);
            }


            auto tmp_res = bitset64_to_vchar(tmp_b_3);

            result.insert(result.end(), tmp_res.begin(), tmp_res.end());

            buffer.clear();
        }
    }

    if (buffer.size() > 0 && buffer.size() < 8) {
        while (buffer.size() < 8) {
            buffer.push_back((char)0);
            last_cnt += 1;
        }
        auto buf_b = vchar_to_bitset64(buffer);

        bitset<64> tmp_b_3;
        if (decypher)
        {
            auto tmp_b_1 = _des.process_block(buf_b, key_b3, decypher);
            auto tmp_b_2 = _des.process_block(tmp_b_1, key_b2, decypher);
            tmp_b_3 = _des.process_block(tmp_b_2, key_b1, decypher);
        } else
        {
            auto tmp_b_1 = _des.process_block(buf_b, key_b1, decypher);
            auto tmp_b_2 = _des.process_block(tmp_b_1, key_b2, decypher);
            tmp_b_3 = _des.process_block(tmp_b_2, key_b3, decypher);
        }

        auto tmp_res = bitset64_to_vchar(tmp_b_3);

        result.insert(result.end(), tmp_res.begin(), tmp_res.end());
    }

    if (!decypher)
        result.push_back((char)last_cnt);

    if (decypher) {
        for (int i = 0; i < last_cnt; i++) {
            result.pop_back();
        }
    }

    return result;
}
