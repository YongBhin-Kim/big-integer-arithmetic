#include "bigint.h"

int string_is_hex(const char *str) {
    long long pos = -1;
    long long   n = strlen(str);

    while (++pos != n) {
        if (
            ((str[pos] >= '0') && (str[pos] <= '9')) || 
            ((str[pos] >= 'a') && (str[pos] <= 'f')) || 
            ((str[pos] >= 'A') && (str[pos] <= 'F'))
           ) {}
        else {
            return FALSE;
        }
    }

    return TRUE;
}

int string_is_bin(const char *str) {
    long long pos = -1;
    long long   n = strlen(str);

    while (++pos != n) {
        if ( ((str[pos] >= '0') && (str[pos] <= '1')) ) {}
        else {
            return FALSE;
        }
    }

    return TRUE;
}

void bi_sign_flip(bigint *x) {
    // x->sign = (x->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE;
    x->sign ^= 1;
}

void bi_delete(bigint **x) { 
    if (*x == NULL) {
        return;
    }

#if ZERORIZE
    array_init((*x)->a, (*x)->wordlen);
#endif

    // free((*x)->a);
    // (*x)->a = NULL;
    // free(*x);
    // *x = NULL;

    if ( (*x)->a != NULL ) {
        free((*x)->a);
        (*x)->a = NULL;
    }
    free(*x);
    *x = NULL;
}

void bi_new(bigint **x, const size_t wordlen) {
    if (*x != NULL) {
        bi_delete(x);
    }

    (*x)          = (bigint *)malloc(sizeof(bigint));
    (*x)->sign    = NON_NEGATIVE;
    (*x)->wordlen = wordlen;

    (*x)->a       = NULL;
    if ( wordlen > 0 ) {
        (*x)->a       = (word *)malloc(wordlen * sizeof(word));  // [word] [word] .. : wordlen개
        array_init((*x)->a, wordlen);
    }
    else {
        (*x)->a = NULL;
    }
    
}

void bi_show_hex(const bigint *x) {
    int i;

    if (x->wordlen == 0) {
        return;
    }

    // printf(" (hex) ");

    /* print sign of the bigint 'x' */
    if (x->sign == NEGATIVE) {
        printf("-");
    }

    printf("0x");

    /* print words except last word */
    for (i = x->wordlen; i-- > 1;) {
        printf(HEX_FORMAT, x->a[i]);
    }
    /* print last word */
    printf(HEX_FORMAT, x->a[i]);
    printf("\n");

}

void bi_show_dec(const bigint *x);

void bi_show_bin(const bigint *x) {
    int j;
    int word_bits = sizeof(word) * 8;
    size_t i;

    if (x->wordlen == 0) {
        return;
    }

    printf(" (bin) ");
    if (x->sign == NEGATIVE) {
        printf("-");
    }
    word *p = x->a;

    for (i = x->wordlen; i-- > 1;) {
        for (j = word_bits-1; j >= 0; j--) {
            printf(DEC_FORMAT, (p[i] >> j) & 0x1);
        }
        printf(":");
    }
    for (j = word_bits-1 ; j >= 0; j--) {
        printf(DEC_FORMAT, (p[i] >> j) & 0x1);
    }
    printf("\n");
}

void bi_set_by_array(bigint **x, const int sign, const word *a, const size_t wordlen) {
    /* case address isn't allocated */
    if (a == NULL) {
        return;
    }

    bi_new(x, wordlen);
    (*x)->sign = sign;

    array_copy((*x)->a, a, wordlen);
}

void bi_set_min_words(bigint **x, const int sign, const size_t wordlen) {
    bi_new(x, wordlen);
    (*x)->sign = sign;
    (*x)->a[wordlen-1] = 1;
}


int bi_set_by_string(bigint **x, const int sign, const char *str, int base) {
    int ret       = FAIL,
        word_bits = WORD_BITS;
    size_t j;
    word value;
    long long pos;

    if ( (*x) != NULL ) {
        bi_delete(x);
    }

    /* 2진수 */
    if (base == 2) {
        if (string_is_bin(str) == TRUE) {
            bi_new(x, (strlen(str)-1) / word_bits + 1); 
            (*x)->sign = sign;

            for (pos = strlen(str)-1; pos >= 0; pos--) {
                j     = (strlen(str) - 1) - pos;
                value = (word)(str[pos] - '0');

                (*x)->a[j / (word_bits/1)] ^= value << ((j % (word_bits/1)) * 1);
            }
            bi_refine(*x);  // case str = '0.......'
        }
        /* string_is_bin(str) == FALSE */
        else {
            printf("Fatal error: given string is not binary value. exit from program.\n");
            return FAIL;
        }

        return SUCCESS;
    }
    /* 16진수 */
    else if (base == 16) {
        if (string_is_hex(str) == TRUE) {
            bi_new(x, strlen(str)/(WORD_BYTES*2) + 1);
            (*x)->sign = sign;

            for (pos = strlen(str)-1; pos >= 0; pos--) {
                j = (strlen(str) - 1) - pos;
                if ((str[pos] >= '0') && (str[pos] <= '9')) {
                    value = (word)(str[pos] - '0');
                }
                else if ((str[pos] >= 'a') && (str[pos] <= 'f')) {
                    value = (word)(str[pos] - 'a') + 10;
                }
                else if ((str[pos] >= 'A') && (str[pos] <= 'F')) {
                    value = (word)(str[pos] - 'A') + 10;
                }
                (*x)->a[j / (word_bits/4)] ^= value << ((j % (word_bits/4)) * 4);
            }
            bi_refine(*x);  // case str = '0.......'
        }
        /* string_is_hex() == FALSE */
        else {
            printf("Fatal error: given string is not hexadecimal value. exit from program.\n");
            return FAIL;
        }

        return SUCCESS;
    }

    /* 10진수 */

    return ret;
}

