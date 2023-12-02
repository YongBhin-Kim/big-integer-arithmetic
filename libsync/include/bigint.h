#ifndef BIGINT_H
#define BIGINT_H

#include "array.h"

#define NON_NEGATIVE  0
#define NEGATIVE      1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 
 *  Structure of big integer
 */
typedef struct {
    int sign;
    size_t wordlen;
    word *a;
} bigint;

typedef bigint *pbigint;
// void bi_delete(pbigint *x);


/*************************************************************************************
 * Big integer operation - Basic operation
**************************************************************************************/

void bi_delete(bigint **x);
void bi_new(bigint **x, const size_t wordlen); 
void bi_show_hex(const bigint *x);
// void bi_show_dec(const bigint *x);
void bi_show_bin(const bigint *x);
void bi_set_min_words(bigint **x, const int sign, const size_t wordlen);
void bi_set_by_array(bigint **x, const int sign, const word *a, const size_t wordlen);
int bi_set_by_string(bigint **x, const int sign, const char *str, int base);
void bi_refine(bigint *x);
void bi_assign(bigint **y, const bigint *x);
void bi_gen_rand(bigint **x, const int sign, const size_t wordlen);
void bi_set_one(bigint **x);
void bi_set_zero(bigint **x);
int bi_is_zero(const bigint *x);
int bi_is_one(const bigint *x);
int bi_compare(const bigint *x, const bigint *y);
int bi_compare_abs(const bigint *x, const bigint *y);
int bi_shift_left(bigint **x, size_t r);
int bi_shift_right(bigint **x, int r);
int bi_reduction(bigint **x, size_t r);



/*************************************************************************************
 * Big integer operation - Addition 
**************************************************************************************/

/**
 * @brief 
 *  addition of two words which contains carry bits (zj <- xj + yj)
 * @param zj : single word integer (xj + yj + c)
 * @param xj : single word integer ([0,W))
 * @param yj : single word integer ([0,W))
 * @param c  : current carry bit which is computated by j-1-th word x, y and c (0 or 1)
 * @return 
 *  next carry bit (0 or 1)
 */
int bi_add_zj(word *zj, word xj, word yj, int c);

/**
 * @brief 
 *  addition of two word arrays (z <- x + y)
 * @param z : multi word word array (x + y)
 * @param x : multi word word array (n = wordlen of x)
 * @param y : multi word word array (m = wordlen of y) with n >= m always
 * @return 
 *  SUCCESS(127) or FAIL(-127)
 */
int bi_add_zxy(bigint **z, const bigint *x, const bigint *y);

/**
 * @brief 
 *  addition of two big integers
 * @param z : big integer (x + y)
 * @param x : big integer 
 * @param y : big integer 
 * @return 
 *  SUCCESS or FAIL
 */
int bi_add(bigint **z, const bigint *x, const bigint *y);

void bi_sign_flip(bigint *x);


/*************************************************************************************
 * Big integer operation - Subtraction
**************************************************************************************/
int bi_sub_zj(word *zj, word xj, word yj, int b);
int bi_sub_zxy(bigint **z, const bigint *x, const bigint *y);
int bi_sub(bigint **z, const bigint *x, const bigint *y);

/*************************************************************************************
 * Big integer operation - Multiplication
**************************************************************************************/
int word_compare(word *x, word *y, size_t x_wordlen, size_t y_wordlen);
int bi_mul(bigint **z, const bigint *x, const bigint *y, const char *str);
int bi_mul_zj(word *zj, word xj, word yj);                                              // Single word mul
int bi_mul_text_zxy(bigint **z, const bigint *x, const bigint *y);                // Textbook mul
int bi_mul_karatsuba_zxy(bigint **z, const bigint *x, const bigint *y, size_t flag); // Karatsuba mul
// int bi_mul_karatsuba_zxy(bigint **z, bigint *x, bigint *y, size_t flag); 
int bi_mul_itext_zxy(bigint** z, const bigint* x, const bigint* y); // Improved textbook mul

/*************************************************************************************
 * Big integer operation - Modular Exponentiation by Montgomery Ladder
**************************************************************************************/
int bi_montgomery_mod_exp(bigint** z, const bigint* x, const bigint* n, const bigint* M);

/*************************************************************************************
 * Big integer operation - Division
**************************************************************************************/

/**
 * @brief
 *  Long Division of two big integers
 * @details
 *  x = yq + r
 * @param q : Quotient
 * @param r : Remainder
 * @param x : Big integer X
 * @param y : Big integer Y
*/
int bi_div(bigint **q, bigint **r, const bigint *x, const bigint *y);

int bi_2word_div(word *q, const bigint *x, const word *y);
int bi_divcc(bigint **q, bigint **r, const bigint *x, const bigint *y);
int bi_divc(bigint **q, bigint **r, const bigint *x, const bigint *y);

/*************************************************************************************
 * Big integer operation - Fast Reduction
**************************************************************************************/
// Barrett Reduction
int bi_barrett_reduction(bigint **r, const bigint *x, const bigint *m, const bigint *t);

#ifdef __cplusplus
}
#endif

#endif /* BIGINT_H */
