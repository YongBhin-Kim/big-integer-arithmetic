#include "array.h"

void array_init(word *a, const size_t wordlen) {

    if ( (wordlen == 0) || (a == NULL) ) {
        return;
    }

    memset(a, 0, wordlen * sizeof(word));
}

void array_copy(word *dst, const word *src, const size_t wordlen) {

    if ( (wordlen == 0) || (src == NULL) || (dst == NULL) ) {
        return;
    }

    memcpy(dst, src, wordlen * sizeof(word));
}

void array_show_hex(const word *a, const size_t wordlen) {

    int i;
    for ( i=0; i<wordlen-1; i++ ) {
        printf("%08x:", a[i]);
    }

    printf("%08x\n", a[i]);
}

void array_rand(word *dst, const size_t wordlen) {

    if ( dst == NULL ) {
        return;
    }

    byte *p = (byte *)dst;
    int cnt = wordlen * sizeof(word);

    while ( cnt > 0 ) {
        *p = rand() & 0xff; // rand = DRBG
        p++;
        cnt--;
    }
}

void array_new_rand(word **const a, const size_t wordlen) {

    // 추가
    if ( *a != NULL ) {
#ifdef ZERORIZE
        array_init(*a, wordlen);
#endif
        free(*a);
        *a = NULL;
    }

    (*a) = (word *)calloc(wordlen, sizeof(word)); // [word] [word] .. : wordlen개

    byte *p = (byte *)(*a);
    int cnt = wordlen * sizeof(word);

    while ( cnt > 0 ) {
        *p = rand() & 0xff; // rand = DRBG
        p++;
        cnt--;
    }
}