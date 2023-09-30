#include "bigint.h"


void bi_delete(bigint **const x) { 

    if ( *x == NULL ) {
        return;
    }

#ifdef ZERORIZE
    array_init((*x)->a, (*x)->wordlen);
#endif

    free((*x)->a);
    free(*x);
    *x = NULL;
}

void bi_new(bigint **const x, const size_t wordlen) {

    if ( *x != NULL ) {
        bi_delete(x);
    }

    *x = (bigint *)malloc(sizeof(bigint));
    (*x)->sign   = NON_NEGATIVE;
    (*x)->wordlen = wordlen;
    (*x)->a = (word *)calloc(wordlen, sizeof(word)); // [word] [word] .. : wordlen개
}

void bi_show_hex(const bigint *x) {

    int i;

    if ( x->wordlen == 0 ) {
        return;
    }

    printf(" (hex) ");

    // print sign of the bigint 'x'
    if ( x->sign == NEGATIVE ) {
        printf("-");
    }

    // print words except last word
    for ( i=x->wordlen; i-->1; ) {
        printf("%08x:", x->a[i]);
    }

    // print last word
    printf("%08x\n",x->a[i]); 
}

void bi_show_dec(const bigint *x);

void bi_show_bin(const bigint *x) {

    size_t i;
    int j;
    int word_bits = sizeof(word) * 8;

    if ( x->wordlen == 0 ) {
        return;
    }

    printf(" (bin) ");
    if ( x->sign == NEGATIVE ) {
        printf("-");
    }
    word *p = x->a;

    for ( i=x->wordlen; i-->1; ) {
        for ( j=word_bits-1; j>=0; j-- ) {
            printf("%d", (p[i] >> j) & 0x1);
        }
        printf(":");
    }
    for ( j=word_bits-1; j>=0; j-- ) {
        printf("%d", (p[i] >> j) & 0x1);
    }
    printf("\n");
}

void bi_set_by_array(bigint **const x, const int sign, const word *a, const size_t wordlen) {

    // case address isn't allocated
    if ( a == NULL ) {
        return;
    }

    bi_new(x, wordlen);
    (*x)->sign = sign;

    array_copy((*x)->a, a, wordlen);
}

int bi_set_by_string(bigint **const x, const int sign, const char *str, const int base) {

    int ret = FAIL;
    // 2진수

    // 16진수

    // 10진수

    return ret;

    
}

void bi_refine(bigint *x) {

    if ( x == NULL ) {
        return;
    }

    size_t new_wordlen = x->wordlen;
    while ( new_wordlen > 1 ) { // at least one word needed
        if ( x->a[new_wordlen - 1] != 0 ) {
            break;
        }
        new_wordlen--;
    }
    if ( x->wordlen != new_wordlen ) {
        x->wordlen = new_wordlen;
        x->a = (word *)realloc(x->a, sizeof(word)*new_wordlen);
    }

    if ( (x->wordlen == 1) && (x->a[0] == 0x0) ) {
        x->sign = NON_NEGATIVE;
    }
}

void bi_assign(bigint **const y, const bigint *x) { // y <- x

    if ( *y != NULL ) {
        bi_delete(y);
    }

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    array_copy((*y)->a, x->a, x->wordlen);
}

