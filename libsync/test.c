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
        bi_set_by_array(va_arg(bigints, c_bigint**), 0, arr, wordlen);
    }

    free(arr);
    va_end(bigints);
}

void free_bigint(const int count, ...) {
    int i;

    va_list bigints;
    va_start(bigints, count);

    for (i = 0; i < count; i++) {
        bi_delete(va_arg(bigints, c_bigint**));
    }

    va_end(bigints);
}

void test_add() {
    /* 
        Example
         - Bigint Addition
    */
    int i;
    c_bigint *z = NULL, *x = NULL, *y = NULL;

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
    c_bigint *z = NULL, *x = NULL, *y = NULL;

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
    c_bigint *z = NULL, *x = NULL, *y = NULL;

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
    c_bigint *z = NULL, *x = NULL, *y = NULL;

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
    c_bigint *z = NULL, *x = NULL, *y = NULL;

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
    c_bigint *r = NULL, *p = NULL;

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
    c_bigint *q = NULL, *r = NULL, *x = NULL, *y = NULL;

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

    free_bigint(4, &q, &r, &x, &y);
}

//z <- x^n mod m
//n이 너무 커지면 파이썬에서 돌릴 수 없음
//그래서 n을 연산을 시작하기 전에 적절한 값으로 지정하고 들어가줘야 함!
void test_modular_expo() {
    int i;
    c_bigint *z = NULL, *x = NULL, *n = NULL, *m = NULL;

    //n이 너무 커지면 파이썬에서
    //x**n을 계산할 수가 없기 때문에 n을 적절한 크기 수로 고정시키고 
    //테스트를 진행해야 함
    bi_new(&n, 1);
    n->a[0] = 85;

    START_PYTHON("bi_montgomery_mod_exp()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 2, &x, &m);
        

       
       bi_montgomery_mod_exp(&z, x, n, m);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(n);
        PRINT(m);
        PRINT(z);
        printf("if (x ** 85) % m == z :\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(4, &z, &x, &n, &m);
}

void test_text_squaring() {
    int i;
    c_bigint *z = NULL, *x = NULL;

    START_PYTHON("bi_squaring_text_zx()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 1, &x);
        

        
        bi_squaring_text_zx(&z, x);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(z);
        printf("if x ** 2 == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(2, &z, &x);
}

void test_karatsuba_squaring() {
    int i;
    c_bigint *z = NULL, *x = NULL;

    START_PYTHON("bi_squaring_karatsuba_zxy()...");
    for (i = 0; i < TEST_EPOCH; i++) {
        get_rand_bigint(BIGINT_SIZE, 1, &x);
        

        
        bi_squaring_karatsuba_zxy(&z, x, 2);

        /*
            Python Format
        */
        PRINT(x);
        PRINT(z);
        printf("if x ** 2 == z:\n\tret = ret + 1\n");
    }
    END_PYTHON("");

    free_bigint(2, &z, &x);
}

int main() {
    // test_add();
    //test_sub();
    //test_mul_textbook();
    //test_mul_improved_text();
    //test_mul_karatsuba();
    //test_div();

    //test_modular_expo(); //64에서 안돌아감
    //test_text_squaring(); //ok
    //test_karatsuba_squaring(); //ok

    // c_bigint *r = NULL;
    // get_rand_bigint(2, 1, &r);
    // r->a[0] = 0xffffffff;
    // r->a[1] = 0xffffffff;
    // bi_show_bin(r);
    // bi_shift_left(&r, 1);
    // bi_show_bin(r);


    return 0;
} 

