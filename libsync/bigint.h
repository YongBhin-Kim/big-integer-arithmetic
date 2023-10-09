#ifndef BIGINT_H
#define BIGINT_H

#include "array.h"

#define NON_NEGATIVE 0
#define NEGATIVE 1
#define ZERORIZE

#define FAIL  -303 // SUCCESS = 303
#define TRUE  127
#define FALSE -127

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
int bi_set_by_string(bigint **const x, const int sign, const char *str, const int base);
void bi_refine(bigint *x);
void bi_assign(bigint **const y, const bigint *x);
void bi_gen_rand(bigint **const x, const int sign, const size_t wordlen);
void bi_set_one(bigint **const x);
void bi_set_zero(bigint **const x);
int bi_is_zero(bigint **const x);
int bi_is_one(bigint **const x);
int bi_compare(bigint **const x, bigint **const y);
int bi_compare_abs(bigint **const x, bigint **const y);
int bi_shift_left(bigint **const x, size_t r);
int bi_shift_right(bigint **const x, int r);
int bi_reduction(bigint **const x, size_t r);

// void bi_add(bigint *z, bigint *x, bigint *y);
// void bi_add(bigint *z, bigint *x);
// void bi_sub(bigint *z, bigint *x, bigint *y);
// void bi_sub(bigint *z, bigint *x);

#endif