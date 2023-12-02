#include "array.h"

void array_init(word *a, const size_t wordlen) {
    if ((wordlen == 0) || (a == NULL)) {
        return;
    }

    memset(a, 0, wordlen * sizeof(word));
}

void array_copy(word *dst, const word *src, const size_t wordlen) {
    if ((wordlen == 0) || (src == NULL) || (dst == NULL)) {
        return;
    }

    memcpy(dst, src, wordlen * WORD_BYTES);
}

void array_show_hex(const word *a, const size_t wordlen) {
    int i = wordlen;

    for (; i-- > 1;)
        printf(HEX_FORMAT, a[i]);
    printf(HEX_FORMAT, a[i]);
    printf("\n");
}

void array_rand(word *dst, const size_t wordlen) {
    if (dst == NULL) {
        return;
    }

    byte *p = (byte *)dst;
    int cnt = wordlen * sizeof(word);

    while (cnt > 0) {
        *p = rand() & 0xff; // rand = DRBG
        p++;
        cnt--;
    }
}

void array_new_rand(word **const a, const size_t wordlen) {
    if (*a != NULL) {
#if ZERORIZE
        array_init(*a, wordlen);
#endif
        free(*a);
        *a = NULL;
    }

    /*
        [word_n-1] [word_n-2] ... [word_0]: wordlen 개
    */
    if ( wordlen > 0 ) {
        (*a) = (word *)malloc(wordlen * sizeof(word));  
        array_init(*a, wordlen);
    }
    else {
        (*a) = NULL;
    }

    byte *p = (byte *)(*a);
    size_t cnt = wordlen * sizeof(word);

    while (cnt > 0) {
        *p = rand() & 0xff;     // rand = DRBG
        p++;
        cnt--;
    }
}