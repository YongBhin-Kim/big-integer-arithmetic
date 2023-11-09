#include "bigint.h"

int string_is_hex(const char *str) {
    
    long long pos = -1;
    long long n = strlen(str);

    while ( ++pos != n ) {
        if ( ((str[pos] >= '0') && (str[pos] <= '9')) || ((str[pos] >= 'a') && ( str[pos] <= 'f' )) || ((str[pos] >= 'A') && ( str[pos] <= 'F' )) ) {
        }
        else return FALSE;
    }

    return TRUE;
}

int string_is_bin(const char *str) {
    
    long long pos = -1;
    long long n = strlen(str);

    while ( ++pos != n ) {
        if ( ((str[pos] >= '0') && (str[pos] <= '1')) ) {
        }
        else return FALSE;
    }

    return TRUE;
}

void bi_sign_flip(bigint *x) {
    // x->sign = (x->sign == NON_NEGATIVE) ? NEGATIVE : NON_NEGATIVE;
    x->sign ^= 1;
}

void bi_delete(bigint **x) { 

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

void bi_new(bigint **x, const size_t wordlen) {

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

void bi_set_by_array(bigint **x, const int sign, const word *a, const size_t wordlen) {

    // case address isn't allocated
    if ( a == NULL ) {
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

    int ret = FAIL;
    long long pos;
    size_t j;
    word value;
    int word_bits = sizeof(word) * 8;
    int exp; // 2^exp = base  1<<exp == base base>>exp = 1


    // 2진수
    if ( base == 2 ) {
        if ( string_is_bin(str) == TRUE ) {
            exp = 1;
            
            bi_new(x, (strlen(str)-1) / word_bits + 1); 
            (*x)->sign = sign;

            for ( pos=strlen(str)-1; pos>=0; pos-- ) {
                j = (strlen(str) - 1) - pos;
                value = (word)(str[pos] - '0');

                (*x)->a[j / (word_bits/1)] ^= value << ((j % (word_bits/1)) * 1);
            }
            bi_refine(*x); // case str = '0.......'
        }
        else { // string_is_bin(str) == FALSE
            printf("Fatal error: given string is not binary value. exit from program.\n");
            return FAIL;
        }
        return SUCCESS;
    }
    // 16진수
    else if ( base == 16 ) {
        if ( string_is_hex(str) == TRUE ) {
            exp = 4;

            bi_new(x, strlen(str)/(WORD_BYTES*2) + 1);
            (*x)->sign = sign;

            for ( pos=strlen(str)-1; pos>=0; pos-- ) {
                j = (strlen(str) - 1) - pos;
                if ( (str[pos] >= '0') && (str[pos] <= '9') ) {
                    value = (word)(str[pos] - '0');
                }
                else if ( (str[pos] >= 'a') && (str[pos] <= 'f') ) {
                    value = (word)(str[pos] - 'a') + 10;
                }
                else if ( (str[pos] >= 'A') && (str[pos] <= 'F') ) {
                    value = (word)(str[pos] - 'A') + 10;
                }
                (*x)->a[j / (word_bits/4)] ^= value << ((j % (word_bits/4)) * 4);
            }
            bi_refine(*x); // case str = '0.......'
        }
        else { // string_is_hex() == FALSE
            printf("Fatal error: given string is not hexadecimal value. exit from program.\n");
            return FAIL;
        }
        return SUCCESS;
    }

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

void bi_assign(bigint **y, const bigint *x) { // y <- x

    if ( *y != NULL ) {
        bi_delete(y);
    }

    bi_new(y, x->wordlen);
    (*y)->sign = x->sign;
    array_copy((*y)->a, x->a, x->wordlen);
}

void bi_gen_rand(bigint **x, const int sign, const size_t wordlen) {

    bi_new(x, wordlen);
    (*x)->sign = sign;
    array_rand((*x)->a, wordlen);

    bi_refine(*x);
}

void bi_set_one(bigint **x) {

    if ( *x != NULL ) {
        bi_delete(x);
    }

    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x1;
}

void bi_set_zero(bigint **x) {

    if ( *x != NULL ) {
        bi_delete(x);
    }
 
    bi_new(x, 1);
    (*x)->sign = NON_NEGATIVE;
    (*x)->a[0] = 0x0;
}

void bi_set_zero_not_refine(bigint **x) {
    memset((*x)->a, 0, WORD_BYTES * (*x)->wordlen);
}

int bi_is_zero(const bigint *x) {

    size_t j;
    int ret = FALSE;

    if ( (x->sign == NEGATIVE) || (x->a[0] != 0) ) {
        return ret;
    }

    for ( j=x->wordlen; j>=1; j-- ) {
        if ( x->a[j] != 0 ) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}

int bi_is_one(const bigint *x) {

    size_t j;
    int ret = FALSE;

    if ( (x->sign == NEGATIVE) || (x->a[0] != 1) ) {
        return ret;
    }

    for ( j=x->wordlen; j>=1; j-- ) {
        if ( x->a[j] != 0 ) {
            return ret;
        }
    }

    ret *= (-1);
    return ret;
}

// Compare two words
int word_compare(word *x, word *y, size_t x_wordlen, size_t y_wordlen) {
    int ret;
    if ( x_wordlen > y_wordlen )
        return 1;
    else if ( x_wordlen < y_wordlen )
        return -1;
    else {
        for ( int j=x_wordlen; j>=0; j-- ) {
            if ( x[j] > y[j] )
                return 1;
            else if ( x[j] < y[j] )
                return -1;
        }
        return 0;
    }
}

// compare(x, y) =  1 (x > y)
// compare(x, y) =  0 (x = y)
// compare(x, y) = -1 (x < y)
// or FAIL
int bi_compare(const bigint *x, const bigint *y) {

    int ret;

    // case exception 
    if ( (x == NULL) || (y == NULL) ) {
        return FAIL;
    }

    // x > y
    if ( (x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE) ) {
        return 1;
    }

    // x < y
    if ( (x->sign == NEGATIVE) && (y->sign == NON_NEGATIVE )) {
        return -1;
    }

    // (x, y > 0) or (x, y < 0)
    ret = bi_compare_abs(x, y);

    if ( x->sign == NON_NEGATIVE ) { // x, y > 0
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
int bi_compare_abs(const bigint *x, const bigint *y) {

    size_t x_wordlen, y_wordlen, j;
    x_wordlen = x->wordlen;
    y_wordlen = y->wordlen;

    if ( x_wordlen > y_wordlen ) { // x > y
        return 1;
    }
    else if ( x_wordlen < y_wordlen ) { // x < y
        return -1;
    }

    // case x_wordlen is same with y_wordlen

    for ( j=x_wordlen; j-->0; ) {
        if ( x->a[j] > y->a[j] ) { // x > y
            return 1;
        }
        else if ( x->a[j] < y->a[j] ) { // x < y
            return -1;
        }
    }
    return 0; // x == y
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
int bi_shift_right(bigint **x, int r) {

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

// bigint, reduction bits
int bi_reduction(bigint **x, size_t r) {

    size_t w, n;
    size_t k, j;
    int rp;
    w = (size_t)(sizeof(word) * 8); // 32
    n = (*x)->wordlen;
    // treating error case 
    if ( r < 0 ) {
        return -1;
    }
    // case r >= wn
    // A mod 2^r = A
    if ( r >= (w*n) ) {
        return 1;
    }
    // case r = wk where k < n
    else if ( (r % w) == 0 ) {
        k = r / w;
        for ( j=k; j<n; j++ ) {
            (*x)->a[j] = 0;
        }
        bi_refine(*x); // erase unused memory and modify wordlen of x
        return 1;
    }
    // case r = wk + r' where k < n and 0 < r' < w
    else {
        k = r / w;
        rp = r % w;
        (*x)->a[k] = (((*x)->a[k] << (w - rp))) >> (w - rp);
        for ( j=k+1; j<n; j++ ) {
            (*x)->a[j] = 0;
        }
        bi_refine(*x);
        return 1;
    }
    return -1;
}

// big integer addition


// word, word, current carry
// xj + yj + c mod W

// C <- xj + yj
// C <- C + c
int bi_add_zj(word *zj, word xj, word yj, int c) {
    int cp; // next carry

    *zj = xj + yj; // C <- xj + yj mod W
    cp = (*zj < xj); // is next carry exists?

    *zj = *zj + c;   // C <- C + carry
    cp = (*zj < c) + cp; // cp 

    return cp;
}

// z <- x + y
// wordlen(x) >= wordlen(y)
int bi_add_zxy(bigint **z, const bigint *x, const bigint *y) {
    // n >= m
    size_t n, m, j;
    int c;
    n = x->wordlen;
    m = y->wordlen; // n >= m

    bi_new(z, n+1);

    bigint *y_expand = NULL;  // copy of y
    if ( n == m ) {
        bi_assign(&y_expand, y);
    }
    else if ( n > m ) {
        // y_expand <- 0...0 || y, allocate new zero n-m+1-words
        bi_new(&y_expand, n);
        y_expand->sign = y->sign;
        array_copy(y_expand->a, y->a, m);
        for ( j=m; j<n; j++ ) {
            y_expand->a[j] = 0;
        }
    }
    c = 0; // set the first word carry bit zero
    
    for ( j=0; j<n; j++ ) {
        // c, Cj <- bi_add_xyc(xj, yj, c);
        c = bi_add_zj(&((*z)->a[j]), x->a[j], y_expand->a[j], c);
    }

    // last carry
    (*z)->a[n] = c; // 1 or 0 --> wordlen is n+1 or n
    bi_refine(*z);

    bi_delete(&y_expand);

    return 1;

}

// addition of two integers
// z <- x + y
// input: x, y \in Z
// output: z = x + y \in Z
int bi_add(bigint **z, const bigint *x, const bigint *y) {

    bigint *x_abs = NULL;
    bigint *y_abs = NULL;

    // case x = 0
    if ( bi_is_zero(x) == TRUE || x == NULL ) {
        bi_assign(z, y);
        return 1;
    }
    // case y = 0
    else if ( bi_is_zero(y) == TRUE || y == NULL ) {
        bi_assign(z, x);
        return 1;
    }
    // case x > 0 and y < 0
    else if ( (x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE) ) {
        // bi_sub(z, x, |y|);
        bi_assign(&y_abs, y);
        bi_sign_flip(y_abs);
        bi_sub(z, x, y_abs);
        bi_delete(&y_abs);
        return 1;
    }
    // case x < 0 and y > 0
    else if ( (x->sign == NEGATIVE) && (y->sign == NON_NEGATIVE) ) {
        // bi_sub(z, y, |x|);
        bi_assign(&x_abs, x);
        bi_sign_flip(x_abs);
        bi_sub(z, y, x_abs);
        bi_delete(&x_abs);
        return 1;
    }
    // case x > 0 and y > 0 or x < 0 and y < 0
    else if ( x->wordlen >= y->wordlen ) {
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

    int bp; // next borrow

    *zj = xj - b; // modular subtraction
    bp = (xj < b); // is next carry exists?
    bp = bp + (*zj < yj); // is next carry exists? (0 or 1)
    *zj = *zj - yj; // modular subtraction

    return bp;
}

// z <- x + y
// always x >= y > 0
int bi_sub_zxy(bigint **z, const bigint *x, const bigint *y) {

    // n >= m
    size_t n, m, j;
    int b;
    n = x->wordlen;
    m = y->wordlen; // n >= m
    bi_new(z, n);

    bigint *y_expand = NULL;  // copy of y
    if ( n == m ) {
        bi_assign(&y_expand, y);
    }
    else if ( n > m ) {
        // y_expand <- 0...0 || y, allocate new zero n-m+1-words
        bi_new(&y_expand, n);
        y_expand->sign = y->sign;
        array_copy(y_expand->a, y->a, m);
        for ( j=m; j<n; j++ ) {
            y_expand->a[j] = 0;
        }
    }

    b = 0; // set the first word borrow bit zero

    for ( j=0; j<n; j++ ) {
        // b <- bi_sub_zj(zj, xj, yj, b);
        b = bi_sub_zj(&((*z)->a[j]), x->a[j], y_expand->a[j], b);
    }

    bi_delete(&y_expand);
    bi_refine(*z);

    return 1;
}

// Subtraction of two integers
// z <- x - y
int bi_sub(bigint **z, const bigint *x, const bigint *y) {

    int cmp;
    bigint *x_temp = NULL;
    bigint *y_temp = NULL;

    // case x is zero
    if ( bi_is_zero(x) == TRUE || x == NULL ) {
        bi_assign(z, y);
        bi_sign_flip(*z);
        return SUCCESS;
    }
    else if ( bi_is_zero(y) == TRUE || y == NULL ) {
        bi_assign(z, x);
        return SUCCESS;
    }

    cmp = bi_compare(x, y);
    // x == y
    if ( cmp == 0 ) {
        bi_set_zero(z);
        return SUCCESS;
    }
    // x, y >= 0
    if ( (x->sign == NON_NEGATIVE) && (y->sign == NON_NEGATIVE) ) {
        // x >= y >= 0 : sub(x, y)
        if ( cmp == 1 ) {
            bi_sub_zxy(z, x, y);
        }
        // y > x >= 0 : -sub(y, x)
        else if ( cmp == -1 ) {
            bi_sub_zxy(z, y, x);
            (*z)->sign = NEGATIVE;
        }
        return SUCCESS;
    }
    // x, y < 0
    else if ( (x->sign == NEGATIVE) && (y->sign == NEGATIVE) ) {
        bi_assign(&x_temp, x);
        x_temp->sign = NON_NEGATIVE;
        bi_assign(&y_temp, y);
        y_temp->sign = NON_NEGATIVE;

        // 0 > x >= y : sub(|y|, |x|)
        if ( cmp == 1 ) {
            bi_sub_zxy(z, y_temp, x_temp);
        }
        // 0 > y > x : -sub(|x|, |y|)
        else if ( cmp == -1 ) {
            bi_sub_zxy(z, x_temp, y_temp);
            (*z)->sign = NEGATIVE;
        }
        
        bi_delete(&x_temp);
        bi_delete(&y_temp);
        return SUCCESS;
    }
    // x > 0 and y < 0 : add(x, |y|)
    else if ( (x->sign == NON_NEGATIVE) && (y->sign == NEGATIVE) ) {
        bi_assign(&y_temp, y);
        y_temp->sign = NON_NEGATIVE;

        bi_add(z, x, y_temp);

        // bi_delete(&x_temp);
        bi_delete(&y_temp);
        return SUCCESS;
    }
    // x < 0 and y > 0 : -add(|x|, y)
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

// Multiplication of two integers
// z <- x * y
int bi_mul(bigint **z, const bigint *x, const bigint *y, const char *str) {

    // Case x or y is zero
    if ( bi_is_zero(x) == TRUE || bi_is_zero(y) == TRUE ) {
        bi_set_zero(z);
        return SUCCESS;
    }

    // Case x is one
    if ( bi_is_one(x) == TRUE ) {
        bi_assign(z, y); // return y
        return SUCCESS;
    }
    // Case y is one
    if ( bi_is_one(y) == TRUE ) {
        bi_assign(z, x); // return x
        return SUCCESS;
    }
    // Case x is minus one
    bigint *temp = NULL;
    bi_assign(&temp, x);
    bi_sign_flip(temp);
    if ( bi_is_one(temp) == TRUE ) {
        bi_assign(z, y); // return -y
        bi_sign_flip(*z);
        bi_delete(&temp);
        return SUCCESS;
    }
    // Case y is minus one
    bi_assign(&temp, y);
    bi_sign_flip(temp);
    if ( bi_is_one(temp) == TRUE ) {
        bi_assign(z, x); // return -x
        bi_sign_flip(*z);
        bi_delete(&temp);
        return SUCCESS;
    }
    bi_delete(&temp);

    // Case else
    // z <- x * y

    bigint *x_ = NULL, *y_ = NULL;
    bi_assign(&x_, x);
    bi_assign(&y_, y);
    x_->sign = NON_NEGATIVE;
    y_->sign = NON_NEGATIVE;

    if ( !strcmp("Textbook", str) ) {
        if ( bi_mul_text_zxy(z, x_, y_) == FAIL ) {
            bi_delete(&x_);
            bi_delete(&y_);
            return FAIL;
        }
    }
    else if ( !strcmp("Improved Text", str) ) {
        // bi_mul_itext_zxy(z, x, y);
    }
    else if ( !strcmp("Karatsuba", str) ) {
        if ( bi_mul_karatsuba_zxy(z, x_, y_, 3) == FAIL ) {
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

// alg. 12. single word mul
// z \in [0, W^2) : 1 word or 2 words
int bi_mul_zj(word *zj, word xj, word yj) {
    word a1, a0, b1, b0, t1, t0, c1, c0, t;

    a1 = xj >> (WORD_BITS/2); // a1 = xj >> W/2 (line 2)
    a0 = xj & ((word) ((1 << (WORD_BITS/2)) - 1)); // 2^{wordbits-1} - 1 (line 2)
    b1 = yj >> (WORD_BITS/2); // (line 3)
    b0 = yj & ((word) ((1 << (WORD_BITS/2)) - 1)); // 2^{wordbits-1} - 1 (line 3)

    t1 = a1 * b0; t0 = a0 * b1; // t1 <- a1b0, t0 <- a0b1 (line 4)
    t0 = t1 + t0; // modular addition
    t1 = t0 < t1; // word compare
    c1 = a1 * b1;
    c0 = a0 * b0; // line 7

    t = c0;
    c0 = c0 + (t0 << (WORD_BITS/2));
    zj[0] = c0;
    zj[1] = c1 + (t1 << (WORD_BITS/2)) + (t0 >> (WORD_BITS/2)) + (c0 < t);

    return SUCCESS;
}

// Textbook Multiplication core code
// bigint z <- bigint x * bigint y
int bi_mul_text_zxy(bigint **z, const bigint *x, const bigint *y) {

    if ( x->sign == NEGATIVE || y->sign == NEGATIVE ) {
        printf("Fatal Error: Textbook multiplication input value should be NON NEGATIVE\n");
        return FAIL;
    }

    size_t n, m, i, j;
    n = x->wordlen;
    m = y->wordlen;
    bigint *c = NULL, *t = NULL;
    bi_new(&c, n+m); // consider 5.1. Memory for mul
    bi_new(&t, 2);

    for ( j=0; j<n; j++ ) {
        for ( i=0; i<m; i++ ) {
            bi_mul_zj(t->a, x->a[j], y->a[i]); // t[0~1] <- x_j * y_i // single word multiplication
            bi_shift_left(&t, WORD_BITS*(i+j)); // t <- t << w(i+j);
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

// Karatsuba Multiplication core code
// z <- |x| * |y|
int bi_mul_karatsuba_zxy(bigint **z, const bigint *x, const bigint *y, size_t flag) {
    
    if ( x->sign == NEGATIVE || y->sign == NEGATIVE ) {
        printf("Fatal Error: Karatsuba multiplication input value should be NON NEGATIVE\n");
        return FAIL;
    }

    size_t n, m, l, lw;
    bigint *a1 = NULL, *a0 = NULL, *b1 = NULL, *b0 = NULL, *t1 = NULL, *t0 = NULL, *c = NULL, *c_ = NULL, *s_ = NULL, *s = NULL, *s1 = NULL, *s0 = NULL;
    bigint *modulo = NULL, *a0_ = NULL, *b0_ = NULL;
    n = x->wordlen;
    m = y->wordlen;

    if ( flag >= MIN(n, m) ) { // flag = 2
        return bi_mul_text_zxy(z, x, y);
    }

    // l = max(n, m) / 2, lw = bits of l-words
    l = (size_t) ((MAX(n, m) + 1) >> 1);
    lw = l * WORD_BITS; 

    // a1 = a >> lw;
    bi_assign(&a1, x);
    bi_shift_right(&a1, lw);

    // modulo = 2^lw
    bi_set_min_words(&modulo, NON_NEGATIVE, l+1);

    // a0 = (a mod 2^lw) or (a + 2^lw mod 2^lw)
    bi_set_by_array(&a0, x->sign, x->a, l); // NON_NEGATIVE
    bi_refine(a0);

    // b1 = b >> lw
    bi_assign(&b1, y);
    bi_shift_right(&b1, lw);
    
    // b0 = (b mod 2^lw) or (b + 2^lw mod 2^lw)
    // if ( y->sign == NON_NEGATIVE )
        bi_set_by_array(&b0, y->sign, y->a, l); // NON_NEGATIVE
    // else {
    //     bi_set_by_array(&b0_, y->sign, y->a, l); // NEGATIVE
    //     bi_add(&b0, b0_, modulo); // b + 2^lw
    // }
    
    // t1, t0 = mul(a1, b1), mul(a0, b0)
    bi_mul_karatsuba_zxy(&t1, a1, b1, flag);
    bi_mul_karatsuba_zxy(&t0, a0, b0, flag);
    
    // c = (t1 << 2lw) + t0
    bi_assign(&c_, t1);
    bi_shift_left(&c_, 2*lw);
    bi_add(&c, c_, t0);
    bi_assign(z, c);
    
    // s1, s0 = sub(a0, a1), sub(b1, b0)
    bi_sub(&s1, a0, a1);
    bi_sub(&s0, b1, b0);
    
    // s = (-1)^{sign of s1 ^ sign of s2} * mul(|s1|, |s0|)
    int sign = s1->sign ^ s0->sign;
    bi_new(&s_, 1);

    s1->sign = NON_NEGATIVE;
    s0->sign = NON_NEGATIVE;
    bi_mul_karatsuba_zxy(&s_, s1, s0, flag);
    s_->sign = sign;

    // s = add(s, t1), s = add(s, t0)
    bi_add(&s, s_, t1);
    bi_assign(&s_, s);
    bi_add(&s, s_, t0);

    // s = s << lw
    bi_shift_left(&s, lw);

    // z = add(c, s)
    bi_add(z, c, s);

    // free bigintegers
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