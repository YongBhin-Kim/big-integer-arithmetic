#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#include "bigint.h"

#define BIGINT_SIZE       7
#define TEST_EPOCH     1000

#define PRINT(x)        { printf(#x " = "); bi_show_hex(x); }
#define START_PYTHON(x) { printf("print("");print(\"%s\")\n", (x)); printf("ret = 0\n"); }
#define END_PYTHON(x)   { printf("if ret == %d:\n\tprint(\"SUCCESS.\")\nprint(\"%s\")\nprint(ret)\n", TEST_EPOCH, (x)); }

long get_ms() {
    struct timespec t_val;
    clock_gettime( CLOCK_MONOTONIC, &t_val );
    return ( t_val.tv_sec * 1000 + t_val.tv_nsec / 1000 / 1000 );
}

void get_rand_bytes(unsigned char *arr, const size_t len) {
    int i;

    for (i = 0; i < len; i++) {
        arr[i] = (rand() % 0xfe) + 1;     // replace rand() with "kisa-ariacbc" drbg
    }
}

void get_rand_bigint(const int wordlen, const int count,...) {
    int i;
    word *arr = (word *)malloc(wordlen * WORD_BYTES);

    va_list bigints;
    va_start(bigints, count);

    for (i = 0; i < count; i++) {
        get_rand_bytes((unsigned char *)arr, wordlen * WORD_BYTES);
        bi_set_by_array(va_arg(bigints, bigint**), 0, arr, wordlen);
    }

    free(arr);
    va_end(bigints);
}

void free_bigint(const int count, ...) {
    int i;

    va_list bigints;
    va_start(bigints, count);

    for (i = 0; i < count; i++) {
        bi_delete(va_arg(bigints, bigint**));
    }

    va_end(bigints);
}

void test_add() {
    /* 
        Example
         - Bigint Addition
    */
    int i;
    bigint *z = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_add()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        
        bi_add(&z, x, y);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(z);
        printf("if x + y == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(3, &z, &x, &y);
}

void test_sub() {
    int i;
    bigint *z = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_sub()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        
        bi_sub(&z, x, y);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(z);
        printf("if x - y == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(3, &z, &x, &y);
}

void test_mul_textbook() {
    int i;
    bigint *z = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_mul() - Textbook...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        
        bi_mul(&z, x, y, "Textbook");

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(z);
        printf("if x * y == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(3, &z, &x, &y);
}

void test_mul_improved_text() {
    int i;
    bigint *z = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_mul() - Improved Text...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        
        bi_mul(&z, x, y, "Improved Text");

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(z);
        printf("if x * y == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(3, &z, &x, &y);
}

void test_mul_karatsuba() {
    int i;
    bigint *z = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_mul() - Karatsuba...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        
        bi_refine(x);
        bi_refine(y);

        bi_mul(&z, x, y, "Karatsuba");

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(z);
        printf("if x * y == z:\n\tret = ret + 1\n");
        printf("else:\n\tprint(x);print(y);print(z)\n");
    }
    END_PYTHON("");

    free_bigint(3, &z, &x, &y);
}

void test_left() {
    int i;
    size_t a;
    bigint *r = NULL, *p = NULL;

    START_PYTHON("bi_left()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        a = rand()%2;


        get_rand_bigint(BIGINT_SIZE, 1, &r);
        bi_assign(&p, r);

        bi_shift_left(&r, a);

        PRINT(p);
        PRINT(r);

    }
    END_PYTHON("");
    free_bigint(1, &r);
    free_bigint(1, &p);
}

void test_div() {
    int i;
    bigint *q = NULL, *r = NULL, *x = NULL, *y = NULL;

    START_PYTHON("bi_div()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &y);
        

        bi_refine(x);
        bi_refine(y);
        bi_div(&q, &r, x, y);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(y);
        PRINT(q);
        PRINT(r);
        printf("if x // y == q and x %% y == r:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(3, &q, &r, &x, &y);
}

int main() {
    test_add();
    test_sub();
    test_mul_textbook();
    test_mul_improved_text();
    test_mul_karatsuba();
    test_div();

    // bigint *r = NULL;
    // get_rand_bigint(2, 1, &r);
    // r->a[0] = 0xffffffff;
    // r->a[1] = 0xffffffff;
    // bi_show_bin(r);
    // bi_shift_left(&r, 1);
    // bi_show_bin(r);


    return 0;
} 
