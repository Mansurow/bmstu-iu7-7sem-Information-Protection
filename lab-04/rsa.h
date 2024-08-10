#ifndef RSA_H
#define RSA_H

#include <vector>
#include <cstdint>
#include <tuple>
#include "ap.hpp"

struct PublicKey {
    int64_t e;
    int64_t n;
};

struct PrivateKey {
    int64_t d;
    int64_t n;
};

class RSA
{
public:
    RSA();

    static int64_t gcd(int64_t a, int64_t b);

    static std::tuple<int64_t, int64_t, int64_t> extendet_efclid_alg(int64_t a, int64_t b);

    static std::pair<int64_t, int64_t> genPQSimple(int64_t min_value, int64_t max_value);

    static std::pair<PrivateKey, PublicKey> genPublicAndSecretKeys(int64_t min_value, int64_t max_value);

    static int64_t modular_pow(int64_t base, int64_t exponent, int64_t module);
    static ap_int<256> modular_pow256(ap_int<256> base, int64_t exponent, int64_t module);

    static ap_int<256> Encrypt(int64_t e, int64_t n, ap_int<256> res);
};

#endif // RSA_H
