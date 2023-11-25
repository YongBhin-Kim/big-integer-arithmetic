# BBQ; Best Biginteger Quality Attribute
Implementation of the libsync Team's Big integer operation Library (Advanced Application Programming)

**members**
- Yongbhin Kim
- Seunghyeon Kim
- Kihyeon Shin
- Eunsu Jeong

**array**
- void array_init(word *a, const size_t wordlen);
- void array_copy(word *dst, const word *src, const size_t wordlen);
- void array_show_hex(const word *a, const size_t wordlen);
- void array_rand(word *dst, const size_t wordlen);
- void array_new_rand(word **const a, const size_t wordlen);


**bigint**
- void bi_delete(bigint **const x);
- void bi_new(bigint **const x, const size_t wordlen);
- void bi_show_hex(const bigint *x);
- // void bi_show_dec(const bigint *x);
- void bi_show_bin(const bigint *x);
- void bi_set_by_array(bigint **const x, const int sign, const word *a, const size_t wordlen);
- // int bi_set_by_string(bigint **const x, const int sign, const char *str, const int base);
- void bi_refine(bigint *x);
- void bi_assign(bigint **const y, const bigint *x);
- void bi_gen_rand(bigint **const x, const int sign, const size_t wordlen);
- void bi_set_one(bigint **const x);
- void bi_set_zero(bigint **const x);
- // void bi_add(bigint *z, bigint *x, bigint *y);
- // void bi_add(bigint *z, bigint *x);
- // void bi_sub(bigint *z, bigint *x, bigint *y);
- // void bi_sub(bigint *z, bigint *x);
