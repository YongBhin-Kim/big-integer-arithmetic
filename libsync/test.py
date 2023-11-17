
# Addition
a = 0x8e2958a1475ed70762340e9797788e0061f21fcebd67889fdd4f4ce2b5f6b2deb2c950abc87a5544
b = 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3

# // C에서 python양식으로 test vector용 함수

# Multiplication
# 1. Single Word Mul
print(0xfffffffe00000001 == (0xffffffff * 0xffffffff))
print(0x7388318d1c9b9927 == (0xb23ecfdd * 0xa5edfed3))
# 2. Textbook Mul Using Single Mul
print(0x18dc1d3be2ff5d77153cfc66d8440869afdaa0ee97b09bd24f2c8138010929769bd79a9abd6c44287a039927 == 0x8e36cfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3)
print(0x000000c6ddadf9c82bffb026a9e8127c228ddd893964b330e9557af98b6a6a9e7cf5b947296c3dc7b8fb3321dddb9927 == 0x00000471a3fecfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3)
# 3. Improved Textbook Mul
# 4. Karatsuba Mul
print(0x00031b76aa4ca184e5188b509a3e22c50e68a2cfc7a00b69c3800b30e5906b3cf3af551320036e1d0d5bdc5563e37f1a879aaf33c781f00da4a846f339e79deb7107f7eb1c9b9927 == (0x0011c68fad6d56fb23ecfdd471a3eb5b55bec8fb3f751c68fad6d56fb23ecfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3))
print("%x" %(0x027eecfdd9fbb3f767eecfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3))


print("%x" %(0x0009fbb3f767eecfdd9fbb3f767eecfdd9fbb3f767eecfdd * 0x2cc00f1e3ac38f81b7e95036fd191ea134ff616d9806e10cae20e251c36cfa5be4d9f3d5a5edfed3))
# 1bec04c6cc03c03f55f83446d7b42daebc6897e10acecea543d88bfeed4309db7519280cc37e811c8296077b434a205e71b58fee95125a863abfa7cab9927

# 5. Long division
x = 0x300127af9cd1bcc189910a2c7ee16add2f168c
y = 0xb78a0100cf62be18cc1
q = x // y
r = x % y
print("q = %x\nr = %x" %(q, r))
print("%x" %(y * q + r))

# Barrett Reduction
import math
print("%x" % (0x01000000000000000000000000000000000000000000000000 // 0xb78a0100cf62be18cc1))
print(0x3097c91b6cccd3197d2 == 0x300127af9cd1bcc189910a2c7ee16add2f168c % 0xb78a0100cf62be18cc1)