#include "bigint.h"


int main() {

    bigint *x = NULL;
    bigint *y = NULL;
    bigint *z = NULL;

    word *a = NULL;

    // test : gen new array and show array
    array_new_rand(&a, 5);
    array_show_hex(a, 5);

    // test : bigint <- array
    bi_set_by_array(&x, NEGATIVE, a, 5);
    bi_show_hex(x);

    // test : gen rand bigint
    bi_gen_rand(&y, NEGATIVE, 10);
    bi_show_hex(y);

    // test : bigint <- bigint assign
    bi_assign(&z, y);
    bi_show_hex(z);

    // test : print binary 
    bi_show_bin(z);


    return 0;
}