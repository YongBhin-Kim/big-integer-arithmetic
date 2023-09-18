#ifndef BIGINT_H
#define BIGINT_H

#include "array.h"

#define NON_NEGATIVE 0
#define NEGATIVE 1
#define ZERORIZE

/**
 * @brief Structure of big integer
 */
typedef struct {
    int sign;
    size_t wordlen;
    word *a;
} bigint;

void bi_delete(bigint **const x);
void bi_new(bigint **const x, const size_t wordlen);
void bi_show_hex(const bigint *x);
// void bi_show_dec(const bigint *x);
void bi_show_bin(const bigint *x);
void bi_set_by_array(bigint **const x, const int sign, const word *a, const size_t wordlen);
// int bi_set_by_string(bigint **const x, const int sign, const char *str, const int base);
void bi_refine(bigint *x);
void bi_assign(bigint **const y, const bigint *x);
void bi_gen_rand(bigint **const x, const int sign, const size_t wordlen);
void bi_set_one(bigint **const x);
void bi_set_zero(bigint **const x);
// void bi_add(bigint *z, bigint *x, bigint *y);
// void bi_add(bigint *z, bigint *x);
// void bi_sub(bigint *z, bigint *x, bigint *y);
// void bi_sub(bigint *z, bigint *x);

#endif