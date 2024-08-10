#include <iostream>
#include <fstream>
#include "ECB.h"

vector<char> read_bytes_from_file(string fname)
{
    ifstream in(fname, ios::in | ios::binary);

    vector<char> buffer;

    if (in.is_open()) {
        char tmp;
        in.seekg(0, ios::beg);
        while (in.get(tmp))
            buffer.push_back(tmp);
    }
    in.close();

    return buffer;
}

void write_bytes_into_file(string fname, vector<char> data) {
    ofstream out;
    out.open(fname, ios_base::binary);
    if (out.is_open())
    {
        for (auto sym : data)
            out.put(sym);
    }
    out.close();
}

int main() {
    ECB ecb;

    vector<char> test = read_bytes_from_file("../data/report.pdf");

    vector<char> key1 = read_bytes_from_file("../data/keys/key1.txt");
    vector<char> key2 = read_bytes_from_file("../data/keys/key2.txt");
    vector<char> key3 = read_bytes_from_file("../data/keys/key3.txt");

    vector<vector<char>> keys = {
            key1, key2, key3
    };

    auto encrypt_text = ecb.cypher(test, keys, false);

    write_bytes_into_file("../data/dec.txt", encrypt_text);

    //auto decrypt_text = ecb.cypher(encrypt_text, keys, true);
    //write_bytes_into_file("../data/res.", decrypt_text);

    return 0;
}
