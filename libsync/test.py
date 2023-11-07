
# Addition
a = 0x8e2958a1475ed70762340e9797788e0061f21fcebd67889fdd4f4ce2b5f6b2deb2c950abc87a5544
b = 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3

# // C에서 python양식으로 test vector용 함수

# Multiplication
# 1. Single Word Mul
print(0xfffffffe00000001 == (0xffffffff * 0xffffffff))
print(0x5c2d8a477a039927 == (0x8e36cfdd * 0xa5edfed3))
# 2. Textbook Mul Using Single Mul
print(0x18dc1d3be2ff5d77153cfc66d8440869afdaa0ee97b09bd24f2c8138010929769bd79a9abd6c44287a039927 == 0x8e36cfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3)
print(0x000000c6ddadf9c82bffb026a9e8127c228ddd893964b330e9557af98b6a6a9e7cf5b947296c3dc7b8fb3321dddb9927 == 0x00000471a3fecfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3)
# 3. Improved Textbook Mul
