//내가 추가한 함수 -> 메모리 배열은 realloc을 이용해서 재할당 해주는 함수
//x를 *형태로 구현해도 상관x
int bi_realloc(bigint** const x, size_t size)
{
    word* new_x = (word*)realloc((*x)->a, sizeof(word) * (size));
    if (new_x == NULL) {
        free(new_x);
        new_x = NULL;
        return FAIL;
    }
    (*x)->a = new_x;
    return SUCCESS;
}


//improved multi version
//z <- x * y
int bi_mul_improved_zxy(bigint** z, const bigint* x, const bigint* y) {
    //11.13 - 수정사항에서 추가된 변수
    bigint* new_x = NULL;
    bi_assign(&new_x, x);
    bigint* new_y = NULL;
    bi_assign(&new_y, y);

    //x,y의 wordlen이 짝수가 아니라면 -> msb에 allzero 짜리 word하나 추가 -> x,y의 wordlen을 짝수열로 맞추자!
    if (new_x->wordlen % 2 != 0) {
        size_t new_wordlen = new_x->wordlen + 1;
        bi_realloc(&new_x, sizeof(word) * (new_wordlen));
        new_x->wordlen = new_wordlen;
        new_x->a[new_wordlen - 1] = 0;
    }
    if (new_y->wordlen % 2 != 0) {
        size_t new_wordlen = new_y->wordlen + 1;
        bi_realloc(&new_y, sizeof(word) * (new_wordlen));
        new_y->wordlen = new_wordlen;
        new_y->a[new_wordlen - 1] = 0;
    }

    //loop에 필요한 변수
    size_t n = new_x->wordlen / 2;
    int cnt_i, cnt_j;

    //연산 시작

    //미리 T0, T1을 크게 잡고 시작 -> CONCAT을 효율적으로 해주기 위해서!
    //T0는 2WORD 크기의 값을 n번(0 ~ n-1) CONCAT 하므로 -> 2 * n 만큼 잡음
    //T1은 2WORD 크기의 값을 n번 CONCAT + 맨 처음에 ALL ZERO짜리 1WORD를 CONCAT 하고 시작하므로 -> 2 * n + 1
    //아래 안쪽 for loop에서 필요하지만 -> 해당 위치에 bi_new를 쓰면 불필요한 bi_new 반복 호출 -> 비효율
    //그래서 아래 위치에 bi_new를 이용해서 T0,T1을 할당 해놓고 계속 재활용 할 예정!
    bigint* T0 = NULL;
    bi_new(&T0, 2 * n);
    bigint* T1 = NULL;
    bi_new(&T1, (2 * n) + 1);

    //Z = Z + T
    //과정에서 사용할 버퍼 변수 C 생성
    bigint* C = NULL;
    bi_set_zero(&C);

    bigint* T = NULL;

    word T0_MV[2] = { 0, };
    word T1_MV[2] = { 0, };

    for (cnt_i = 0; cnt_i < new_y->wordlen; cnt_i++) {
        for (cnt_j = 0; cnt_j < n; cnt_j++) {
            //T0,T1 중간값 -> 2word 
            bi_mul_zj(T0_MV, new_x->a[2 * cnt_j], new_y->a[cnt_i]);
            bi_mul_zj(T1_MV, new_x->a[(2 * cnt_j) + 1], new_y->a[cnt_i]);

            //T0 -> CONCAT하면서 값 생성
            T0->a[2 * cnt_j] = T0_MV[0];
            T0->a[(2 * cnt_j) + 1] = T0_MV[1];

            //T1 -> CONCAT하면서 값 생성
            T1->a[(2 * cnt_j) + 1] = T1_MV[0];
            T1->a[(2 * cnt_j) + 2] = T1_MV[1];
        }
        //ADDC
        //T <- T0 + T1
        bi_add(&T, T0, T1);
   
        //SHIFT
        //T << 32 * w
        size_t shift_len = WORD_BITS * cnt_i;
        bi_shift_left(&T, shift_len);

        //ADD
        //z <- z + T
        bi_add(z, C, T);
        bi_assign(&C, *z);
    }

    //메모리 누수 관리 & z refine
    bi_refine(*z);
    bi_delete(&new_x);
    bi_delete(&new_y);
    bi_delete(&C);
    bi_delete(&T);
    bi_delete(&T0);
    bi_delete(&T1);

    return SUCCESS;
}
