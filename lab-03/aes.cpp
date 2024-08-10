#include "aes.h"

block aes::CryptBlock(const block& block128, const key& key128, bool decypher) {
    // Расширение ключа - KeyExpansion
    auto keys = GetKeys128(key128, decypher);

    // для удобства конвертируем в матрицу
    auto state = ArrayToMrx4x4(block128);

    // Начальный раунд - сложение с основным ключом;
    AddRoundKey(state, keys[0]);

    // 9 раундов шифрования
    for (int i = 1; i < 10; i++)
    {
        if (decypher)
        {
            InvShiftRows(state);
            InvSubBytes(state);
            AddRoundKey(state, keys[i]);
            InvMixColumns(state);
        }
        else
        {
            SubBytes(state);
            ShiftRows(state);
            MixColumns(state);
            AddRoundKey(state, keys[i]);
        }

    }

    // Финальный раунд
    if (decypher)
    {
        InvShiftRows(state);
        InvSubBytes(state);
    }
    else
    {
        SubBytes(state);
        ShiftRows(state);
    }
    AddRoundKey(state, keys[10]);

    return Mrx4x4ToArray(state);
}

vector<key> aes::GetKeys128(key key128, bool decrypter) {

    vector<key> roundKeys(11, key(16));

    // Ключ для начального раунда
    roundKeys[0] = std::move(key128);

    // Ключи для остальных 10 раундов
    for (int i = 1; i <= 10; ++i) {
        // На основе предыдущего ключа, создаем новый
        key tempWord = roundKeys[i - 1];

        tempWord = rot_word(tempWord);
        tempWord = sub_word(tempWord);
        tempWord[0] ^= R_CON[i - 1];

        for (int j = 0; j < 16; ++j) {
            roundKeys[i][j] = roundKeys[i - 1][j] ^ tempWord[j];
        }
    }

    // if (decrypter)
    //    reverse(roundKeys.begin(), roundKeys.end());

    return roundKeys;
}

key aes::sub_word(key &word) {
    vector<uint8_t> result(word.size());
    for (size_t i = 0; i < word.size(); ++i) {
        result[i] = S_BOX[word[i]];
    }
    return result;
}

key aes::rot_word(key &word) {
    vector<uint8_t> result = word;
    uint8_t temp = result[0];
    for (size_t i = 0; i < word.size() - 1; ++i) {
        result[i] = result[i + 1];
    }
    result[word.size() - 1] = temp;
    return result;
}

void aes::AddRoundKey(mtx& block, const key& roundKey) {
    for (int i = 0; i < block.size(); ++i) {
        for (int j = 0; j < block[0].size(); ++j)
        {
            block[i][j] ^= roundKey[i + j];
        }
    }
}

void aes::InvSubBytes(mtx &block)
{
    for (size_t i = 0; i < block.size(); ++i) {
        for (int j = 0; j < block[0].size(); ++j)
        {
            block[i][j] = S_BOX_INV[block[i][j]];
        }
    }
}

void aes::SubBytes(mtx &block) {
    for (size_t i = 0; i < block.size(); ++i) {
        for (int j = 0; j < block[0].size(); ++j)
        {
            block[i][j] = S_BOX[block[i][j]];
        }
    }
}

mtx aes::ArrayToMrx4x4(const block &block) {
    mtx mtx(4, vector<uint8_t>(4));

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            mtx[j][i] = block[(i * 4) + j];
        }
    }

    return mtx;
}

block aes::Mrx4x4ToArray(const mtx &mtx) {
    block block128(16);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            block128[(i * 4) + j] = mtx[j][i];
        }
    }

    return block128;
}

void aes::ShiftRows(mtx &mtx) {
    for (int i = 1; i < mtx.size(); ++i) {
        std::rotate(mtx[i].begin(), mtx[i].begin() + i, mtx[i].end());
    }
}

