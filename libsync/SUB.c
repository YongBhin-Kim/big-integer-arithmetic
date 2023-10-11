#include "bigint.h"
#include <assert.h>

//A: uint32_t data, B:uint32_t data, b:borrow-bit(0 or 1)
//(A-b)-B
int SUB_b(const word A, const word B, const int b, word* const result){
    int next_b = 0;
    *result = A - b;
    //아래 두 상황은 다음 워드열에서 값을 빌려온 상황
    next_b = A < b;
    next_b += *result < B;//여기서 어차피 0 or 1

    *result -= B;
    return next_b;
}

//A>=B>=0
//A-B
bigint* SUBC(bigint* const A, bigint* const B){
    //wordlen(A)+1의 결과값을 담을 메모리 할당
    bigint* result;
    size_t result_len = A->wordlen;
    bi_new(&result, result_len);
    result->sign = A -> sign;//A>=B>=0 -> 결과의 부호는 NON-NEGATIVE

    //B 메모리 A wordlen으로 재할당한 new_B -> realloc 쓰지 않고 그냥 새롭게 구현
    word* new_B = (word*)calloc(A->wordlen, sizeof(word));
    assert(new_B != NULL);
    memcpy(new_B, B->a, B->wordlen * sizeof(word));

    //연산 시작
    int b = 0;
    for(int i = 0; i < A->wordlen; i++){
        b = SUB_b(A->a[i], new_B[i], b, result->a + i);
    }
    bi_refine(result);//잘 동작하는지 확인
     //메모리 누수 방지
    free(new_B);
    new_B = NULL;

    return result;
}

//A-B
//PDF 4.3.2 OPTION PART는 우선 제외시킴 -> 상의 후 필요하다면 집어넣기
bigint* SUB(bigint* const A, bigint* const B){
    //0 <= B <= A
    if(B->sign == NON_NEGATIVE && (bi_compare(A, B) == 1 || bi_compare(A, B) == 0)){
        return SUBC(A, B);
    }
    //0 <= A <B
    else if(A->sign == NON_NEGATIVE && bi_compare(A, B) == -1){
        bigint* result = SUBC(B, A);
        result->sign = NEGATIVE;
        return result;
    }
    //0 > A >= B
    else if(A->sign == NEGATIVE && (bi_compare(A, B) == 1 || bi_compare(A, B) == 0)){
        //아래처럼 구현하는게 맞나?
        B->sign = NON_NEGATIVE;
        A->sign = NON_NEGATIVE;
        return SUBC(B, A);
    }
    //0 > B > A
     else if(B->sign == NEGATIVE && bi_compare(A, B) == -1){
        //아래처럼 구현하는게 맞나?
        B->sign = NON_NEGATIVE;
        A->sign = NON_NEGATIVE;
        bigint* result = SUBC(A, B);
        result->sign = NEGATIVE;
        return result;
    }
     //A >= 0 && B < 0
    else if(A->sign == NON_NEGATIVE && B->sign == NEGATIVE){
        B->sign = NON_NEGATIVE;
        ADD(A, B);
    }
          //A < 0 && B >= 0
    else{
        A->sign = NON_NEGATIVE;//|A|
        bigint* result = ADD(A, B);
        result->sign = NEGATIVE;
        return result;
    }
}