void bi_refine(bigint *x) {

    if (x == NULL) {
        return;
    }

    size_t new_wordlen = x->wordlen;
    while (new_wordlen > 1) {   // at least one word needed
        if (x->a[new_wordlen - 1] != 0) {
            break;
        }
        new_wordlen--;
    }
    if (x->wordlen != new_wordlen) {
        x->wordlen = new_wordlen;
        x->a       = (word *)realloc(x->a, sizeof(word)*new_wordlen);
    }

    if ((x->wordlen == 1) && (x->a[0] == 0x0)) {
        x->sign = NON_NEGATIVE;
    }
}

void bi_assign(bigint **y, const bigint *x) {   // y <- x

    if ( *y != NULL ) {
        bi_delete(y);
    }

    bi_new(y, x->wordlen);
    array_copy((*y)->a, x->a, x->wordlen);
    (*y)->sign = x->sign;
}

void bi_gen_rand(bigint **x, const int sign, const size_t wordlen) {
    
    bi_new(x, wordlen);
    array_rand((*x)->a, wordlen);
    (*x)->sign = sign;

    bi_refine(*x);
}

void bi_set_one(bigint **x) {

    if (*x != NULL) {
        bi_delete(x);
    }

    bi_new(x, 1);
    (*x)->a[0] = 0x1;
    (*x)->sign = NON_NEGATIVE;
}

void bi_set_zero(bigint **x) {

    if (*x != NULL) {
        bi_delete(x);
    }
 
    bi_new(x, 1);
    (*x)->a[0] = 0x0;
    (*x)->sign = NON_NEGATIVE;
}

void bi_set_zero_not_refine(bigint **x) {

    if ( *x == NULL || (*x)->a == NULL )
        return;
    memset((*x)->a, 0, WORD_BYTES * (*x)->wordlen);
}

