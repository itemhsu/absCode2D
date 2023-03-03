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

#define IMG_SIZE 26
//#define ESP32ASM
#ifdef ESP32ASM
int getSum(unsigned int* array, int rowNum){
    int i,sum;
    uint32_t result;
    uint32_t x
    sum=0;
    for (i=0; i<rowNum; i++){
        x=array[i];
        __asm__ __volatile__("popc %0, %1" : "=r" (result) : "r" (x));
        sum+=result;
    }
    return sum;
}
#else
int getSum(unsigned int* array, int rowNum){
    int i,sum;
    sum=0;
    for (i=0; i<rowNum; i++){
        sum+=__builtin_popcountl(array[i]);
    }
    return sum;
}
#endif

int sizeStatistic(unsigned int* bigger, int* sizeStatistic_result) {
    unsigned int bigger_tmp_H[32];
    unsigned int bigger_tmp_V[26];
    unsigned int bigger_tmp_U[26];
    unsigned int bigger_tmp_D[26];
    int i,j;

    // bigger = img > imgThreshold
    
    // plt.imshow(bigger)
    // ...

    // bigger_tmp = bigger[0:26,6:26]
    for (i = 0; i < 26; i++) {
        bigger_tmp_V[i] = bigger[i];
        bigger_tmp_U[i] = bigger[i+6] & 0x3ffffff;
        bigger_tmp_D[i] = bigger[i] & 0x3ffffff;
    }
    
    for (i = 0; i < 32; i++) {
        bigger_tmp_H[i] = bigger[i] & 0x3ffffff;
    }

    for (i = 0; i < 24; i++) {
        sizeStatistic_result[i] = 0;
    }

    for (j = 1; j < 7; j++) {
        for (i = 0; i < 26; i++) {
            bigger_tmp_V[i] = bigger_tmp_V[i] & (bigger[i+j]);
            bigger_tmp_U[i] = bigger_tmp_U[i] & ((bigger[i+6-j]>>j) & 0x3ffffff);
            bigger_tmp_D[i] = bigger_tmp_D[i] & ((bigger[i+j]>>j) & 0x3ffffff);
        }
    
        for (i = 0; i < 32; i++) {
            bigger_tmp_H[i] = bigger_tmp_H[i] & ((bigger[i]>>j) & 0x3ffffff);
        }
        
        sizeStatistic_result[j-1+0]=getSum(bigger_tmp_H, 32);
        sizeStatistic_result[j-1+6]=getSum(bigger_tmp_V, 26);
        sizeStatistic_result[j-1+12]=getSum(bigger_tmp_U, 26);
        sizeStatistic_result[j-1+18]=getSum(bigger_tmp_D, 26);

    }
    return 0;
}

void showStat(int* sizeStatistic_result){
    int j;
    for(j=0;j<24;j++){
        printf("X[%2d]=%3d, ",j,sizeStatistic_result[j]);
        if(j%6==5) printf("\n");
    }
    printf("\n");
}

int unit_test_sizeStatistic() {
    unsigned int bigger[32]={0};
    int sizeStatistic_result[24]={0};
    //test 0
    int i,j;
    for(i=0;i<32;i++){
        bigger[i]=0xffffffff;
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);

    for(i=0;i<32;i++){
        bigger[i]=0xf0f0f0f;
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
    
    for(i=0;i<32;i++){
        bigger[i]=0x7f7f7f7f;
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
    
    for(i=0;i<32;i++){
        unsigned int pattern=0xf0f0f0f;
        if ((pattern>>i)&1) bigger[i]=0xffffffff;
        else bigger[i]=0;
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
    
    for(i=0;i<32;i++){
        unsigned int pattern=0x7f7f7f7f;
        if ((pattern>>i)&1) bigger[i]=0xffffffff;
        else bigger[i]=0;
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
    
    for(i=0;i<32;i++){
        unsigned int pattern=0xf0f0f0f;
        bigger[i]=((pattern>>i) | (pattern<<(32-i)));
        //printf("%2d: %x, %8x, %8x,\n",i,bigger[i], (pattern>>i), );
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
 
    for(i=0;i<32;i++){
        unsigned int pattern=0xf0f0f0f;
        bigger[i]=((pattern<<i) | (pattern>>(32-i)));
        //printf("%2d: %x, %8x, %8x,\n",i,bigger[i], (pattern>>i), );
    }
    sizeStatistic(bigger, sizeStatistic_result);
    showStat(sizeStatistic_result);
    
    return 0;
}





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
    int sizeStatistic_result[24]={0};
    
    //測試數據
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
    
    sizeStatistic(bits_array, sizeStatistic_result);
    showStat(sizeStatistic_result);


    return 0;
}

