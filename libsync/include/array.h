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
#define HEX_FORMAT "%02x"
#define DEC_FORMAT "%d"
#define WORD_MAX    UINT8_MAX
#define WORD_MASK 0xFF
#define MSB_MASK 0x80
#elif WORD == 16
#define HEX_FORMAT "%04x"
#define DEC_FORMAT "%d"
#define WORD_MAX    UINT16_MAX
typedef uint16_t word;
#define WORD_MASK 0xFFFF
#define MSB_MASK 0x8000
#elif WORD == 64
#define HEX_FORMAT "%016llx"
#define DEC_FORMAT "%lld"
#define WORD_MAX    UINT64_MAX
#define WORD_MASK 0xFFFFFFFFFFFFFFFF
#define MSB_MASK 0x8000000000000000
typedef uint64_t word;
#else /* Default Value = 32bits */
#define HEX_FORMAT "%08x"
#define DEC_FORMAT "%d"
#define WORD_MAX    UINT32_MAX
typedef uint32_t word;
#define WORD_MASK 0xFFFFFFFF
#define MSB_MASK 0x80000000
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
