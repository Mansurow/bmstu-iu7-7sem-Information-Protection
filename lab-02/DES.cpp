#include <algorithm>
#include "DES.h"

bitset<64> DES::process_block(bitset<64> value, bitset<64> key, bool decypher)
{
    // generate keys
    auto keys = generate_keys(key, decypher);

    // initial permutation
    auto round_val = IP_f(value);

    // 16 rounds
    for (auto rkey : keys) {
        round_val = wround(round_val, rkey);
    }

    // final permutation
    auto final_val = FP_f(round_val);

    return final_val;
}

bitset<56> DES::PC1_f(bitset<64> key)
{
    bitset<56> new_key = { 0 };

    int i = 0;
    for (auto index : PC1) {
        new_key[i] = key[index - 1];
        i++;
    }

    return new_key;
}

bitset<48> DES::PC2_f(bitset<56> key)
{
    bitset<48> new_key = { 0 };

    int i = 0;
    for (auto index : PC2) {
        new_key[i] = key[index - 1];
        i++;
    }

    return new_key;
}

vector<bitset<48>> DES::generate_keys(bitset<64> key, bool decypher)
{
    auto initial_value = PC1_f(key);

    vector<bitset<48>> keys = {};

    for (auto shift : key_shift_cnt) {
        auto tmp = initial_value;

        bitset<28> left = {};
        bitset<28> right = {};

        for (int i = 0; i < 56; i++) {
            if (i < 28) {
                left[i] = tmp[i];
            } else {
                right[i - 28] = tmp[i];
            }
        }

        auto initial_left = left;
        auto initial_right = right;

        // cycle shift
        left <<= shift;
        int j = 0;
        for (int i = 28 - shift; i < 28; i++) {
            left[i] = initial_left[j];
            j++;
        }

        right <<= shift;
        j = 0;
        for (int i = 28 - shift; i < 28; i++) {
            right[i] = initial_right[j];
            j++;
        }

        for (int i = 0; i < 28; i++) {
            initial_value[i] = left[i];
            initial_value[i + 27] = right[i];
        }

        keys.push_back(PC2_f(initial_value));
    }

    if (decypher)
        reverse(keys.begin(), keys.end());

    return keys;
}

pair<bitset<32>, bitset<32>> DES::IP_f(bitset<64> value)
{
    bitset<64>  ip_res = {};

    int i = 0;
    for (auto index : IP) {
        ip_res[i] = value[index - 1];
        i++;
    }

    bitset<32> left = {};
    bitset<32> right = {};

    for (int i = 0; i < 32; i++) {
        left[i] = ip_res[i];
        right[i] = ip_res[i + 32];
    }

    return pair<bitset<32>, bitset<32>>(left, right);
}

pair<bitset<32>, bitset<32>> DES::wround(pair<bitset<32>, bitset<32>> RL, bitset<48> key)
{
    return pair<bitset<32>, bitset<32>>(RL.second, RL.first ^ inner_f(RL.second, key));
}

bitset<48> DES::E_f(bitset<32> value)
{
    bitset<48> new_value = { 0 };

    int i = 0;
    for (auto index : E) {
        new_value[i] = value[index - 1];
        i++;
    }

    return new_value;
}

bitset<4> DES::S_f(bitset<6> value, int i)
{
    bitset<2> row_b = {};
    row_b[0] = value[0];
    row_b[1] = value[5];

    bitset<4> col_b = {};
    col_b[0] = value[1];
    col_b[1] = value[2];
    col_b[2] = value[3];
    col_b[3] = value[4];

    int row = row_b.to_ulong();
    int col = col_b.to_ulong();

    bitset<4> result = {};

    result = S[i][row][col];
    return result;
}

bitset<32> DES::P_f(bitset<32> value)
{
    bitset<32> new_value = { 0 };

    int i = 0;
    for (auto index : P) {
        new_value[i] = value[index - 1];
        i++;
    }

    return new_value;
}

bitset<32> DES::inner_f(bitset<32> value, bitset<48> key)
{
    auto expanded_val = E_f(value);
    auto xor_res = key ^ expanded_val;

    bitset<6> tmp = {};
    bitset<32> result = {};

    int k = 0;
    for (int i = 0; i < 8; i += 1) {
        // block of 6 bits
        for (int j = 0; j < 6; j++) {
            tmp[j] = xor_res[i * 6 + j];
        }

        auto s_res = S_f(tmp, i);

        for (int p = 0; p < 4; p++) {
            result[p + k] = s_res[p];
        }
        k += 4;
    }

    auto final_res = P_f(result);

    return final_res;
}

bitset<64> DES::FP_f(pair<bitset<32>, bitset<32>> RL)
{
    bitset<64> tmp_res = {};

    for (int i = 0; i < 32; i++) {
        tmp_res[i] = RL.second[i];
        tmp_res[i + 32] = RL.first[i];
    }

    int i = 0;
    bitset<64> result = {};
    for (auto index : FP) {
        result[i] = tmp_res[index - 1];
        i++;
    }

    return result;
}
