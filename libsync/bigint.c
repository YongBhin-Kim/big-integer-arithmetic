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
    if ( x->sign == NEGATIVE ) {
        printf("-");
    }
    for ( i=0; i<x->wordlen-1; i++ ) {
        printf("%08x:", x->a[i]);
    }
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
    for ( i=0; i<x->wordlen-1; i++ ) {
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