int bi_is_zero(const bigint* x) {
    int ret = FALSE;
    size_t j;

    if ((x->sign == NEGATIVE) || (x->a[0] != 0)) {
        return ret;
    }

    for (j = x->wordlen - 1; j >= 1; j--) {
        if (x->a[j] != 0) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}

int bi_is_one(const bigint* x) {
    int ret = FALSE;
    size_t j;

    if ((x->sign == NEGATIVE) || (x->a[0] != 1)) {
        return ret;
    }

    for (j = x->wordlen - 1; j >= 1; j--) {
        if (x->a[j] != 0) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}


/* Compare two words */
int word_compare(word *x, word *y, size_t x_wordlen, size_t y_wordlen) {
    if (x_wordlen > y_wordlen) {
        return  1;
    }
    else if (x_wordlen < y_wordlen) {
        return -1;
    }
    else {
        int j;
        for (j = x_wordlen; j >= 0; j--) {
            if      (x[j] > y[j]) {
                return 1;
            }
            else if (x[j] < y[j]) {
                return -1;
            }
        }

        return 0;
    }
}

/*
    compare(x, y) =  1 (x > y)
    compare(x, y) =  0 (x = y)
    compare(x, y) = -1 (x < y)
    or FAIL
*/
int bi_compare(const bigint *x, const bigint *y) {
    int ret;

    /* case exception */
    if ((x == NULL) || (y == NULL)) {
        return FAIL;
    }

    /* x > y */
    if ((x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE)) {
        return 1;
    }

    /* x < y */
    if ((x->sign == NEGATIVE) && (y->sign == NON_NEGATIVE)) {
        return -1;
    }

    /* (x, y > 0) or (x, y < 0) */
    ret = bi_compare_abs(x, y);

    if (x->sign == NON_NEGATIVE) {
        return ret;             // 1, 0, -1
    }
    else {
        return ret * (-1);      // -1, 0, 1
    }

    /* case exception */
    return FAIL;
}

/*
    compareabs(x, y) =  1 (x > y)
    compareabs(x, y) = -1 (x < y)
    compareabs(x, y) =  0 (x = y)
    or FAIL
*/
int bi_compare_abs(const bigint *x, const bigint *y) {
    size_t j, x_wordlen = x->wordlen, y_wordlen = y->wordlen;
    
    if (x_wordlen > y_wordlen) {   // x > y
        return  1;
    }
    else if (x_wordlen < y_wordlen) {   // x < y
        return -1;
    }

    /* case x_wordlen is same with y_wordlen */
    for (j = x_wordlen; j-- > 0;) {
        if (x->a[j] > y->a[j]) {   // x > y
            return  1;
        }
        else if (x->a[j] < y->a[j]) {   // x < y
            return -1;
        }
    }

    return 0;   // x == y
}

int bi_shift_left(bigint **x, size_t r) {
    size_t n = (*x)->wordlen;
    size_t w = (size_t)(sizeof(word) * 8);
    size_t k, j;
    int rp;
    bigint *y = NULL;

    // case r == wk
    // x <- x << r (= x << wk)
    if ( r%w == 0 ) {
        k = r/w; 

        // y <- x, new allocate k-words
        bi_new(&y, n + k);
        y->sign = (*x)->sign;
        array_copy(y->a+k, (*x)->a, (*x)->wordlen);

        // x <- y
        bi_assign(x, y);
        bi_delete(&y);

        return 1;
    }
    // case r = wk + r' where 0 < r' < w
    else {
        k = r/w;
        rp = r%w;
#if 0
        printf("k = %d, rp = %d\n", k, rp);
#endif
        word c = (word)(WORD_BITS - rp), cb = (1 << (c + 1)) - 1; 
        bi_new(&y, n + 1);
        y->sign = (*x)->sign;

        y->a[n] = (*x)->a[n-1] >> c;
        for (j = n - 1; j > 0; j--) {
            y->a[j] = (((*x)->a[j]) & cb) << rp | ((*x)->a[j-1]) >> c;
        }
        y->a[0] = ((*x)->a[0] << rp);

        bi_refine(y);
        bi_assign(x, y);
        bi_delete(&y);

        if (k > 0) {
            return bi_shift_left(x, k * WORD_BITS);
        }

        return 1;

        // bi_new(&y, n + k + 1);
        // y->sign = (*x)->sign;

        // j = k;
        // y->a[j] = ((*x)->a[j-k] << (32-rp)) & 0xffffffff;
        // j++;
        // for ( ; j<n+k+1; j++ ) {
        //     y->a[j] = ((((*x)->a[j-k] << rp)) & 0xffffffff) | ((*x)->a[j-k-1] >> (32-rp));
        // }

        // // x <- y
        // bi_assign(x, y);
        // bi_delete(&y);

        // return 1;
    }

    return -1;
}

/* r: shift bits */
int bi_shift_right(bigint **x, int r) {
    if (r < 0) {
        bi_shift_left(x, (-1)*r);
        return 1;
    }
    else if (r == 0) {
        return 1;
    }

    int rp;
    size_t k, j,
           n = (*x)->wordlen,    // wordlen
           w = (size_t)(sizeof(word) * 8);      // word bits (= 32)

    /* case r >= wn  */
    if (r >= (w*n)) {
        bi_set_zero(x);
        return 1; 
    }

    /*
        case r == wk 

         w = wn-1 ... wk wk-1 ... w0 (n개)
    ->   w = wn-1 ... wk 
           = 0 0 ... wn-1 ... wk (n-k개)
         erase r-bit means erase k-words
        
         (procedure right shift)
         w0 <- wk
         w1 <- wk+1
         ...
         wn-k-1 <- wn-1
         wn-k <- 0
         ...
         wn-1 <- 0
    
         (procedure refine)
    */
    else if (r%w == 0) {
        k = r / w;

        for (j = 0; j < n-k; j++) {
            (*x)->a[j] = (*x)->a[j+k];
        }
        for (; j < n; j++) {
            (*x)->a[j] = 0;
        }
#if DEBUG_OUT
        printf("before refine, x = "); bi_show_hex(*x);
#endif
        bi_refine(*x); // 함수 안에서 wordlen = n-k 까지 처리
#if DEBUG_OUT
        printf("before refine, x = "); bi_show_hex(*x);
#endif
        return 1;
    }
    /* case r = wk + r' where 0 < r' < w */
    else {
        k  = r / w;
        rp = r % w;
#if DEBUG_OUT
        printf(" k = %d, rp = %d\n", k, rp);
#endif
        for (j = 0; j < n-k-1; j++) {
            (*x)->a[j] = ((*x)->a[j+k+1] << (WORD_BITS-rp)) | ((*x)->a[j+k] >> rp);
        }
        (*x)->a[j] = ((*x)->a[j+k] >> rp);
        j++;
        for (; j < n; j++) {
            (*x)->a[j] = 0;
        }
        bi_refine(*x);

        return 1;
    }

    /* exception */
    return -1;
}

/* bigint, reduction bits */
int bi_reduction(bigint **x, size_t r) {
    int rp;
    size_t k, j,
           w = (size_t)(sizeof(word) * 8),      // 32
           n = (*x)->wordlen;

    /* treating error case */
    if (r < 0) {
        return -1;
    }
    /* 
        case r >= wn
        A mod 2^r = A
    */
    if (r >= (w*n)) {
        return 1;
    }
    /* case r = wk where k < n */
    else if ((r%w) == 0) {
        k = r / w;
        for (j = k; j < n; j++) {
            (*x)->a[j] = 0;
        }
        bi_refine(*x);  // erase unused memory and modify wordlen of x

        return 1;
    }
    /* case r = wk + r' where k < n and 0 < r' < w */
    else {
        k  = r / w;
        rp = r % w;
        (*x)->a[k] = (((*x)->a[k] << (w - rp))) >> (w - rp);
        for (j = k+1; j < n; j++) {
            (*x)->a[j] = 0;
        }
        bi_refine(*x);

        return 1;
    }

    return -1;
}


/*
    big integer addition

     word, word, current carry
     xj + yj + c mod W
     C <- xj + yj
     C <- C + c
*/
int bi_add_zj(word *zj, word xj, word yj, int c) {
    int cp;     // next carry

    *zj = xj + yj;      // C <- xj + yj mod W
    cp  = (*zj < xj);   // is next carry exists?

    *zj = *zj + c;          // C <- C + carry
    cp  = (*zj < c) + cp;   // cp 

    return cp;
}

/*
    z <- x + y
    wordlen(x) >= wordlen(y)
*/
int bi_add_zxy(bigint **z, const bigint *x, const bigint *y) {
    int c;
    size_t j,
           n = x->wordlen,
           m = y->wordlen;      // n >= m

    bi_new(z, n+1);

    bigint *y_expand = NULL;    // copy of y
    if (n == m) {
        bi_assign(&y_expand, y);
    }
    else if (n > m) {
        /* y_expand <- 0...0 || y, allocate new zero n-m+1-words */
        bi_new(&y_expand, n);
        array_copy(y_expand->a, y->a, m);
        y_expand->sign = y->sign;
        for (j = m; j < n; j++) {
            y_expand->a[j] = 0;
        }
    }
    c = 0;      // set the first word carry bit zero
    
    for (j = 0; j < n; j++) {
        // c, Cj <- bi_add_xyc(xj, yj, c);
        c = bi_add_zj(&((*z)->a[j]), x->a[j], y_expand->a[j], c);
    }

    /* last carry */
    (*z)->a[n] = c;     // 1 or 0 --> wordlen is n+1 or n
    bi_refine(*z);

    bi_delete(&y_expand);
    return 1;
}

/*
    addition of two integers
     z <- x + y
     input: x, y \in Z
     output: z = x + y \in Z
*/
int bi_add(bigint **z, const bigint *x, const bigint *y) {
    bigint *x_abs = NULL, 
           *y_abs = NULL;

    /* case x = 0 */
    if (bi_is_zero(x) == TRUE || x == NULL) {
        bi_assign(z, y);
        return 1;
    }
    /* case y = 0 */
    else if (bi_is_zero(y) == TRUE || y == NULL) {
        bi_assign(z, x);
        return 1;
    }
    /* case x > 0 and y < 0 */
    else if ((x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE)) {
        // bi_sub(z, x, |y|);
        bi_assign(&y_abs, y);
        bi_sign_flip(y_abs);
        bi_sub(z, x, y_abs);
        bi_delete(&y_abs);
        return 1;
    }
    /* case x < 0 and y > 0 */
    else if ((x->sign == NEGATIVE) && (y->sign == NON_NEGATIVE)) {
        // bi_sub(z, y, |x|);
        bi_assign(&x_abs, x);
        bi_sign_flip(x_abs);
        bi_sub(z, y, x_abs);
        bi_delete(&x_abs);
        return 1;
    }
    /* case x > 0 and y > 0 or x < 0 and y < 0 */
    else if (x->wordlen >= y->wordlen) {
        bi_add_zxy(z, x, y);
        return 1;
    }
    else {
        bi_add_zxy(z, y, x);
        return 1;
    }

    return -1;
}

int bi_sub_zj(word *zj, word xj, word yj, int b) {
    int bp;     // next borrow

    *zj = xj - b;           // modular subtraction
    bp = (xj < b);          // is next carry exists?
    bp = bp + (*zj < yj);   // is next carry exists? (0 or 1)
    *zj = *zj - yj;         // modular subtraction

    return bp;
}

/*
    z <- x + y
    always x >= y > 0
*/
int bi_sub_zxy(bigint **z, const bigint *x, const bigint *y) {

    int b;
    size_t j, 
           n = x->wordlen, 
           m = y->wordlen;      // n >= m
    
    bi_new(z, n);
    bigint *y_expand = NULL;    // copy of y
    if (n == m) {
        bi_assign(&y_expand, y);
    }
    else if (n > m) {
        /* y_expand <- 0...0 || y, allocate new zero n-m+1-words */
        bi_new(&y_expand, n);
        array_copy(y_expand->a, y->a, m);
        y_expand->sign = y->sign;
        for (j = m; j < n; j++) {
            y_expand->a[j] = 0;
        }
    }

    b = 0;      // set the first word borrow bit zero

    for (j = 0; j < n; j++) {
        // b <- bi_sub_zj(zj, xj, yj, b);
        b = bi_sub_zj(&((*z)->a[j]), x->a[j], y_expand->a[j], b);
    }

    bi_delete(&y_expand);
    bi_refine(*z);

    return 1;
}

/*
    Subtraction of two integers
     z <- x - y
*/
int bi_sub(bigint **z, const bigint *x, const bigint *y) {
    int cmp;
    bigint *x_temp = NULL,
           *y_temp = NULL;

    /* case x is zero */
    if (bi_is_zero(x) == TRUE || x == NULL) {
        bi_assign(z, y);
        bi_sign_flip(*z);

        return SUCCESS;
    }
    else if (bi_is_zero(y) == TRUE || y == NULL) {
        bi_assign(z, x);

        return SUCCESS;
    }

    cmp = bi_compare(x, y);
    /* x == y */
    if (cmp == 0) {
        bi_set_zero(z);
        return SUCCESS;
    }
    /* x, y >= 0 */
    if ((x->sign == NON_NEGATIVE) && (y->sign == NON_NEGATIVE)) {
        /* x >= y >= 0 : sub(x, y) */
        if (cmp ==  1) {
            bi_sub_zxy(z, x, y);
        }
        /* y > x >= 0 : -sub(y, x) */
        else if (cmp == -1) {
            bi_sub_zxy(z, y, x);
            (*z)->sign = NEGATIVE;
        }

        return SUCCESS;
    }
    /* x, y < 0 */
    else if ((x->sign == NEGATIVE) && (y->sign == NEGATIVE)) {
        bi_assign(&x_temp, x);
        x_temp->sign = NON_NEGATIVE;
        bi_assign(&y_temp, y);
        y_temp->sign = NON_NEGATIVE;

        /* 0 > x >= y : sub(|y|, |x|) */
        if (cmp ==  1) {
            bi_sub_zxy(z, y_temp, x_temp);
        }
        /* 0 > y > x : -sub(|x|, |y|) */
        else if (cmp == -1) {
            bi_sub_zxy(z, x_temp, y_temp);
            (*z)->sign = NEGATIVE;
        }
        
        bi_delete(&x_temp);
        bi_delete(&y_temp);
        return SUCCESS;
    }
    /* x > 0 and y < 0 : add(x, |y|) */
    else if ((x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE)) {
        bi_assign(&y_temp, y);
        y_temp->sign = NON_NEGATIVE;

        bi_add(z, x, y_temp);

        // bi_delete(&x_temp);
        bi_delete(&y_temp);
        return SUCCESS;
    }
    /* x < 0 and y > 0 : -add(|x|, y) */
    else {
        bi_assign(&x_temp, x);
        x_temp->sign = NON_NEGATIVE;

        bi_add(z, x_temp, y);
        (*z)->sign = NEGATIVE;

        bi_delete(&x_temp);
        // bi_delete(&y_temp);
        return SUCCESS;
    }

    return FAIL;
}

/*
    Multiplication of two integers
     z <- x * y
*/
int bi_mul(bigint **z, const bigint *x, const bigint *y, const char *str) {
    /* Case x or y is zero */
    if ( bi_is_zero(x) == TRUE || bi_is_zero(y) == TRUE ) {
        bi_set_zero(z);
        return SUCCESS;
    }

    /* Case x is one */
    if ( bi_is_one(x) == TRUE ) {
        bi_assign(z, y);        // return y
        return SUCCESS;
    }
    /* Case y is one */
    if ( bi_is_one(y) == TRUE ) {
        bi_assign(z, x);        // return x
        return SUCCESS;
    }
    /* Case x is minus one */
    bigint *temp = NULL;
    bi_assign(&temp, x);
    bi_sign_flip(temp);
    if (bi_is_one(temp) == TRUE) {
        bi_assign(z, y);        // return -y
        bi_sign_flip(*z);
        bi_delete(&temp);

        return SUCCESS;
    }
    /* Case y is minus one */
    bi_assign(&temp, y);
    bi_sign_flip(temp);
    if ( bi_is_one(temp) == TRUE ) {
        bi_assign(z, x);        // return -x
        bi_sign_flip(*z);
        bi_delete(&temp);

        return SUCCESS;
    }

    bi_delete(&temp);

    /*
        Case else
         z <- x * y
    */
    bigint *x_ = NULL,
           *y_ = NULL;
    bi_assign(&x_, x);
    bi_assign(&y_, y);
    x_->sign = NON_NEGATIVE;
    y_->sign = NON_NEGATIVE;

    if (!strcmp("Textbook", str)) {
        if (bi_mul_text_zxy(z, x_, y_) == FAIL) {
            bi_delete(&x_);
            bi_delete(&y_);
            return FAIL;
        }
    }
    else if (!strcmp("Improved Text", str)) {
        bi_mul_itext_zxy(z, x, y);
    }
    else if (!strcmp("Karatsuba", str)) {
        if (bi_mul_karatsuba_zxy(z, x_, y_, 3) == FAIL) {
            bi_delete(&x_);
            bi_delete(&y_);
            return FAIL;
        }
    }
    (*z)->sign = x->sign ^ y->sign;

    bi_delete(&x_);
    bi_delete(&y_);
    return SUCCESS;
}

/*
    alg. 12. single word mul
     z \in [0, W^2) : 1 word or 2 words
*/
int bi_mul_zj(word *zj, word xj, word yj) {
    word a1, a0, b1, b0, t1, t0, c1, c0, t;

    a1 = xj >> (WORD_BITS/2);       // a1 = xj >> W/2 (line 2)
    a0 = xj & ((word) (((word)1 << (WORD_BITS/2)) - 1));      // 2^{wordbits-1} - 1 (line 2)
    b1 = yj >> (WORD_BITS/2);       // (line 3)
    b0 = yj & ((word) (((word)1 << (WORD_BITS/2)) - 1));      // 2^{wordbits-1} - 1 (line 3)

    t1 = a1 * b0; t0 = a0 * b1;     // t1 <- a1b0, t0 <- a0b1 (line 4)
    t0 = t1 + t0;       // modular addition
    t1 = t0 < t1;       // word compare
    c1 = a1 * b1;
    c0 = a0 * b0;       // line 7

    t = c0;
    c0 = c0 + (t0 << (WORD_BITS/2));
    zj[0] = c0;
    zj[1] = c1 + (t1 << (WORD_BITS/2)) + (t0 >> (WORD_BITS/2)) + (c0 < t);

    return SUCCESS;
}

/*
    Textbook Multiplication core code
     bigint z <- bigint x * bigint y
*/
int bi_mul_text_zxy(bigint **z, const bigint *x, const bigint *y) {
    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        printf("Fatal Error: Textbook multiplication input value should be NON NEGATIVE\n");
        return FAIL;
    }

    size_t i, j,
           n = x->wordlen, 
           m = y->wordlen;
    bigint *c = NULL, 
           *t = NULL;

    bi_new(&c, n+m);        // consider 5.1. Memory for mul
    bi_new(&t, 2);

    for (j = 0; j < n; j++) {
        for (i = 0; i < m; i++) {
            bi_mul_zj(t->a, x->a[j], y->a[i]);      // t[0~1] <- x_j * y_i // single word multiplication
            bi_shift_left(&t, WORD_BITS*(i+j));     // t <- t << w(i+j);
            bi_add(z, c, t);
            bi_assign(&c, *z);
            bi_new(&t, 2);
        }
    }
    bi_refine(*z);
    bi_delete(&c);
    bi_delete(&t);

    return SUCCESS;
}

/*
    Karatsuba Multiplication core code
     z <- |x| * |y|
*/
int bi_mul_karatsuba_zxy(bigint **z, const bigint *x, const bigint *y, size_t flag) {
    if (x->sign == NEGATIVE || y->sign == NEGATIVE) {
        printf("Fatal Error: Karatsuba multiplication input value should be NON NEGATIVE\n");
        return FAIL;
    }

    size_t l, lw, 
           n = x->wordlen, 
           m = y->wordlen;
    bigint *a1 = NULL, *a0 = NULL, 
           *b1 = NULL, *b0 = NULL, 
           *t1 = NULL, *t0 = NULL, 
           *c  = NULL, *c_ = NULL, 
           *s_ = NULL, *s  = NULL, *s1 = NULL, *s0 = NULL,
           *modulo = NULL, *a0_ = NULL, *b0_ = NULL;

    /* flag 2 */
    if (flag >= MIN(n, m)) {
        return bi_mul_text_zxy(z, x, y);
    }

    /*
        l = max(n, m) / 2, 
        lw = bits of l-words
    */
    l  = (size_t) ((MAX(n, m) + 1) >> 1);
    lw = l * WORD_BITS; 

    /* a1 = a >> lw */
    bi_assign(&a1, x);
    bi_shift_right(&a1, lw);

    /* modulo = 2^lw */
    bi_set_min_words(&modulo, NON_NEGATIVE, l+1);

    /*
        a0 = (a mod 2^lw) (a0 is always non-negative in this function.)
         if a is negative, then a0 = (2^lw) + (|a| mod 2^lw)
    */
    bi_set_by_array(&a0, x->sign, x->a, l);     // NON_NEGATIVE
    bi_refine(a0);

    /* b1 = b >> lw */
    bi_assign(&b1, y);
    bi_shift_right(&b1, lw);
    
    /* b0 = (b mod 2^lw) */
    bi_set_by_array(&b0, y->sign, y->a, l); 
    
    /* t1, t0 = mul(a1, b1), mul(a0, b0) */
    bi_mul_karatsuba_zxy(&t1, a1, b1, flag);
    bi_mul_karatsuba_zxy(&t0, a0, b0, flag);
    
    /* c = (t1 << 2lw) + t0 */
    bi_assign(&c_, t1);
    bi_shift_left(&c_, 2*lw);
    bi_add(&c, c_, t0);
    bi_assign(z, c);
    
    /* s1, s0 = sub(a0, a1), sub(b1, b0) */
    bi_sub(&s1, a0, a1);
    bi_sub(&s0, b1, b0);
    
    /* s = (-1)^{sign of s1 ^ sign of s2} * mul(|s1|, |s0|) */
    int sign = s1->sign ^ s0->sign; 
    bi_new(&s_, 1);

    s1->sign = NON_NEGATIVE;
    s0->sign = NON_NEGATIVE;
    bi_mul_karatsuba_zxy(&s_, s1, s0, flag);
    s_->sign = sign;

    /* s = add(s, t1), s = add(s, t0) */
    bi_add(&s, s_, t1);
    bi_assign(&s_, s);
    bi_add(&s, s_, t0);

    /* s = s << lw */
    bi_shift_left(&s, lw);

    /* z = add(c, s) */
    bi_add(z, c, s);

    /* free bigintegers */
    bi_delete(&a1);
    bi_delete(&a0);
    bi_delete(&b1);
    bi_delete(&b0);
    bi_delete(&t1);
    bi_delete(&t0);
    bi_delete(&c);
    bi_delete(&c_);
    bi_delete(&s_);
    bi_delete(&s);
    bi_delete(&s1);
    bi_delete(&s0);
    bi_delete(&modulo);
    bi_delete(&a0_);
    bi_delete(&b0_);
    return SUCCESS;
}

/*
    Long Division Algorithms
     - Multi-Precision Long Division (bi_div)
     - DIVC (bi_divc)
     - DIVCC (bi_divcc)
     - 2-Word Long Division (bi_2word_div)

     Function-call diagram:
      bi_div()
      ⎿ bi_divc()
        ⎿ bi_divcc()
          ⎿ bi_2word_div()
*/

/*
    2-Word Long Division
     input : X1·W + X0, Y
     output: Q
*/
int bi_2word_div(word *q, const bigint *x, const word *y) {
    if (q == NULL || x == NULL || y == NULL) {
        return FALSE;
    }

    int j = WORD_BITS - 1;
    word w = 1 << j,
         r = x->a[1];

    *q = 0;

    while (j >= 0) {
        if (r >= w) {
            (*q) += (1 << j);
            ( r)  = (2*r) + ((x->a[0] >> j) & 0x1) - (*y);
        }
        else {
            r = (2*r) + ((x->a[0] >> j) & 0x1);
            if (r >= (*y)) {
                (*q) += (1 << j);
                ( r) -= *y;
            }
        }
        j--;
    }

    return TRUE;
}

/*
    DIVCC
     input : Xn-1·W^{n-1} + Xn-2·W^{n-2} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
     output: Q, R
*/
int bi_divcc(bigint **q, bigint **r, const bigint *x, const bigint *y) {
    if (x == NULL || y == NULL) {       // Check input
        return FALSE;
    }
    
    size_t  n = x->wordlen, m = y->wordlen;
    bigint *a = NULL, *t = NULL;

    bi_new(q, 1);

    if (n == m) {
        (*q)->a[0] = x->a[m-1] / y->a[m-1];     // Q = Xm-1 / Ym-1
    }
    else if (n == (m + 1)) {
        if (x->a[m] != y->a[m-1]) {
            /*
                Q = A / Ym-1
                A = Xm·W + Xm-1
            */
            bi_new(&a, 2);
            a->a[1] = x->a[m];
            a->a[0] = x->a[m-1];

            if (bi_2word_div(&((*q)->a[0]), a, &(y->a[m-1])) == FALSE) {
                bi_delete(&a);
                return FALSE;
            }
        }
        else {
            (*q)->a[0] = WORD_MAX;
        }
    }
    else {
        return FALSE;
    }

    /* R = X - YQ */
    bi_mul(&t, y, *q, "Karatsuba");
    bi_sub(r, x, t);

    while ((*r)->sign == NEGATIVE) {        // R < 0
        /* 
            Q = Q - 1
            T = R + Y
            R = T
        */
        (*q)->a[0]--;
        bi_add(&t, *r, y);
        bi_assign(r, t);
    }

    bi_delete(&a);
    bi_delete(&t);
    return TRUE;
}

/*
    DIVC
     input : Xm·W^m + Xm-1·W^{m-1} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
     output: Q, R

     if wordlen(X) is in {wordlen(Y), wordlen(Y) + 1}
      else return Q = 0, R = X
*/
int bi_divc(bigint **q, bigint **r, const bigint *x, const bigint *y) {
    if (x == NULL || y == NULL) {       // Check input
        return FALSE;
    }

    /*
        X < Y, 
         Q = 0
         R = X
    */
    if (bi_compare_abs(x, y) == -1) {
        bi_set_zero(q);
        bi_assign(r, x);
        bi_refine(*r);
        return TRUE;
    }

    word    w = (word) 1 << (WORD_BITS - 1);
    size_t  m = y->wordlen;
    bigint *a = NULL, *b  = NULL;

    /* 
        Compute k (k >= 0) such that 2^{WORD-1} <= (2^k · Ym-1) < 2^{WORD}
    */
    size_t k = 0;
    while (k < WORD_BITS) {
        if ((y->a[m-1] << k) >= w) {
            if ((y->a[m-1] << k) <= WORD_MAX) {
                break;
            }
            else {
                return FALSE;
            }
        }
        k++;
    }

    bi_assign(&a, x);
    bi_shift_left(&a, k);       // A = 2^k · X 
    bi_assign(&b, y);
    bi_shift_left(&b, k);       // A = 2^k · Y
    
    if (bi_divcc(q, r, a, b) == FALSE) {
        bi_delete(&a);
        bi_delete(&b);
        return FALSE;
    }

    bi_shift_right(r, k);       // 2^{-k} · R
    bi_refine(*r);

    bi_delete(&a);
    bi_delete(&b);
    return TRUE;
}

/*
    Multi-Precision Long Division
     input : Xn-1·W^{n-1} + Xn-2·W^{n-2} + ··· + X0, Ym-1·W^{m-1} + Ym-2·W^{m-2} + ··· + Y0
     output: Q, R such that X = YQ + R
*/
int bi_div(bigint** q, bigint** r, const bigint* x, const bigint* y) {
    if (x == NULL || y == NULL) {       // Check input
        return FAIL;
    }

    //추가된 부분 -> y는 음수나 0이 될 수 없음!
    if (y->sign == NEGATIVE || bi_is_zero(y) == TRUE) {
        printf("sign of y is only NON_NEGATIVE and y can not setting zero!\n");
        return FAIL;
    }

    /*
        X < Y,
         Q = 0
         R = X
    */
    if (bi_compare_abs(x, y) == -1) {
        bi_set_zero(q);
        bi_assign(r, x);
        return SUCCESS;
    }

    size_t i, n = x->wordlen;
    bigint* _q = NULL, * _r1 = NULL, * _r2 = NULL;

    bi_new(q, x->wordlen);
    bi_set_zero(&_r1);

    for (i = n; i > 0; i--) {
        /* R1 = R0·W + Xi */
        bi_shift_left(&_r1, WORD_BITS);
        _r1->a[0] = x->a[i - 1];
        bi_refine(_r1);

        bi_divc(&_q, &_r2, _r1, y);

        bi_assign(&_r1, _r2);       // Update R0
        (*q)->a[i - 1] = _q->a[0];    // Q = Qn-1·W^{n-1} + Qn-2·W^{n-2} + ··· + Q0
    }


    //결과 처리
    
    if (x->sign == NEGATIVE) {
        //x가 음수라면
        
        //q <- -q-1 
        bigint* buf = NULL;
        (*q)->sign = NEGATIVE;
        bigint* one = NULL;
        bi_set_one(&one);
        bi_sub(&buf, (*q), one);
        bi_assign(q, buf);

        //r <- b-r
        bi_sub(&buf, y, _r1);
        bi_assign(r, buf);

        //memory leak
        bi_delete(&buf);
        bi_delete(&one);

    }
    else {
        //x가 양수라면
        bi_assign(r, _r1);
        bi_refine(*q);
    }
   

    bi_delete(&_q);
    bi_delete(&_r1);
    bi_delete(&_r2);
    return SUCCESS;
}

/*
    Barrett Reduction
     X is in [0, W^{2n}) 2n-word
     N is in [W^{n-1}, W^n) fixed n-word
     r <- x mod m
*/
int bi_barrett_reduction(bigint **r, const bigint *x, const bigint *m, const bigint *t) {

    if (x == NULL || m == NULL || t == NULL) {
        return FAIL;
    }

    size_t n = m->wordlen;

    if ( x->wordlen > (2*n) ) {
        return FAIL;
    }

    bigint *r_ = NULL, 
           *qh = NULL, 
           *x_ = NULL;

    bi_assign(&x_, x);
    bi_shift_right(&x_, WORD_BITS * (n - 1));
    bi_mul(&qh, x_, t, "Karatsuba");
    bi_shift_right(&qh, WORD_BITS * (n + 1));
    bi_mul(&r_, m, qh, "Karatsuba");
    bi_sub(r, x, r_);

    while (bi_compare(*r, m) >= 0) {
        bi_sub(&r_, *r, m); 
        bi_assign(r, r_); 
    }

    bi_delete(&qh);
    bi_delete(&x_);
    bi_delete(&r_);
    return SUCCESS;
}





//내가 추가한 함수 -> 메모리 배열은 realloc을 이용해서 재할당 해주는 함수
//x를 *형태로 구현해도 상관x
int bi_realloc(bigint** const x, size_t size)
{
    word* new_x = (word*)realloc((*x)->a, sizeof(word) * (size));
    if (new_x == NULL) {
        free(new_x);
        new_x = NULL;
        return FAIL;
    }
    (*x)->a = new_x;
    return SUCCESS;
}

int bi_mul_itext_zxy(bigint** z, const bigint* x, const bigint* y) {
    //11.13 - 수정사항에서 추가된 변수
    bigint* new_x = NULL;
    bi_assign(&new_x, x);
    bigint* new_y = NULL;
    bi_assign(&new_y, y);

    
    if (new_x->wordlen % 2 != 0) {
        size_t new_wordlen = new_x->wordlen + 1;
        bi_realloc(&new_x, sizeof(word) * (new_wordlen));
        new_x->wordlen = new_wordlen;
        new_x->a[new_wordlen - 1] = 0;
    }
    if (new_y->wordlen % 2 != 0) {
        size_t new_wordlen = new_y->wordlen + 1;
        bi_realloc(&new_y, sizeof(word) * (new_wordlen));
        new_y->wordlen = new_wordlen;
        new_y->a[new_wordlen - 1] = 0;
    }

    //loop에 필요한 변수
    size_t n = new_x->wordlen / 2;
    int cnt_i, cnt_j;

    //미리 T0, T1을 크게 잡고 시작 -> CONCAT을 효율적으로 해주기 위해서!
    //T0는 2WORD 크기의 값을 n번(0 ~ n-1) CONCAT 하므로 -> 2 * n 만큼 잡음
    //T1은 2WORD 크기의 값을 n번 CONCAT + 맨 처음에 ALL ZERO짜리 1WORD를 CONCAT 하고 시작하므로 -> 2 * n + 1
    //아래 안쪽 for loop에서 필요하지만 -> 해당 위치에 bi_new를 쓰면 불필요한 bi_new 반복 호출 -> 비효율
    //그래서 아래 위치에 bi_new를 이용해서 T0,T1을 할당 해놓고 계속 재활용 할 예정!

    bigint* T0 = NULL;
    bi_new(&T0, 2 * n);
    bigint* T1 = NULL;
    bi_new(&T1, (2 * n) + 1);

    //Z = Z + T
    //과정에서 사용할 버퍼 변수 C 생성
    bigint* C = NULL;
    bi_set_zero(&C);
    bigint* T = NULL;

    word T0_MV[2] = { 0, };
    word T1_MV[2] = { 0, };

    for (cnt_i = 0; cnt_i < new_y->wordlen; cnt_i++) {
        for (cnt_j = 0; cnt_j < n; cnt_j++) {
            //T0,T1 중간값 -> 2word 
            bi_mul_zj(T0_MV, new_x->a[2 * cnt_j], new_y->a[cnt_i]);
            bi_mul_zj(T1_MV, new_x->a[(2 * cnt_j) + 1], new_y->a[cnt_i]);

            //T0 -> CONCAT하면서 값 생성
            T0->a[2 * cnt_j] = T0_MV[0];
            T0->a[(2 * cnt_j) + 1] = T0_MV[1];
            //T1 -> CONCAT하면서 값 생성
            T1->a[(2 * cnt_j) + 1] = T1_MV[0];
            T1->a[(2 * cnt_j) + 2] = T1_MV[1];
        }
        //ADDC
        //T <- T0 + T1
        bi_add(&T, T0, T1);
   
        //SHIFT
        //T << 32 * w
        size_t shift_len = WORD_BITS * cnt_i;
        bi_shift_left(&T, shift_len);
        //ADD
        //z <- z + T
        bi_add(z, C, T);
        bi_assign(&C, *z);
    }
    //메모리 누수 관리 & z refine
    bi_refine(*z);
    bi_delete(&new_x);
    bi_delete(&new_y);
    bi_delete(&C);
    bi_delete(&T);
    bi_delete(&T0);
    bi_delete(&T1);

    return SUCCESS;
}

//Modular Exponential by Montgomery Ladder
//z <- (x ^ n) mod M
int bi_montgomery_mod_exp(bigint** z, const bigint* x, const bigint* n, const bigint* M) {
    //아래 연산시작 전 예외처리3개 + n이 0일때 따로 처리하는 부분
    //순서 중요!

    //예외처리!
    //n은 양의 정수여야 함! 
    if (n->sign == NEGATIVE) {
        printf("Please reset the sign of n!\n");
        return FAIL;
    }

    //예외처리!
   //M은 양수 여야 -> 음수이거나 0은 안되!
    if (M->sign == NEGATIVE || bi_is_zero(M) == TRUE) {
        printf("sign of M is only NON_NEGATIVE and M can not setting zero!\n");
        return FAIL;
    }
       
    //n이 0일때는 따로 처리
    if (bi_is_zero(n) == TRUE) {
        //M이 1이라면 결과값은 0
        if (bi_is_one(M) == TRUE) {
            bi_set_zero(z);
            return SUCCESS;
        }
        else {//나머지의 경우는 결과값이 1
            bi_set_one(z);
            return SUCCESS;
        }
    }

    //예외처리
    //(00000..000) (......)....(.....) -> 이런식으로 n의 word 배열의 가장 큰 part에 0이 패딩되어 있는 경우는
    //n의 제로패딩된 부분을 없애서 인자로 넣어달라고 하는 부분 
    size_t msw_idx = n->wordlen - 1;
    if (n->a[msw_idx] == 0) {
        printf("please reset n!\n");
        return FAIL;
    }
    
    

    
    


   

    //---------------------Modular Exponential by montgomery ladder 시작--------------------------------
   
    //n의 워드배열 중 최상위 파트에서 어디서부터 1인지 찾아내는 부분
    //즉, n->a[n->wordlen-1]부분에서 어디서부터 비트가 1로 시작하는지를 알아내야
    //Modular Exponential by montgomery ladder 연산을 시작 할 수 있음
    uint64_t mp = WORD_BITS - 1;//n의 워드배열 중 최상위 파트에서 최초로 1이 나타나는 인덱스
    int i, j;
    for (i = WORD_BITS - 1; i >= 0; i--) {
        if (((n->a[n->wordlen - 1] >> i) & 0x01) == 0) {
            mp--;
        }
        else {
            break;
        }
    }

    //---------------------------------------------------------------------------------------------------
    //Modular Exponential by Montgomery Ladder 연산 시작

    //T0,T1 초기화
    bigint* T0 = NULL;
    bigint* T1 = NULL;
    bi_set_one(&T0);
    bi_assign(&T1, x);

    //T0,T1 중간 결과값 담아놓을 버퍼
    //이 값을 T0,T1에 넣어서 T0,T1 계속 UPDATE
    bigint* T = NULL;

    //bi_div()의 인자에 넣어줄 Q,R -> 이 중 R만 사용해서 MOD연산 진행
    bigint* Q = NULL;
    bigint* R = NULL;

    //n의 워드배열의 각 비트자리가 0 or 1?
    int ni = 0;

    //n->a[n->wordlen-1] 부분 처리 -> 워드배열 중 최상위 파트 처리
    for (; mp >= 0; mp--) {
        ni = (n->a[n->wordlen - 1] >> mp) & 0x01;
        if (ni == 0) {
            //T1 계산
            bi_mul(&T, T0, T1, "Karatsuba");
            bi_div(&Q, &R, T, M);
            bi_assign(&T1, R);

            //T0 계산
            bi_mul(&T, T0, T0, "Karatsuba");//나중에 SQUARING으로 교체할 예정
            bi_div(&Q, &R, T, M);
            bi_assign(&T0, R);
        }
        else {//ni==1
            //T0 계산
            bi_mul(&T, T0, T1, "Karatsuba");
            bi_div(&Q, &R, T, M);
            bi_assign(&T0, R);

            //T1 계산
            bi_mul(&T, T1, T1, "Karatsuba");//나중에 SQUARING으로 교체할 예정
            bi_div(&Q, &R, T, M);
            bi_assign(&T1, R);
        }
    }

    //n->a[n->wordlen-2] ~ n->a[0] 부분 처리
    for (i = n->wordlen - 2; i >= 0; i--) {
        for (j = WORD_BITS - 1; j >= 0; j--) {
            ni = (n->a[i] >> j) & 0x01;
            if (ni == 0) {
                //T1 계산
                bi_mul(&T, T0, T1, "Karatsuba");
                bi_div(&Q, &R, T, M);
                bi_assign(&T1, R);

                //T0 계산
                bi_mul(&T, T0, T0, "Karatsuba");//나중에 SQUARING으로 교체할 예정
                bi_div(&Q, &R, T, M);
                bi_assign(&T0, R);
            }
            else {//ni==1
                //T0 계산
                bi_mul(&T, T0, T1, "Karatsuba");
                bi_div(&Q, &R, T, M);
                bi_assign(&T0, R);

                //T1 계산
                bi_mul(&T, T1, T1, "Karatsuba");//나중에 SQUARING으로 교체할 예정
                bi_div(&Q, &R, T, M);
                bi_assign(&T1, R);
            }
        }
    }

    //결과값 처리
    bi_assign(z, T0);

    //메모리 누수 처리
    bi_delete(&T);
    bi_delete(&T0);
    bi_delete(&T1);
    bi_delete(&Q);
    bi_delete(&R);

    return SUCCESS;
}