void bi_gen_rand(bigint **const x, const int sign, const size_t wordlen) {

    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

void bi_set_one(bigint **const x) {

    if ( *x != NULL ) {
        bi_delete(x);
    }

    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

void bi_set_zero(bigint **const x) {

    if ( *x != NULL ) {
        bi_delete(x);
    }
 
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

int bi_is_zero(bigint **const x) {

    size_t j;
    int ret = FALSE;

    if ( ((*x)->sign == NEGATIVE) || ((*x)->a[0] != 0) ) {
        return ret;
    }

    for ( j=(*x)->wordlen; j>=1; j-- ) {
        if ( (*x)->a[j] != 0 ) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}

int bi_is_one(bigint **const x) {

    size_t j;
    int ret = FALSE;

    if ( ((*x)->sign == NEGATIVE) || ((*x)->a[0] != 1) ) {
        return ret;
    }

    for ( j=(*x)->wordlen; j>=1; j-- ) {
        if ( (*x)->a[j] != 0 ) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}

// compare(x, y) =  1 (x > y)
// compare(x, y) =  0 (x = y)
// compare(x, y) = -1 (x < y)
// or FAIL
int bi_compare(bigint **const x, bigint **const y) {

    int ret;

    // case exception 
    if ( (x == NULL) || (y == NULL) ) {
        return FAIL;
    }

    // x > y
    if ( ((*x)->sign == NON_NEGATIVE) && ((*y)->sign == NEGATIVE) ) {
        return 1;
    }

    // x < y
    if ( ((*x)->sign == NEGATIVE) && ((*y)->sign == NON_NEGATIVE )) {
        return -1;
    }

    // (x, y > 0) or (x, y < 0)
    ret = bi_compare_abs(x, y);

    if ( (*x)->sign == NON_NEGATIVE ) { // x, y > 0
        return ret; // 1, 0, -1
    }
    else {
        return ret * (-1); // -1, 0, 1
    }

    // case exception
    return FAIL;
}

// compareabs(x, y) =  1 (x > y)
// compareabs(x, y) = -1 (x < y)
// compareabs(x, y) =  0 (x = y)
// or FAIL
int bi_compare_abs(bigint **const x, bigint **const y) {

    size_t x_wordlen, y_wordlen, j;
    x_wordlen = (*x)->wordlen;
    y_wordlen = (*y)->wordlen;

    if ( x_wordlen > y_wordlen ) { // x > y
        return 1;
    }
    else if ( x_wordlen < y_wordlen ) { // x < y
        return -1;
    }

    // case x_wordlen is same with y_wordlen

    for ( j=x_wordlen; j-->0; ) {
        if ( (*x)->a[j] > (*y)->a[j] ) { // x > y
            return 1;
        }
        else if ( (*x)->a[j] < (*y)->a[j] ) { // x < y
            return -1;
        }
    }
    return 0; // x == y
}

int bi_shift_left(bigint **const x, size_t r) {

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
        bi_new(&y, n + k + 1);
        y->sign = (*x)->sign;

        j = k;
        y->a[j] = ((*x)->a[j-k] << (32-rp)) & 0xffffffff;
        j++;
        for ( ; j<n+k+1; j++ ) {
            y->a[j] = ((((*x)->a[j-k] << rp)) & 0xffffffff) | ((*x)->a[j-k-1] >> (32-rp));
        }

        // x <- y
        bi_assign(x, y);
        bi_delete(&y);

        return 1;
    }

    return -1;
}

// r: shift bits
int bi_shift_right(bigint **const x, int r) {

    if ( r < 0 ) {
        bi_shift_left(x, (-1)*r);
        return 1;
    }
    else if ( r == 0 ) {
        return 1;
    }

    size_t n = (*x)->wordlen; // wordlen
    size_t w = (size_t)(sizeof(word) * 8); // word bits (= 32)
    size_t k, j;
    int rp;

    // case r >= wn 
    if ( r >= (w*n) ) {
        bi_set_zero(x);
        return 1; 
    }
    // case r == wk 
    /*
        w = wn-1 ... wk wk-1 ... w0 (n개)
    ->  w = wn-1 ... wk 
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
    else if ( r%w == 0 ) {
        k = r/w;

        for ( j=0; j<n-k; j++ ) {
            (*x)->a[j] = (*x)->a[j+k];
        }
        for ( ; j<n; j++ ) {
            (*x)->a[j] = 0;
        }
#if 0
        printf("before refine, x = "); bi_show_hex(*x);
#endif
        bi_refine(*x); // 함수 안에서 wordlen = n-k 까지 처리
#if 0
        printf("before refine, x = "); bi_show_hex(*x);
#endif
        return 1;
    }
    // case r = wk + r' where 0 < r' < w
    else {
        k = r/w;
        rp = r%w;
#if 0
        printf(" k = %d, rp = %d\n", k, rp);
#endif
        for ( j=0; j<n-k-1; j++ ) {
            (*x)->a[j] = (((*x)->a[j+k+1] << (32-rp)) & 0xffffffff) | ((*x)->a[j+k] >> rp);
        }
        (*x)->a[j] = ((*x)->a[j+k] >> rp);
        j++;
        for ( ; j<n; j++ ) {
            (*x)->a[j] = 0x0000000;
        }
        bi_refine(*x);

        return 1;
    }

    // exception
    return -1;
}