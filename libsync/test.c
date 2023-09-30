#include "bigint.h"


int main() {

    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;

    word *a = NULL;

    // test : gen new array and show array (yb)
    array_new_rand(&a, 5);
    printf("a = "); array_show_hex(a, 5);
    printf("----------\n");

    // test : bigint <- array (yb)
    bi_set_by_array(&x, NEGATIVE, a, 5);
    printf("x = "); bi_show_hex(x);
    printf("----------\n");

    // test : gen rand bigint (yb)
    bi_gen_rand(&y, NEGATIVE, 10);
    printf("y = "); bi_show_hex(y);
    printf("----------\n");

    // test : bigint <- bigint assign (yb)
    bi_assign(&z, y);
    printf("z = "); bi_show_hex(z);
    printf("----------\n");

    // test : print binary (yb)
    printf("z = "); bi_show_bin(z);

    // test : isone, iszero (yb)
    bi_set_zero(&x);
    printf("x is zero? %d\n", (bi_is_zero(&x) == TRUE));
    bi_set_one(&x);
    printf("x is one? %d\n", (bi_is_one(&x) == TRUE));
    printf("----------\n");

    // test : compare (yb)
    y->sign = NON_NEGATIVE;
    z->sign = NON_NEGATIVE;
    
    printf("x = "); bi_show_hex(x);
    printf("y = "); bi_show_hex(y);
    printf("z = "); bi_show_hex(z);
    printf("x VS y = %d\n", bi_compare(&x, &y)); // x > y (1)
    printf("y VS z = %d\n", bi_compare(&y, &z)); // x = y (0)
    printf("----------\n");

    // test : bi_shift (yb)
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


    printf("----------\n");

    bi_delete(&x);
    bi_delete(&y);
    bi_delete(&z);

    return 0;
} 