#include "bigint.h"

//아래에 4가지 경우를 테스트 해봐서 맞다고 나와서 공유합니다.
//아직 python bignumber library를 이용하는 방법을 익히지 않아서
//수기로 가능한 범위에 대해서 해봐서 정확성이 100%라고 장담은 못하겠습니다.
//추후에 python bignumber library를 이용해서 정확하게 다시 테스트 할 예정입니다!
int main() {
    /*
    //0xffffffff ffffffff + 0xffffffff ffffffff =>  (hex) 00000001:ffffffff:fffffffe =>맞게 나옴
    //연산자1 생성
    bigint A;
    A.a = (word*)malloc(2*sizeof(word));
    assert(A.a != NULL);
    A.sign = NON_NEGATIVE;
    A.wordlen = 2;
    A.a[0] = 0xffffffff;
    A.a[1] = 0xffffffff;
    //연산자2 생성
    bigint B;
    B.a = (word*)malloc(2*sizeof(word));
    assert(B.a != NULL);
    B.sign = NON_NEGATIVE;
    B.wordlen = 2;
    B.a[0] = 0xffffffff;
    B.a[1] = 0xffffffff;
    //ADD 진행
    bigint* result = NULL;
    result = ADD(&A, &B);
    bi_show_hex(result);
    printf("\n");
    //메모리 삭제
    free(A.a);
    free(B.a);
    */
    
    /*
    //0x167899ef 567899fe - 0x12345678 ffffffee =>  (hex) 04444376:56789a10 (맞게 나옴)
    //연산자1 생성
    bigint A;
    A.a = (word*)calloc(2, sizeof(word));
    assert(A.a != NULL);
    A.sign = NON_NEGATIVE;
    A.wordlen = 2;
    A.a[0] = 0x567899fe;
    A.a[1] = 0x167899ef;
    //연산자2 생성
    bigint B;
    B.a = (word*)calloc(2, sizeof(word));
    assert(B.a != NULL);
    B.sign = NON_NEGATIVE;
    B.wordlen = 2;
    B.a[0] = 0xffffffee;
    B.a[1] = 0x12345678;
    //SUB 진행
    bigint* result = NULL;
    result = SUB(&A, &B);
    bi_show_hex(result);
    //메모리 삭제
    free(A.a);
    free(B.a);
    */

    /*
    //0x167899ef 567899fe - 0x00000000 00000000 =>   (hex) 167899ef:567899fe(맞게 나옴)
    bigint A;
    A.a = (word*)calloc(2, sizeof(word));
    assert(A.a != NULL);
    A.sign = NON_NEGATIVE;
    A.wordlen = 2;
    A.a[0] = 0x567899fe;
    A.a[1] = 0x167899ef;
    //연산자2 생성
    bigint B;
    B.a = (word*)calloc(2, sizeof(word));
    assert(B.a != NULL);
    B.sign = NON_NEGATIVE;
    B.wordlen = 2;
    B.a[0] = 0x00000000;
    B.a[1] = 0x00000000;
    //SUB 진행
    bigint* result = NULL;
    result = SUB(&A, &B);
    bi_show_hex(result);
    //메모리 삭제
    free(A.a);
    free(B.a);
    */

    /*
    //0x167899ef 567899fe + (-0x12345678 ffffffee) =>  (hex) 04444376:56789a10 (맞게 나옴)
    //연산자1 생성
    bigint A;
    A.a = (word*)calloc(2, sizeof(word));
    assert(A.a != NULL);
    A.sign = NON_NEGATIVE;
    A.wordlen = 2;
    A.a[0] = 0x567899fe;
    A.a[1] = 0x167899ef;
    //연산자2 생성
    bigint B;
    B.a = (word*)calloc(2, sizeof(word));
    assert(B.a != NULL);
    B.sign = NEGATIVE;
    B.wordlen = 2;
    B.a[0] = 0xffffffee;
    B.a[1] = 0x12345678;
    //SUB 진행
    bigint* result = NULL;
    result = ADD(&A, &B);
    bi_show_hex(result);
    //메모리 삭제
    free(A.a);
    free(B.a);
    */
   

    return 0;
} 