void aes::InvShiftRows(mtx &mtx) {
    for (int i = 1; i < mtx.size(); ++i) {
        std::rotate(mtx[i].rbegin(), mtx[i].rbegin() + i, mtx[i].rend());
    }
}

void aes::MixColumns(mtx& mtr) {
    for (size_t i = 0; i < 4; ++i) {
        uint8_t s0 = mtr[0][i];
        uint8_t s1 = mtr[1][i];
        uint8_t s2 = mtr[2][i];
        uint8_t s3 = mtr[3][i];

        mtr[0][i] = GMul(s0, 0x02) ^ GMul(s1, 0x03) ^ s2 ^ s3;
        mtr[1][i] = s0 ^ GMul(s1, 0x02) ^ GMul(s2, 0x03) ^ s3;
        mtr[2][i] = s0 ^ s1 ^ GMul(s2, 0x02) ^ GMul(s3, 0x03);
        mtr[3][i] = GMul(s0, 0x03) ^ s1 ^ s2 ^ GMul(s3, 0x02);
    }
}

void aes::InvMixColumns(mtx &mtr) {
    for (size_t i = 0; i < 4; ++i) {
        uint8_t s0 = mtr[0][i];
        uint8_t s1 = mtr[1][i];
        uint8_t s2 = mtr[2][i];
        uint8_t s3 = mtr[3][i];

        mtr[0][i] = GMul(s0, 0x0E) ^ GMul(s1, 0x0B) ^ GMul(s2, 0x0D) ^ GMul(s3, 0x09);
        mtr[1][i] = GMul(s0, 0x09) ^ GMul(s1, 0x0E) ^ GMul(s2, 0x0B) ^ GMul(s3, 0x0D);
        mtr[2][i] = GMul(s0, 0x0D) ^ GMul(s1, 0x09) ^ GMul(s2, 0x0E) ^ GMul(s3, 0x0B);
        mtr[3][i] = GMul(s0, 0x0B) ^ GMul(s1, 0x0D) ^ GMul(s2, 0x09) ^ GMul(s3, 0x0E);
    }
}

uint8_t aes::GMul(uint8_t x, uint8_t y) {
    uint8_t result = 0;
    uint8_t hbit = 0;
    for (int i = 0; i < 8; ++i) {
        if (y & 1) {
            result ^= x;
        }
        hbit = x & 0x80;
        x <<= 1;
        if (hbit) {
            x ^= 0x1B;
        }
        y >>= 1;
    }
    return result;
}

void aes::AddPadding(vector<uint8_t> &data) {
    size_t padding = 16 - (data.size() % 16);
    AmountPadding = padding;
    for (size_t i = 0; i < padding; ++i) {
        data.push_back(static_cast<uint8_t>(padding));
    }
}

void aes::RemovePadding(vector<uint8_t> &data) {
    for (size_t i = 0; i < AmountPadding; ++i) {
        data.pop_back();
    }
}

block aes::EncryptBlock(const block &block128, const key &key128) {
    // Расширение ключа - KeyExpansion
    auto keys = GetKeys128(key128, false);

    // для удобства конвертируем в матрицу
    auto state = ArrayToMrx4x4(block128);

    // Начальный раунд - сложение с основным ключом;
    AddRoundKey(state, keys[0]);

    // 9 раундов шифрования
    for (int i = 1; i < 10; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, keys[i]);
    }

    // Финальный раунд
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, keys[10]);

    return Mrx4x4ToArray(state);
}

block aes::DecryptBlock(const block &block128, const key &key128) {
// Расширение ключа - KeyExpansion
    auto keys = GetKeys128(key128, false);

    // для удобства конвертируем в матрицу
    auto state = ArrayToMrx4x4(block128);

    // Начальный раунд - сложение с основным ключом;
    AddRoundKey(state, keys[10]);

    // 9 раундов шифрования
    for (int i = 9; i > 0; i--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, keys[i]);
        InvMixColumns(state);
    }

    // Финальный раунд
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, keys[0]);

    return Mrx4x4ToArray(state);
}
