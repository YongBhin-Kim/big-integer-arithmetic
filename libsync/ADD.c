#include "bigint.h"

//A: uint32_t data, B:uint32_t data, c:carry-bit(0 or 1)
//(A+B)+c
//parameter result는 워드열의 각 해당 워드 주소가 날라온것 -> ADDC 고려해서 이렇게 구현
int ADD_c(const word A, const word B, const int c, word* const result){
    int next_c = 0;
    word C = A + B;//A + B(mod 2^32)
    next_c = (C < A);
    C += c;//이전 연산단계에서 넘어온 carry-bit랑 ADD
    next_c += C < c;//next_c가 2가 되는일은 없어 -> 0 or 1 (double carry 발생X)
    *result = C;
    return next_c;
}

//ADDC(A,B)
//A,B 부호 같다고 가정
//(1) 아래 구현은 wordlen(A) >= wordlen(B)라고 가정하고 구현 
bigint* ADDC(bigint* const A, bigint* const B){
    //wordlen(A)+1의 결과값을 담을 메모리 할당
    bigint* result;
    size_t result_len = A->wordlen + 1;
    bi_new(&result, result_len);
    result->sign = A -> sign;

    //B 메모리 A wordlen으로 재할당한 new_B -> realloc 쓰지 않고 그냥 새롭게 구현
    word* new_B = (word*)calloc(A->wordlen, sizeof(word));
    assert(new_B != NULL);
    memcpy(new_B, B->a, B->wordlen * sizeof(word));

    //연산 시작
    int carry = 0;
    for(int i = 0; i < A->wordlen; i++){
        carry = ADD_c(A->a[i], new_B[i], carry, result->a + i);
    }
    result -> a[result_len - 1] = carry;
    //마지막 MSB에서 CARRY가 발생하지 않았다고 하더라도 REFINE()과정은 안하는 게 더 좋다고 생각!
    
    //메모리 누수 방지
    free(new_B);
    new_B = NULL;

    return result;
}

//ADD(A,B)
//PDF 3.3.2 OPTION PART는 우선 제외시킴 -> 상의 후 필요하다면 집어넣기
bigint* ADD(bigint* const A, bigint* const B){
    //A >= 0 && B < 0
    if(A->sign == NON_NEGATIVE && B->sign == NEGATIVE){
        B->sign = NON_NEGATIVE;//|B|
        return SUB(A, B);
    }
    //A < 0 && B >= 0
    else if(A->sign == NEGATIVE && B->sign == NON_NEGATIVE){
        A->sign = NON_NEGATIVE;//|A|
        return SUB(B, A); 
    }
    else{
        if(A->wordlen >= B->wordlen){
            return ADDC(A, B);
        }
        else{
            return ADDC(B, A);
        }
    }
    
}