#include <random>
#include <tuple>
#include <iostream>

#include "rsa.h"

RSA::RSA()
{

}

int64_t RSA::gcd(int64_t a, int64_t b) {
    while (b != 0) {
        int64_t tmp = a;
        a = b;
        b = tmp % b;
    }

    return a;
}

int64_t RSA::modular_pow(int64_t base, int64_t exponent, int64_t module) {
    if (module == 1) {
        return 0;
    }

    int64_t result = 1;
    base %= module;

    while (exponent != 0) {
        if (exponent % 2) {
            result = result * base % module;
        }

        exponent /= 2;
        base = base * base % module;
    }

    return result;
}

ap_int<256> RSA::modular_pow256(ap_int<256> base, int64_t exponent, int64_t module) {
    if (module == 1) {
        return 0;
    }

    ap_int<256> result(1);
    base %= module;

    while (exponent != 0) {
        if (exponent % 2) {
            result = result * base % module;
        }

        exponent /= 2;
        base = base * base % module;
    }

    return result;
}

std::tuple<int64_t, int64_t, int64_t> RSA::extendet_efclid_alg(int64_t a, int64_t b) {
    bool swap = false;
    if (a < b) {
        int64_t tmp = a;
        a = b;
        b = tmp;
        swap = true;
    }

    std::tuple<int64_t, int64_t, int64_t> first{ a, 1, 0 };
    std::tuple<int64_t, int64_t, int64_t> second{ b, 0, 1 };


    while (std::get<0>(second) != 0) {
        int64_t q = std::get<0>(first) / std::get<0>(second);
        auto tmp = second;

        second = { std::get<0>(first) % std::get<0>(second),
                  std::get<1>(first) - std::get<1>(second) * q,
                  std::get<2>(first) - std::get<2>(second) * q
        };
        first = tmp;
    }

    if (swap) {
        first = { std::get<0>(first), std::get<2>(first), std::get<1>(first) };
    }

    return first;
}

std::pair<int64_t, int64_t> RSA::genPQSimple(int64_t min_value, int64_t max_value) {
    std::vector<int64_t> table;
    for (int64_t i = 0; i <= max_value; ++i) {
        table.push_back(i);
    }

    table[1] = 0;

    for (int64_t num : table) {
        if (num != 0) {
            for (int64_t i = num * num; i <= max_value; i += num) {
                table[i] = 0;
            }
        }
    }

    std::vector<int64_t> simple_nums;
    for (int64_t num : table) {
        if (num != 0 && num >= min_value) {
            simple_nums.push_back(num);
        }
    }

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> distribution(0, simple_nums.size() - 1);

    int64_t p = simple_nums[distribution(generator)];
    int64_t q = simple_nums[distribution(generator)];
    while (p == q) {
        q = simple_nums[distribution(generator)];
    }

    return { p, q };
}

std::pair<PrivateKey, PublicKey> RSA::genPublicAndSecretKeys(int64_t min_value, int64_t max_value) {
    auto [p, q] = genPQSimple(min_value, max_value);

    int64_t N = p * q;

    int64_t euler = (p - 1) * (q - 1);

    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::int64_t> distribution(2, euler - 1);

    int64_t e = distribution(generator);
    while (gcd(e, euler) != 1) {
        e = distribution(generator);
    }

    int64_t d = std::get<1>(extendet_efclid_alg(e, euler));
    if (d < 0) {
        d += euler;
    }

    return {
        PrivateKey{
            d, N
        },
        PublicKey{
            e, N
        }
    };
}

ap_int<256> RSA::Encrypt(int64_t e, int64_t n, ap_int<256> res) {
    auto encrypted = modular_pow256(res, e, n);

    return res;
}
