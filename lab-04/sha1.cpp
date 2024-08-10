#include <iostream>
#include "sha1.h"
#include "common.h"

sha1::sha1() {

}

uint32_t left_rotate(uint32_t n, uint32_t b) {
    return ((n << b) | (n >> (32 - b))) & 0xffffffff;
}

ap_int<256> sha1::calculate_hash(std::vector<uint8_t>& data) {
    uint32_t h0 = 0x67452301, a = 0;
    uint32_t h1 = 0xEFCDAB89, b = 0;
    uint32_t h2 = 0x98BADCFE, c = 0;
    uint32_t h3 = 0x10325476, d = 0;
    uint32_t h4 = 0xC3D2E1F0, e = 0;

    uint64_t len = data.size() * 8;

    // добавляем единичный бит и делаем длину в битах сравнимой с 448 по модулю 512
    data.push_back((uint8_t)0x80);

    while (data.size() % 64 != 56) {
        data.push_back(0);
    }

    uint8_t* len_ptr = (uint8_t*)&len;

    // добавляем в конец 64-битное представление длины до расширения
    for (int i = 0; i < 8; ++i) {
        data.push_back(*len_ptr);
        ++len_ptr;
    }

    uint32_t blocks[80] = {0};

    for (int i = 0; i < data.size(); i += 64) {
        auto chunk = get_slice(data, i, i + 64);
        // First, build 16 32-bits blocks from the chunk
        for (uint8_t bid = 0; bid < 16; bid++) {
            blocks[bid] = 0;

            // Having to build a 32-bit word from 4-bit words
            // Add each and shift them to the left
            for (uint8_t cid = 0; cid < 4; cid++) {
                blocks[bid] = (blocks[bid] << 8) +
                    chunk[bid * 4 + cid];
            }

            // Extend the 16 32-bit words into 80 32-bit words
            for (uint8_t i = 16; i < 80; i++) {
                blocks[i] =
                    left_rotate(blocks[i - 3] ^ blocks[i - 8] ^
                        blocks[i - 14] ^ blocks[i - 16],
                        1);
            }
        }

        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;

        // Main "hashing" loop
        for (uint8_t i = 0; i < 80; i++) {
            uint32_t F = 0, g = 0;
            if (i < 20) {
                F = (b & c) | ((~b) & d);
                g = 0x5A827999;
            }
            else if (i < 40) {
                F = b ^ c ^ d;
                g = 0x6ED9EBA1;
            }
            else if (i < 60) {
                F = (b & c) | (b & d) | (c & d);
                g = 0x8F1BBCDC;
            }
            else {
                F = b ^ c ^ d;
                g = 0xCA62C1D6;
            }

            // Update the accumulators
            uint32_t temp = left_rotate(a, 5) + F + e + g + blocks[i];
            e = d;
            d = c;
            c = left_rotate(b, 30);
            b = a;
            a = temp;
        }
        // Update the state with this chunk's hash
        h0 = (h0 + a) & 0xffffffff;
        h1 = (h1 + b) & 0xffffffff;
        h2 = (h2 + c) & 0xffffffff;
        h3 = (h3 + d) & 0xffffffff;
        h4 = (h4 + e) & 0xffffffff;
    }
    ap_int<256> res(0);
    res += ap_int<256>(h4);
    res <<= 32;
    res += ap_int<256>(h3);
    res <<= 32;
    res += ap_int<256>(h2);
    res <<= 32;
    res += ap_int<256>(h1);
    res <<= 32;
    res += ap_int<256>(h0);

    return res;
}
