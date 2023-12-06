#ifndef CONFIG_H
#define CONFIG_H

/**
 * @details 
 *  The bit level at which the big-integer-arithmetic operates
 *   Supports: 8bits, 16bits, 32bits(Default) and 64bits
*/

#define WORD      32

/**
 * @details
 *  The structure name of big-integer-arithmetic
*/

#define STRUCT_BIGINT     c_bigint

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
 *  The flag of Karatsuba multiplication
 *   Recommand: 128 in 8bits, 64 in 32bits and 64bits
*/

#define KARATSUBA  64


/**
 * @details
 *  Return Values
*/

#define SUCCESS  303
#define FAIL    -303 
#define TRUE     127
#define FALSE   -127


#endif /* CONFIG_H */


