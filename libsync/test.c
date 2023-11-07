// 23.10.13 14:01 
// C에서 python양식으로 Test vector용 함수

#include "bigint.h"

// march oview open source

// 1. rng drbg 추가 및 의존성 제거 (AES_CTR_DRBG)
// 2. 

int main() {

    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;

    word *a = NULL;

    // Test : gen new array and show array
    printf("=============================================\n");
    printf("Test : gen new array and show array\n");
    printf("---------------------------------------------\n");
    array_new_rand(&a, 5);
    printf("a = "); array_show_hex(a, 5);
    printf("=============================================\n\n\n");

    // Test : bigint <- array
    printf("=============================================\n");
    printf("Test : bigint <- array\n");
    printf("---------------------------------------------\n");
    bi_set_by_array(&x, NEGATIVE, a, 5);
    printf("x = "); bi_show_hex(x);
    printf("=============================================\n\n\n");

    // Test : gen rand bigint
    printf("=============================================\n");
    printf("Test : gen rand bigint\n");
    printf("---------------------------------------------\n");
    bi_gen_rand(&y, NEGATIVE, 10);
    printf("y = "); bi_show_hex(y);
    printf("=============================================\n\n\n");

    // Test : bigint <- bigint assign
    printf("=============================================\n");
    printf("Test : bigint <- bigint assign\n");
    printf("---------------------------------------------\n");
    bi_assign(&z, y);
    printf("z = "); bi_show_hex(z);
    printf("=============================================\n\n\n");

    // Test : print binary
    printf("=============================================\n");
    printf("Test : print binary\n");
    printf("---------------------------------------------\n");
    printf("z = "); bi_show_bin(z);
    printf("=============================================\n\n\n");

    // Test : isone, iszero
    printf("=============================================\n");
    printf("Test : isone, iszero\n");
    printf("---------------------------------------------\n");
    bi_set_zero(&x);
    printf("x is zero? %d\n", (bi_is_zero(&x) == TRUE));
    bi_set_one(&x);
    printf("x is one? %d\n", (bi_is_one(&x) == TRUE));
    printf("=============================================\n\n\n");

    // Test : compare
    printf("=============================================\n");
    printf("Test : compare\n");
    printf("---------------------------------------------\n");
    y->sign = NON_NEGATIVE;
    z->sign = NON_NEGATIVE;
    
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    printf("z = "); bi_show_hex(z);
    printf("x VS y = %d\n", bi_compare(&x, &y)); // x > y (1)
    printf("y VS z = %d\n", bi_compare(&y, &z)); // x = y (0)
    printf("=============================================\n\n\n");

    // Test : bi_shift
    printf("=============================================\n");
    printf("Test : bi_shift\n");
    printf("---------------------------------------------\n");
    printf("z = "); bi_show_hex(z);
    bi_shift_right(&z, 64);
    printf(" >>= 64 = "); bi_show_hex(z);

    bi_shift_right(&z, 4);
    printf(" >>= 4 = "); bi_show_hex(z);

    bi_shift_right(&z, 500);
    printf(" >>= 500 = "); bi_show_hex(z);

    bi_gen_rand(&z, NEGATIVE, 5);

    printf("z = "); bi_show_hex(z);
    bi_shift_left(&z, 64);
    printf(" <<= 64 = "); bi_show_hex(z);

    bi_shift_left(&z, 8);
    printf(" <<= 8 = "); bi_show_hex(z);

    bi_shift_left(&z, 200);
    printf(" <<= 200 = "); bi_show_hex(z);

    printf("=============================================\n\n\n");

    // Test : bi_reduction
    printf("=============================================\n");
    printf("Test : bi_reduction\n");
    printf("---------------------------------------------\n");
    printf("z = "); bi_show_hex(z);
    if ( bi_reduction(&z, 400) == 1 ) {
        printf(" mod 2^400 = "); bi_show_hex(z);
    }
    printf("=============================================\n\n\n");

    // Test : bi_add
    printf("=============================================\n");
    printf("Test : bi_add\n");
    printf("---------------------------------------------\n");
    bi_gen_rand(&y, NON_NEGATIVE, 10);
    bi_gen_rand(&x, NEGATIVE, 10);
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);

    bi_add(&z, x, y);
    printf("z = x + y = "); bi_show_hex(z);
    
    printf("=============================================\n\n\n");


    // Test : bi_sub
    printf("=============================================\n");
    printf("Test : bi_sub\n");
    printf("---------------------------------------------\n");
    printf("z = "); bi_show_hex(z);
    printf("x = "); bi_show_hex(x);

    bi_sub(&x, z, y);
    printf("x = z - y = "); bi_show_hex(x);

    printf("=============================================\n\n\n");

    // Test : bi_set_by_string
    printf("=============================================\n");
    printf("Test : bi_set_by_string\n");
    printf("---------------------------------------------\n");
    const char *str_hex = "aabbc0aaca9";
    printf("str_hex = %s\n", str_hex);
    bi_set_by_string(&x, NEGATIVE, str_hex, 16);
    printf("x = "); bi_show_hex(x);

    const char *str_bin = "01000111000110100011111111101100111111011101";
    printf("str_bin = %s\n", str_bin);
    bi_set_by_string(&x, NEGATIVE, str_bin, 2);
    printf("x = "); bi_show_hex(x);

    printf("=============================================\n\n\n");

    // Test : bi_mul
    printf("=============================================\n");
    printf("Test : bi_mul\n");
    printf("---------------------------------------------\n");
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    printf("Test : bi_mul_zxy (Single Mul)\n");
    word t[2];
    bi_mul_zj(t, x->a[0], y->a[0]);
    printf("t = x->a[0] * y->a[0] = "); array_show_hex(t, 2);
    printf("---------------------------------------------\n");

    printf("Test : bi_mul_textbook (Textbook Mul)\n");
    bi_mul(&z, x, y);
    printf("z = x * y = "); bi_show_hex(z);
    printf("---------------------------------------------\n");

    


    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);

    return 0;
} 