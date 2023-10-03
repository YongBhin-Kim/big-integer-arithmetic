//x->word[] mod2^r
//인자로 받은 bigint를 연산 결과값으로 바꿔버리는 형식을 택함
void bi_reduction(bigint *x, const size_t r){
    if(r < 0){//r이 음수인 경우는 예외처리 or 에러메세지 출력
        return;
    }

    if(r == 0){ //A mod1 -> 0
        memset(x->a, 0x00, sizeof(word) * x->wordlen);//bi_set_zero 사용하기
        //refine(x); //팀원들과 상의해서 결정
    }
    else if(r >= 32 * x->wordlen){//결과값이 input값 그대로 인출 -> 아무것도 하지 않고 빠져나오도록 구현
        return;
    }
    else if(r % 32 == 0){
        size_t q = r / 32;
        memset(x->a + q, 0x00, sizeof(word) * (x->wordlen - q));//하위 q개의 word열만 남겨놓자 -> 나머지 상위 word열은 0x00처리
        //refine(x); //팀원들과 상의해서 결정
    }
    else{ // r = 32 * q + r'
        size_t q = r / 32;
        size_t remainder = r % 32;
        memset(x->a + q, 0x00, sizeof(word) * (x->wordlen - q));//하위 q개의 word열만 남겨놓자 -> 나머지 상위 word열은 0x00처리
        word value = (1 << remainder) -1;//2^remainder - 1, 연산자 우선순위 조심 -> shift연산자 < 뺄셈
        x->a[q] = x->a[q] & value;
        //refine(x); //팀원들과 상의해서 결정
    }
}