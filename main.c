#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>


#define M 0x80000000
#define BY2BE(arr, index, t) (((arr[index] - t) & M) >> (31 - index))

#define IMG_SIZE 26

int getSum(unsigned int* array, int rowNum){
    int i,sum;
    sum=0;
    for (i=0; i<rowNum; i++){
        sum+=__builtin_popcountl(array[i]);
    }
    return sum;
}


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

//320 x 240, shift =2; 160 x 120, shift =1, 
//for 320
//INPUT_ROWS = 240
//INPUT_COLS = 320
//HISTOGRAM_ROWS = 32
//HISTOGRAM_COLS = 32
//shift = 2

void subSample32x32(uint8_t* arr, int INPUT_ROWS, int INPUT_COLS, int shift, int HISTOGRAM_ROWS, int HISTOGRAM_COLS, uint8_t* retarray) {
    int start_row = (INPUT_ROWS >> 1) - (HISTOGRAM_ROWS << (shift - 1));
    int start_col = (INPUT_COLS >> 1) - (HISTOGRAM_COLS << (shift - 1));
    int end_row = start_row + (HISTOGRAM_ROWS << shift);
    int end_col = start_col + (HISTOGRAM_COLS << shift);
    int step = (1 << shift);
    int ret_idx = 0;
    
    for (int i = start_row; i < end_row; i += step) {
        for (int j = start_col; j < end_col; j += step) {
            retarray[ret_idx] = arr[i * INPUT_COLS + j];
            //printf("i,j,ret_idx,=%d,%d,%d retarray[ret_idx]=%d i*INPUT_COLS+j=%d\n",i,j,ret_idx,retarray[ret_idx], i*INPUT_COLS+j);
            ret_idx++;
        }
    }
    
    //for (int i=0; i<32; i++){
    //    printf(" %3d",retarray[i]);
    //}
}


void showStat(int* sizeStatistic_result){
    int j;
    for(j=0;j<24;j++){
        printf("X[%2d]=%3d, ",j,sizeStatistic_result[j]);
        if(j%6==5) printf("\n");
    }
    printf("\n");
}

void showStatF(float* sizeStatistic_result_norm){
    int j;
    for(j=0;j<24;j++){
        printf("X[%2d]=%.4f, ",j,sizeStatistic_result_norm[j]);
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





uint32_t calbits(uint8_t arr[32], uint32_t t) {
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

//arr size must 32x32
uint32_t calculate_32x32_bits_array(uint8_t *arr, uint8_t t, uint32_t* bits_array) {
    for (int i = 0; i < 32; i++) {
        uint32_t x = calbits(&arr[i * 32], t);
        bits_array[i] = x;
    }
    return 0;
}



int calculate_28_percent(uint8_t* arr) {
    int freq[256]={0};
    int sum_ = 0;
    uint8_t arrSize =32;
    float threshold = arrSize * arrSize * 0.28;
    threshold=286.5;
    
    for (int i = 0; i < arrSize; i++) {
        for (int j = 0; j < arrSize; j++) {
            freq[arr[i * 32 + j]] += 1;
            //printf("freq = %d, number=%d\n",arr[i * 32 + j],freq[arr[i * 32 + j]]);
        }
    }
    
    for (int i = 255; i >= 0; i--) {
        sum_ += freq[i];
        //printf("%d, %d, %d\n", i, freq[i], sum_);
        if (sum_ > threshold) {
            return i;
        }
    }
    return 0;
}


void create_matrices(uint8_t** ref_image, int theta, int scale, uint8_t** output_matrix) {
    int WIDTH = 42;
    int HEIGHT = 42;
    double angle = (theta * 3.14159 / 180);
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int matrix3 = round(((j - 21) * cos_angle + (i - 21) * sin_angle + 40) * scale);
            int matrix4 = round((-(j - 21) * sin_angle + (i - 21) * cos_angle + 30) * scale);
            output_matrix[i][j] = ref_image[matrix4][matrix3];
        }
    }
}


void allFeature(uint8_t* arr, int arrRows, int arrCols, uint32_t* sizeStatistic_result) {
    uint8_t arr_32x32[32 * 32];
    uint32_t bits_array[32];
    int t_value;
    
    
    subSample32x32(arr, arrRows, arrCols, 2, 32, 32, arr_32x32);
    t_value = calculate_28_percent(arr_32x32);
    //printf("t_value =%d\n",t_value);
    calculate_32x32_bits_array(arr_32x32, t_value, bits_array);
    sizeStatistic(bits_array, sizeStatistic_result);
    //showStat(sizeStatistic_result);
}

void allFeatureNorm(uint8_t* arr, int arrRows, int arrCols, float* sizeStatistic_result_norm) {
    int sizeStatistic_result[24];
    allFeature(arr, arrRows, arrCols, sizeStatistic_result);
    for (int i = 0; i < 4; i++) {
        float _sum = 1.0f;
        for (int j = 0; j < 6; j++) {
            _sum += sizeStatistic_result[i * 6 + j];
        }
        for (int j = 0; j < 6; j++) {
            sizeStatistic_result_norm[i * 6 + j] = sizeStatistic_result[i * 6 + j] / _sum;
        }
    }
}

/*
int main() {
    uint8_t arr[240][320];
    int i,j,k;
    // 初始化数组
    k=0;
    float sizeStatistic_result_norm[24]={0};
    
    //測試數據
    for (i = 0; i < 240; i++) {
        for (j = 0; j < 320; j++) {
            arr[i][j] = k % 255;
            k++;
        }
    }

    // 计算 28% 的值并输出
    
    allFeatureNorm((uint8_t *)arr, 240, 320, (float *)sizeStatistic_result_norm);
        
    showStatF(sizeStatistic_result_norm);


    return 0;
}
*/



typedef struct {
    unsigned char magic[2];
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct {
    unsigned int header_size;
    int width;
    int height;
    unsigned short color_planes;
    unsigned short bits_per_pixel;
    unsigned int compression_method;
    unsigned int image_size;
    int x_resolution;
    int y_resolution;
    unsigned int num_colors;
    unsigned int important_colors;
} BMPInfoHeader;


int main() {
    float sizeStatistic_result_norm[24]={0};
    int x,y, i;
    BMPHeader header;
    BMPInfoHeader info_header;

    FILE* file = fopen("gray_2023_1_16_15_56_33_Image.bmp", "rb");
    fread(&header, sizeof(BMPHeader), 1, file);
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);

    int width = 320;
    int height = 240;
    //printf("info_header.width=%d, info_header.height=%d\n", width, info_header.height);

    unsigned char* arr = (unsigned char*)malloc(width * height);

    fseek(file, 1024+54, SEEK_SET);
    for(int p=320*239; p>-1; p-=320){
        //printf("p=%d\n",p);
        fread(arr+p, sizeof(unsigned char), 320, file);
    }
    allFeatureNorm((uint8_t *)arr, 240, 320, (float *)sizeStatistic_result_norm);
        
    showStatF(sizeStatistic_result_norm);

    
    
    fclose(file);

    free(arr);
    
    return 0;
}
