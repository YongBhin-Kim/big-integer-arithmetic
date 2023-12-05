#include "libsync_api.h"

char bigint::mul_algo[50] = "";

bigint::bigint(const char* str_hex) {
    if (str_hex[0] - '0') {
        bi_set_by_string(&a, NEGATIVE, &str_hex[3], 16);
    }
    else {
        bi_set_by_string(&a, NON_NEGATIVE, &str_hex[2], 16);
    }

    // bi_set_by_string(&this->a, (str_hex[0] - '0') ? NEGATIVE : NON_NEGATIVE, (str_hex[0] - '0') ? &str_hex[3] : &str_hex[2], 16);
}

bigint::bigint(const bigint &src) {
    bi_assign(&a, src.a);
}

bigint::~bigint() {
    if (a) {
        bi_delete(&a);
    }
}

void bigint::square() {
    bigint ret = *this;
    bi_squaring(&a, ret.a);
}

void bigint::power(bigint n, bigint M) {
    bigint ret = *this;
    bi_montgomery_mod_exp(&a, ret.a, n.a, M.a);
}

std::ostream &operator<<(std::ostream &out, const bigint &bigint_) {
    bi_show_hex(bigint_.a);
    return out;
}

bigint bigint::operator+(const bigint &other) const {
    bigint ret;
    
    bi_add(&ret.a, a, other.a);
    return ret;
}

bigint bigint::operator-(const bigint &other) const {
    bigint ret;

    bi_sub(&ret.a, a, other.a);
    return ret;
}

bigint bigint::operator*(const bigint &other) const {
    bigint ret;

    bi_mul(&ret.a, a, other.a, mul_algo);
    return ret;
}

bigint bigint::operator/(const bigint &other) const {
    c_bigint *remain = nullptr;
    bigint    ret;

    bi_div(&ret.a, &remain, a, other.a);
    bi_delete(&remain);
    return ret;
}

bigint bigint::operator%(const bigint &other) const {
    c_bigint *quotient = nullptr;
    bigint    ret;

    bi_div(&quotient, &ret.a, a, other.a);
    bi_delete(&quotient);
    return ret;
}

bigint &bigint::operator<<(int amount) {
    bi_shift_left(&a, amount);
    return *this;
}

bigint &bigint::operator>>(const int amount) {
    bi_shift_right(&a, amount);
    return *this;
}

bigint &bigint::operator+=(const bigint &other) {
    c_bigint *ret = nullptr;
    
    bi_add(&ret, a, other.a);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator+=(const char *str_hex) {
    c_bigint *ret = nullptr;
    bigint other(str_hex);
    
    bi_add(&ret, a, other.a);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator-=(const bigint &other) {
    c_bigint *ret = nullptr;
    
    bi_sub(&ret, a, other.a);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator-=(const char *str_hex) {
    c_bigint *ret = nullptr;
    bigint other(str_hex);
    
    bi_sub(&ret, a, other.a);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator*=(const bigint &other) {
    c_bigint *ret = nullptr;
    
    bi_mul(&ret, a, other.a, mul_algo);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator*=(const char *str_hex) {
    c_bigint *ret = nullptr;
    bigint other(str_hex);
    
    bi_mul(&ret, a, other.a, mul_algo);
    bi_assign(&a, ret);
    bi_delete(&ret);
    return *this;
}

bigint &bigint::operator/=(const bigint &other) {
    c_bigint *quotient = nullptr;
    c_bigint *remains  = nullptr;
    
    bi_div(&quotient, &remains, a, other.a);
    bi_assign(&a, quotient);
    bi_delete(&quotient);
    bi_delete(&remains);
    return *this;
}

bigint &bigint::operator/=(const char *str_hex) {
    c_bigint *quotient = nullptr;
    c_bigint *remains  = nullptr;
    bigint other(str_hex);
    
    bi_div(&quotient, &remains, a, other.a);
    bi_assign(&a, quotient);
    bi_delete(&quotient);
    bi_delete(&remains);
    return *this;
}

bigint &bigint::operator%=(const bigint &other) {
    c_bigint *quotient = nullptr;
    c_bigint *remains  = nullptr;
    
    bi_div(&quotient, &remains, a, other.a);
    bi_assign(&a, remains);
    bi_delete(&quotient);
    bi_delete(&remains);
    return *this;
}

bigint &bigint::operator%=(const char *str_hex) {
    c_bigint *quotient = nullptr;
    c_bigint *remains  = nullptr;
    bigint other(str_hex);
    
    bi_div(&quotient, &remains, a, other.a);
    bi_assign(&a, remains);
    bi_delete(&quotient);
    bi_delete(&remains);
    return *this;
}

bool bigint::operator>(const bigint &other) const {
    if (bi_compare(a, other.a) == 1) {
        return true;
    }
    return false;
}

bool bigint::operator<(const bigint &other) const {
    if (bi_compare(a, other.a) == -1) {
        return true;
    }
    return false;
}

bool bigint::operator==(const bigint &other) const {
    if (bi_compare(a, other.a) == 0) {
        return true;
    }
    return false;
}

bool bigint::operator!=(const bigint &other) const {
    if (bi_compare(a, other.a) != 0) {
        return true;
    }
    return false;
}

bool bigint::operator>=(const bigint &other) const {
    if (bi_compare(a, other.a) == -1) {
        return false;
    }
    return true;
}

bool bigint::operator<=(const bigint &other) const {
    if (bi_compare(a, other.a) == 1) {
        return false;
    }
    return true;
}

// int main(void) {
//     bigint::set_mul_algo("Karatsuba");

//     bigint a = "0x12";
//     bigint b = "0x10";
//     bigint c = "0xff";

//     std::cout << "1. " << a + b;
//     std::cout << "2. " << a - b;
//     std::cout << "3. " << a * b;
//     std::cout << "4. " << a / b;
//     std::cout << "5. " << a % b;
//     std::cout << "6. " << (b << 3);
//     std::cout << "7. " << (b >> 1);
    
//     a += "0x35";    std::cout << "8. " << a;
//     a -= "0x35";    std::cout << "9. " << a;
//     a *= "0x35";    std::cout << "10. " << a;
//     a /= "0x35";    std::cout << "11. " << a;
//     a %= "0x12";    std::cout << "12. " << a;

//     c.square();     std::cout << "13. " << c;
//     b.power("0xa881", "0xff");
//     std::cout << "14. " << b;

//     if (c > a) {
//         std::cout << "15. " << "c > a\n";
//     }

//     if (c >= a) {
//         std::cout << "16. " << "c >= a\n";
//     }

//     if (c != a) {
//         std::cout << "17. " << "c != a\n";
//     }

//     return 0;
// }