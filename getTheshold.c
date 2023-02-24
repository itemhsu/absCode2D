#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define INPUT_ROWS 120
#define INPUT_COLS 160
#define HISTOGRAM_ROWS 64
#define HISTOGRAM_COLS 64

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
            arr[i][j] = k % 256;
	    k++;
        }
    }

    // 计算 28% 的值并输出
    uint32_t value = calculate_28_percent(arr);
    printf("28%% of values are ==> %u\n", value);

    return 0;
}

