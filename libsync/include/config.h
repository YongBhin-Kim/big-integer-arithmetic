#ifndef CONFIG_H
#define CONFIG_H

/**
 * @details 
 *  The bit level at which the big-integer-arithmetic operates
 *   Supports: 8bits, 16bits, 32bits(Default) and 64bits
*/

#define WORD      8
// #define WORD      16
// #define WORD      32
// #define WORD     1 64

/**
 * @details
 *  The structure name of big-integer-arithmetic
*/

#define STRUCT_BIGINT     c_bigint

/**
 * @details
 *  The flag of Karatsuba multiplication
 *   set the appropriate value according to the word size of the frequently used big-integer.
*/

#define KARATSUBA  12

/**
 * @details 
 *  Initializes the value to zero when memory is released.
 *   0 : deactivate
 *   1 : activate
*/

#define ZERORIZE   1

/**
 * @details
 *  Prints the process of the operation in your terminal
 *   0 : deactivate
 *   1 : activate
*/

#define DEBUG_OUT  0

/**
 * @details
 *  Return Values
*/

#define SUCCESS  303
#define FAIL    -303 
#define TRUE     127
#define FALSE   -127


#endif /* CONFIG_H */


