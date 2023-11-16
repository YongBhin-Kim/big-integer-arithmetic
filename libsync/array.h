#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

#include "config.h"

typedef uint8_t  byte;

#if   WORD ==  8
typedef uint8_t  word;
#define WORD_MAX    UINT8_MAX
#elif WORD == 16
#define WORD_MAX    UINT16_MAX
typedef uint16_t word;
#elif WORD == 64
#define WORD_MAX    UINT64_MAX
typedef uint64_t word;
#else /* Default Value = 32bits */
#define WORD_MAX    UINT32_MAX
typedef uint32_t word;
#endif

#define WORD_BITS  (sizeof(word) * 8)
#define WORD_BYTES (sizeof(word)    )

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifdef __cplusplus
extern "C" {
#endif

void array_init(word *a, const size_t wordlen);
void array_copy(word *dst, const word *src, const size_t wordlen);
void array_show_hex(const word *a, const size_t wordlen);
void array_rand(word *dst, const size_t wordlen);
void array_new_rand(word **const a, const size_t wordlen);

#ifdef __cplusplus
}
#endif
#endif /* ARRAY_H */
