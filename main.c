#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#define INPUT_ROWS 120
#define INPUT_COLS 160
#define HISTOGRAM_ROWS 64
#define HISTOGRAM_COLS 64
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

uint32_t calculate_32x32_bits_array(uint8_t arr[INPUT_ROWS][INPUT_COLS], uint32_t t, uint32_t bits_array[32]) {
    //uint32_t bits_array[32] = { 0 };
    uint32_t count = HISTOGRAM_ROWS * HISTOGRAM_COLS;
    uint32_t start_row = (INPUT_ROWS - HISTOGRAM_ROWS) / 2;
    uint32_t start_col = (INPUT_COLS - HISTOGRAM_COLS) / 2;
    int i,j;

    // 统计每个元素的频率
    j=0;
    for (i = start_row; i < start_row + HISTOGRAM_ROWS; i++) {
    	bits_array[j] = calbits(arr[i]+start_col,t);
	j++;
    }
    return 0;
}

uint32_t calculate_28_percent(uint8_t arr[INPUT_ROWS][INPUT_COLS]) {
    uint32_t freq[256] = { 0 };
    uint32_t count = HISTOGRAM_ROWS * HISTOGRAM_COLS;
    uint32_t threshold = count * 28 / 100;
    uint32_t sum = 0;
    uint32_t start_row = (INPUT_ROWS - HISTOGRAM_ROWS) / 2;
    uint32_t start_col = (INPUT_COLS - HISTOGRAM_COLS) / 2;
    int i,j;

    // 统计每个元素的频率
    for (i = start_row; i < start_row + HISTOGRAM_ROWS; i++) {
        for (j = start_col; j < start_col + HISTOGRAM_COLS; j++) {
            freq[arr[i][j]]++;
        }
    }

    // 排序并找到 28% 阈值
    for (i = 255; i >= 0; i--) {
            sum += freq[i];
            if (sum >= threshold) {
                return i;
        }
    }

    return 0;
}

int main() {
    uint8_t arr[INPUT_ROWS][INPUT_COLS];
    int i,j,k;
    // 初始化数组
    k=0;
    for (i = 0; i < INPUT_ROWS; i++) {
        for (j = 0; j < INPUT_COLS; j++) {
            arr[i][j] = k % 255;
	    k++;
        }
    }

    // 计算 28% 的值并输出
    uint32_t value = calculate_28_percent(arr);
    printf("28%% of values are ==> %u\n", value);
    uint32_t bits_array[32] = { 0 };
    calculate_32x32_bits_array(arr, value, bits_array);

    for (i=0; i<32; i++){
        printf("%d, %x\n",i, bits_array[i]);
    }

    return 0;
}

