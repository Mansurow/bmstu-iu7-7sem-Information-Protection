#include <iostream>
#include <vector>
#include <fstream>
#include "cfb.h"
#include "aes.h"

void test_cfb_one_block()
{
    key key128 = { 65, 65, 65, 65, 65, 64, 67, 65, 65, 70, 65, 65, 65, 65, 65, 65 };
    vector<uint8_t> message2 = { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80 };
    vector<uint8_t> message1 = { 70, 100, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80 };
    vector<uint8_t> message = { 65, 66, 67, 68, 69, 70, 100, 72, 73, 74, 150, 76, 77, 78, 79, 80 };
    vector<uint8_t> iv = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

    cfb cfb(iv);

    cout << "Message 1: ";
    for (int i = 0; i < message.size(); i++)
    {
        cout << message[i];
    }
    cout << endl;

    cout << "Message 2: ";
    for (int i = 0; i < message1.size(); i++)
    {
        cout << message1[i];
    }
    cout << endl;

    cout << "Message 3: ";
    for (int i = 0; i < message2.size(); i++)
    {
        cout << message2[i];
    }
    cout << endl;

    auto out = cfb.encrypt(message, key128);

    cout << "Crypt M1: ";
    for (int i = 0; i < 16; i++)
    {
        cout << out[i];
    }
    cout << endl;

    auto out1 = cfb.encrypt(message1, key128);

    cout << "Crypt M2: ";
    for (int i = 0; i < 16; i++)
    {
        cout << out1[i];
    }
    cout << endl;

    auto out2 = cfb.encrypt(message2, key128);

    cout << "Crypt M3: ";
    for (int i = 0; i < 16; i++)
    {
        cout << out1[i];
    }
    cout << endl;

    cfb.setVI(iv);

    auto dec = cfb.decrypt(out, key128);

    cout << "Decrypt: ";
    for (int i = 0; i < dec.size(); i++)
    {
        cout << dec[i];
    }
    cout << endl;

    auto dec1 = cfb.decrypt(out1, key128);

    cout << "Decrypt: ";
    for (int i = 0; i < dec1.size(); i++)
    {
        cout << dec1[i];
    }
    cout << endl;

    auto dec2 = cfb.decrypt(out2, key128);

    cout << "Decrypt: ";
    for (int i = 0; i < dec2.size(); i++)
    {
        cout << dec2[i];
    }
    cout << endl;
}

void test_exp_key()
{
    aes aes128;

    key key128 = { 65, 65, 65, 65, 65, 64,67, 65, 65, 70, 65, 65, 65, 65, 65, 65 };

    auto keys = aes128.GetKeys128(key128);

    for (auto key : keys)
    {
        cout << "Key: ";
        for (auto byte : key)
        {
            cout << byte;
        }
        cout << endl;
    }
}

void test_aes()
{
    aes aes;

    vector<uint8_t> message = { 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80  };
    vector<uint8_t> key = { 100, 65, 65, 200, 65, 64,67, 0, 65, 70, 65, 65, 65, 65, 65, 65 };

    cout << "Message: ";
    for (int i = 0; i < message.size(); i++)
    {
        cout << message[i];
    }
    cout << endl;

    // aes.AddPadding(message);

    auto out = aes.EncryptBlock(message, key);
    auto out1 = aes.EncryptBlock(message, key);

    cout << "Encrypt 1: ";
    for (int i = 0; i < 16; i++)
    {
        cout << out[i];
    }
    cout << endl;

    cout << "Encrypt 2: ";
    for (int i = 0; i < 16; i++)
    {
        cout << out1[i];
    }
    cout << endl;

    auto dec1 = aes.DecryptBlock(out, key);

    // aes.RemovePadding(out1);

    cout << "Decrypt: ";
    for (int i = 0; i < dec1.size(); i++)
    {
        cout << dec1[i];
    }
    cout << endl;

    auto dec2 = aes.DecryptBlock(out, key);

    cout << "Decrypt: ";
    for (int i = 0; i < dec1.size(); i++)
    {
        cout << dec2[i];
    }
    cout << endl;
}

vector<uint8_t> read_bytes_from_file(string fname)
{
    ifstream in(fname, ios::in | ios::binary);

    vector<uint8_t> buffer;

    if (in.is_open()) {
        char tmp;
        in.seekg(0, ios::beg);
        while (in.get(tmp))
            buffer.push_back((uint8_t) tmp);
    }
    in.close();

    return buffer;
}

void write_bytes_into_file(string fname, vector<uint8_t> data) {
    ofstream out;
    out.open(fname, ios_base::binary);
    if (out.is_open())
    {
        for (auto sym : data)
            out.put((char) sym);
    }
    out.close();
}

int main() {

    // test_cfb_one_block();
    // test_cfb_one_block();
    // test_exp_key();
    // test_aes();

    vector<uint8_t> test = read_bytes_from_file("../data/cfb.png");
    vector<uint8_t> key = read_bytes_from_file("../config/key.txt");
    vector<uint8_t> vi = read_bytes_from_file("../config/vi.txt");

    cfb cfb(vi);
    vector<block> enc_blocks;

    auto blocks = cfb.divideBlocks(test);

    // cfb.print_message(test);
    // cfb.print_bloks(blocks);


    for (auto block: blocks)
    {
        auto dec_block = cfb.encrypt(block, key);
        enc_blocks.push_back(dec_block);
    }

    auto mes = cfb.mergeBlocks(enc_blocks);

    // cfb.print_message(mes);

    write_bytes_into_file("../data/enc.bin", mes);

    cfb.setVI(vi);
    vector<block> dec_blocks;

    // enc_blocks[0][0] = 1;

    for (auto block: enc_blocks)
    {
        auto dec_block = cfb.decrypt(block, key);
        dec_blocks.push_back(dec_block);
    }

    auto dec_mes = cfb.mergeBlocks(dec_blocks);

    // cfb.print_message(mes);

    write_bytes_into_file("../data/dec.png", dec_mes);

    return 0;
}
