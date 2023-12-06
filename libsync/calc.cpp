#include <iostream>

#include "libsync_api.h"

int main(void) {
    bigint::set_mul_algo("Karatsuba");

    std::cout << "Big-Integer Calculator with BBQ libsync\n" 
              << " 1. Addition\n" 
              << " 2. Subtraction\n" 
              << " 3. Multiplication\n" 
              << " 4. Division\n" 
              << " 5. Modulus\n"
              << " Enter a number: "; 

    bigint x, y, z, q, r;
    int n;
    std::cin >> n;

    std::cout << "Enter a big-integer in hex format\n" 
              << " ex, 0xf1ae765d0a31ecc4\n";
    std::cout << " X = "; std::cin >> x;
    std::cout << " Y = "; std::cin >> y;

    switch (n) {
        case 1:
            std::cout << " X + Y = " << x + y;
            break;
        case 2:
            std::cout << " X - Y = " << x - y;
            break;
        case 3:
            std::cout << " X * Y = " << x * y;
            break;
        case 4:
            std::cout << " X / Y = " << x / y;
            break;
        case 5:
            std::cout << " X % Y = " << x % y;
            break;
        default:
            break;
    }

    return 0;
}