#ifndef LAB_02_ECB_H
#define LAB_02_ECB_H

#include "DES.h"

using namespace std;

class ECB {
public:
    ECB() {};

    string cypher(string message, string key, bool decrypt=false);
    vector<char> cypher(vector<char> message, vector<char> key, bool decrypt=false);
    vector<char> cypher(vector<char> message, vector<vector<char>> keys, bool decrypt=false);

protected:
    bitset<64> vchar_to_bitset64(vector<char> input);

    vector<char> bitset64_to_vchar(bitset<64> input);

    DES _des;
};


#endif //LAB_02_ECB_H
