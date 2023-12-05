#ifndef __LIBSYNC_API
#define __LIBSYNC_API

#include <iostream>
#include <string>

#include "config.h"
#include "array.h"
#include "bigint.h"

class bigint {
    private:
        STRUCT_BIGINT *a = nullptr;

    public:
        static char mul_algo[50];
        static void set_mul_algo(const char *str) {
            memcpy(mul_algo, str, std::strlen(str));
        }

         bigint() {}
         bigint(const char *str_hex);
         bigint(const bigint &src);
        ~bigint();

        void square();
        void power(bigint n, bigint M);

        friend std::ostream &operator<<(std::ostream &out, const bigint &bigint_);

        bigint operator+(const bigint &other) const;
        bigint operator-(const bigint &other) const;
        bigint operator*(const bigint &other) const;
        bigint operator/(const bigint &other) const;
        bigint operator%(const bigint &other) const;
        
        bigint &operator<<(const int amount);
        bigint &operator>>(const int amount);
        bigint &operator+=(const bigint &other);
        bigint &operator+=(const char *str_hex);
        bigint &operator-=(const bigint &other);
        bigint &operator-=(const char *str_hex);
        bigint &operator*=(const bigint &other);
        bigint &operator*=(const char *str_hex);
        bigint &operator/=(const bigint &other);
        bigint &operator/=(const char *str_hex);
        bigint &operator%=(const bigint &other);
        bigint &operator%=(const char *str_hex);
        
        bool operator>(const bigint &other) const;
        bool operator<(const bigint &other) const;
        bool operator==(const bigint &other) const;
        bool operator!=(const bigint &other) const;
        bool operator>=(const bigint &other) const;
        bool operator<=(const bigint &other) const;

};

#endif /* __LIBSYNC_API */