#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define M 0x80000000
#define BY2BE(arr, index, t) (((arr[index<<1] - t) & M) >> (31 - index))

uint32_t calbits(uint8_t arr[64], uint32_t t) {
    uint32_t bits = 0;
    bits |= BY2BE(arr, 0, t);
    bits |= BY2BE(arr, 1, t);
    bits |= BY2BE(arr, 2, t);
    bits |= BY2BE(arr, 3, t);
    bits |= BY2BE(arr, 4, t);
    bits |= BY2BE(arr, 5, t);
    bits |= BY2BE(arr, 6, t);
    bits |= BY2BE(arr, 7, t);
    bits |= BY2BE(arr, 8, t);
    bits |= BY2BE(arr, 9, t);
    bits |= BY2BE(arr, 10, t);
    bits |= BY2BE(arr, 11, t);
    bits |= BY2BE(arr, 12, t);
    bits |= BY2BE(arr, 13, t);
    bits |= BY2BE(arr, 14, t);
    bits |= BY2BE(arr, 15, t);
    bits |= BY2BE(arr, 16, t);
    bits |= BY2BE(arr, 17, t);
    bits |= BY2BE(arr, 18, t);
    bits |= BY2BE(arr, 19, t);
    bits |= BY2BE(arr, 20, t);
    bits |= BY2BE(arr, 21, t);
    bits |= BY2BE(arr, 22, t);
    bits |= BY2BE(arr, 23, t);
    bits |= BY2BE(arr, 24, t);
    bits |= BY2BE(arr, 25, t);
    bits |= BY2BE(arr, 26, t);
    bits |= BY2BE(arr, 27, t);
    bits |= BY2BE(arr, 28, t);
    bits |= BY2BE(arr, 29, t);
    bits |= BY2BE(arr, 30, t);
    bits |= BY2BE(arr, 31, t);
    bits ^= 0xFFFFFFFF;
    return bits;
}

int main() {
    uint8_t arr1[64] = {20, 19, 20, 18, 178, 176, 177, 179, 18, 18, 189, 187, 186, 192, 19, 19, 197, 195, 196, 198, 193, 194, 188, 185, 184, 181, 210, 220, 23, 24, 25, 26, 250, 250, 251, 252, 253, 20, 20, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint32_t result1 = calbits(arr1,178);
    printf("%x\n",result1);
    assert(result1 == 0xfff73f64);

    uint8_t arr3[64] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint32_t result3 = calbits(arr3,178);
    assert(result3 == 0xFFFFFFFF);

}
