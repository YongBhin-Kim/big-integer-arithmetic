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
} STRUCT_BIGINT;

// void bi_delete(pbigint *x);


/*************************************************************************************
 * Big integer operation - Basic operation
**************************************************************************************/

void bi_delete(STRUCT_BIGINT **x);
void bi_new(STRUCT_BIGINT **x, const size_t wordlen); 
void bi_show_hex(const STRUCT_BIGINT *x);
// void bi_show_dec(const STRUCT_BIGINT *x);
void bi_show_bin(const STRUCT_BIGINT *x);
void bi_set_min_words(STRUCT_BIGINT **x, const int sign, const size_t wordlen);
void bi_set_by_array(STRUCT_BIGINT **x, const int sign, const word *a, const size_t wordlen);
int bi_set_by_string(STRUCT_BIGINT **x, const int sign, const char *str, int base);
void bi_refine(STRUCT_BIGINT *x);
void bi_assign(STRUCT_BIGINT **y, const STRUCT_BIGINT *x);
void bi_gen_rand(STRUCT_BIGINT **x, const int sign, const size_t wordlen);
void bi_set_one(STRUCT_BIGINT **x);
void bi_set_zero(STRUCT_BIGINT **x);
int bi_is_zero(const STRUCT_BIGINT *x);
int bi_is_one(const STRUCT_BIGINT *x);
int bi_compare(const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);
int bi_compare_abs(const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);
int bi_shift_left(STRUCT_BIGINT **x, size_t r);
int bi_shift_right(STRUCT_BIGINT **x, int r);
int bi_reduction(STRUCT_BIGINT **x, size_t r);



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
int bi_add_zxy(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);

/**
 * @brief 
 *  addition of two big integers
 * @param z : big integer (x + y)
 * @param x : big integer 
 * @param y : big integer 
 * @return 
 *  SUCCESS or FAIL
 */
int bi_add(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);

void bi_sign_flip(STRUCT_BIGINT *x);


/*************************************************************************************
 * Big integer operation - Subtraction
**************************************************************************************/
int bi_sub_zj(word *zj, word xj, word yj, int b);
int bi_sub_zxy(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);
int bi_sub(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);

/*************************************************************************************
 * Big integer operation - Multiplication
**************************************************************************************/
int word_compare(word *x, word *y, size_t x_wordlen, size_t y_wordlen);
int bi_mul(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y, const char *str);
int bi_mul_zj(word *zj, word xj, word yj);                                              // Single word mul
int bi_mul_text_zxy(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);                // Textbook mul
int bi_mul_karatsuba_zxy(STRUCT_BIGINT **z, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y, size_t flag); // Karatsuba mul
// int bi_mul_karatsuba_zxy(STRUCT_BIGINT **z, STRUCT_BIGINT *x, STRUCT_BIGINT *y, size_t flag); 
int bi_mul_itext_zxy(STRUCT_BIGINT** z, const STRUCT_BIGINT* x, const STRUCT_BIGINT* y); // Improved textbook mul

/*************************************************************************************
 * Big integer operation - Squaring
**************************************************************************************/
int bi_squaring_text_zx(STRUCT_BIGINT** z, const STRUCT_BIGINT* x);
int bi_squaring_karatsuba_zxy(STRUCT_BIGINT** z, const STRUCT_BIGINT* x, size_t flag);
int bi_squaring(STRUCT_BIGINT** z, const STRUCT_BIGINT* x);

/*************************************************************************************
 * Big integer operation - Modular Exponentiation by Montgomery Ladder
**************************************************************************************/
int bi_montgomery_mod_exp(STRUCT_BIGINT** z, const STRUCT_BIGINT* x, STRUCT_BIGINT* n, STRUCT_BIGINT* M);

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
int bi_div(STRUCT_BIGINT **q, STRUCT_BIGINT **r, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);

int bi_2word_div(word *q, const STRUCT_BIGINT *x, const word *y);
int bi_divcc(STRUCT_BIGINT **q, STRUCT_BIGINT **r, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);
int bi_divc(STRUCT_BIGINT **q, STRUCT_BIGINT **r, const STRUCT_BIGINT *x, const STRUCT_BIGINT *y);

/*************************************************************************************
 * Big integer operation - Fast Reduction
**************************************************************************************/
// Barrett Reduction
int bi_barrett_reduction(STRUCT_BIGINT **r, const STRUCT_BIGINT *x, const STRUCT_BIGINT *m, const STRUCT_BIGINT *t);

#ifdef __cplusplus
}
#endif

#endif /* BIGINT_H */