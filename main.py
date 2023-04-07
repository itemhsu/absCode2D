import numpy as np


def getSum(array):
    sum = 0
    for x in array:
        sum += bin(x).count("1")
    return sum

def q_sizeStatistic(bigger):
    bigger_tmp_H = np.zeros(32, dtype=np.uint32)
    bigger_tmp_V = np.zeros(26, dtype=np.uint32)
    bigger_tmp_U = np.zeros(26, dtype=np.uint32)
    bigger_tmp_D = np.zeros(26, dtype=np.uint32)

    # bigger_tmp = bigger[0:26,6:26]
    for i in range(26):
        bigger_tmp_V[i] = bigger[i]
        bigger_tmp_U[i] = bigger[i + 6] & 0x3ffffff
        bigger_tmp_D[i] = bigger[i] & 0x3ffffff

    for i in range(32):
        bigger_tmp_H[i] = bigger[i] & 0x3ffffff

    sizeStatistic_result = np.zeros(24, dtype=np.int32)

    for j in range(1, 7):
        for i in range(26):
            bigger_tmp_V[i] &= bigger[i + j]
            bigger_tmp_U[i] &= (bigger[i + 6 - j] >> j) & 0x3ffffff
            bigger_tmp_D[i] &= (bigger[i + j] >> j) & 0x3ffffff

        for i in range(32):
            bigger_tmp_H[i] &= (bigger[i] >> j) & 0x3ffffff

        sizeStatistic_result[j -1 + 0] = getSum(bigger_tmp_H)
        sizeStatistic_result[j -1 + 6] = getSum(bigger_tmp_V)
        sizeStatistic_result[j -1 + 12] = getSum(bigger_tmp_U)
        sizeStatistic_result[j -1 + 18] = getSum(bigger_tmp_D)
    return sizeStatistic_result


def q_sizeStatistic_7(bigger):
    bigger_tmp_H = np.zeros(32, dtype=np.uint32)
    bigger_tmp_V = np.zeros(25, dtype=np.uint32)
    bigger_tmp_U = np.zeros(25, dtype=np.uint32)
    bigger_tmp_D = np.zeros(25, dtype=np.uint32)

    # bigger_tmp = bigger[0:26,6:26]
    for i in range(25):
        bigger_tmp_V[i] = bigger[i]
        bigger_tmp_U[i] = bigger[i + 7] & 0x1ffffff
        bigger_tmp_D[i] = bigger[i] & 0x1ffffff

    for i in range(32):
        bigger_tmp_H[i] = bigger[i] & 0x1ffffff

    sizeStatistic_result = np.zeros(28, dtype=np.int32)

    for j in range(1, 8):
        for i in range(25):
            bigger_tmp_V[i] &= bigger[i + j]
            bigger_tmp_U[i] &= (bigger[i + 7 - j] >> j) & 0x1ffffff
            bigger_tmp_D[i] &= (bigger[i + j] >> j) & 0x1ffffff

        for i in range(32):
            bigger_tmp_H[i] &= (bigger[i] >> j) & 0x1ffffff

        sizeStatistic_result[j -1 + 0] = getSum(bigger_tmp_H)
        sizeStatistic_result[j -1 + 7] = getSum(bigger_tmp_V)
        sizeStatistic_result[j -1 + 14] = getSum(bigger_tmp_U)
        sizeStatistic_result[j -1 + 21] = getSum(bigger_tmp_D)
    return sizeStatistic_result


#320 x 240, shift =2; 160 x 120, shift =1, 
def subSmaple32x32(arr, INPUT_ROWS, INPUT_COLS, shift):
    HISTOGRAM_ROWS = int(32)
    HISTOGRAM_COLS = int(32)

    start_row = (INPUT_ROWS>>1) - (HISTOGRAM_ROWS<<(shift-1))
    #向左移動，結果亮度剛好，batch feature.N 計算img/2023_1_16_15_56_33_Image.bmp 得到漂亮的-78度
    start_col = (INPUT_COLS>>2) - (HISTOGRAM_COLS<<(shift-1))
    start_col = (INPUT_COLS>>1) - (HISTOGRAM_COLS<<(shift-1))
    end_row = start_row + (HISTOGRAM_ROWS<<shift)
    end_col = start_col + (HISTOGRAM_COLS<<shift)
    step= (1<<shift)
    retarray=arr[start_row:end_row:step, start_col:end_col:step]
    
    #for i in range(32):
    #    print(" %3d",retarray[i])

    
    return retarray


def r_subSmaple32x32(arr, INPUT_ROWS, INPUT_COLS, shift):
    HISTOGRAM_ROWS = int(32)
    HISTOGRAM_COLS = int(32)

    start_row = (INPUT_ROWS>>1) - (HISTOGRAM_ROWS<<(shift-1))
    start_col = (INPUT_COLS>>1)
    end_row = start_row + (HISTOGRAM_ROWS<<shift)
    end_col = start_col + (HISTOGRAM_COLS<<shift)
    step= (1<<shift)
    retarray=arr[start_row:end_row:step, start_col:end_col:step]
    
    #for i in range(32):
    #    print(" %3d",retarray[i])

    
    return retarray

def l_subSmaple32x32(arr, INPUT_ROWS, INPUT_COLS, shift):
    HISTOGRAM_ROWS = int(32)
    HISTOGRAM_COLS = int(32)

    start_row = (INPUT_ROWS>>1) - (HISTOGRAM_ROWS<<(shift-1))
    start_col = (INPUT_COLS>>1) - (HISTOGRAM_COLS<<(shift))
    end_row = start_row + (HISTOGRAM_ROWS<<shift)
    end_col = start_col + (HISTOGRAM_COLS<<shift)
    step= (1<<shift)
    retarray=arr[start_row:end_row:step, start_col:end_col:step]
    
    #for i in range(32):
    #    print(" %3d",retarray[i])

    
    return retarray



def BY2BE(arr, index, t):
    x = ((arr[index] - t) & 0x80000000) >> (31 - index)
    return x

def calbits(arr, t):
    bits = 0
    bits |= BY2BE(arr,  0, t)
    bits |= BY2BE(arr,  1, t)
    bits |= BY2BE(arr,  2, t)
    bits |= BY2BE(arr,  3, t)
    bits |= BY2BE(arr,  4, t)
    bits |= BY2BE(arr,  5, t)
    bits |= BY2BE(arr,  6, t)
    bits |= BY2BE(arr,  7, t)
    bits |= BY2BE(arr,  8, t)
    bits |= BY2BE(arr,  9, t)
    bits |= BY2BE(arr,  10, t)
    bits |= BY2BE(arr,  11, t)
    bits |= BY2BE(arr,  12, t)
    bits |= BY2BE(arr,  13, t)
    bits |= BY2BE(arr,  14, t)
    bits |= BY2BE(arr,  15, t)
    bits |= BY2BE(arr,  16, t)
    bits |= BY2BE(arr,  17, t)
    bits |= BY2BE(arr,  18, t)
    bits |= BY2BE(arr,  19, t)
    bits |= BY2BE(arr,  20, t)
    bits |= BY2BE(arr,  21, t)
    bits |= BY2BE(arr,  22, t)
    bits |= BY2BE(arr,  23, t)
    bits |= BY2BE(arr,  24, t)
    bits |= BY2BE(arr,  25, t)
    bits |= BY2BE(arr,  26, t)
    bits |= BY2BE(arr,  27, t)
    bits |= BY2BE(arr,  28, t)
    bits |= BY2BE(arr,  29, t)
    bits |= BY2BE(arr,  30, t)
    bits |= BY2BE(arr,  31, t)
    bits ^= 0xFFFFFFFF
    #print(bits)
    return bits

def calculate_32x32_bits_array(arr, t):
    bits_array = np.zeros(32, dtype=np.uint32)
    for i in range(32):
        x=calbits(arr[i,:], t)
        bits_array[i] = x
    
    return bits_array

import numpy as np

#320 x 240, scale =2
#160 x 120, scale =1
def calculate_28_percent(arr,arrSize):
    freq=np.zeros(256)
    sum_=0
    for i in range(arrSize):
        for j in range(arrSize):
            freq[arr[i][j]] += 1
            
    ##if isPlot:
    ##    one_d_array = np.ravel(arr)
    ##    sorted_array = np.sort(one_d_array)
    ##    plt.plot(sorted_array)


    # 排序并找到 28% 阈值
    threshold=arrSize*arrSize*0.28
    threshold=286.5

    for i in range(255, -1, -1):
        sum_ += freq[i]
        #print(i, freq[i], sum_)
        if sum_ > threshold:
            return i

    return 0

import math
def calculate_28_percent_narrow(arr,arrSize, boundSize):
    freq=np.zeros(256)
    sum_=0
    for i in range(arrSize):
        for j in range(arrSize):
            freq[arr[i][j]] += 1
            
    ##if isPlot:
    ##    one_d_array = np.ravel(arr)
    ##    sorted_array = np.sort(one_d_array)
    ##    plt.plot(sorted_array)


    # 排序并找到 28% 阈值
    #math.floor(x)
    threshold=math.ceil(((arrSize-2*boundSize)**2)*0.28)-0.5


    for i in range(255, -1, -1):
        sum_ += freq[i]
        #print(i, freq[i], sum_)
        if sum_ > threshold:
            return i

    return 0

import math
def create_matrices(ref_image, theta , scale): # scale for 320x240 should be 4, for 160x120 should be 2
    WIDTH = 42
    HEIGHT = 42
    angle =  (theta*3.14159/180)
    output_matrix = np.zeros((HEIGHT, WIDTH), dtype=np.uint8)

    cos_angle = math.cos(angle)
    sin_angle = math.sin(angle)
    #print(cos_angle, sin_angle)
    for i in range(HEIGHT):
        for j in range(WIDTH):
            matrix3 = int(round(((j-21)*cos_angle + (i-21)*sin_angle  + 40)*scale))
            matrix4 = int(round(( -(j-21)*sin_angle + (i-21)*cos_angle  + 30)*scale))
            output_matrix[i][j] = ref_image[matrix4][matrix3]

    return output_matrix


def allFeature(arr):
        
    arr_32x32=subSmaple32x32(arr, INPUT_ROWS=240, INPUT_COLS=320, shift=2)
    sizeStatistic_result=allFeature_core(arr_32x32)
    # 计算 28% 的值并输出
    return sizeStatistic_result

def allFeature_core(arr_32x32):
        
    # 计算 28% 的值并输出
    t_value = calculate_28_percent(arr_32x32, arrSize=32)
    #print("28% of values are ==> {}".format(t_value))
    bits_array = np.zeros(32, dtype=np.uint32)
    bits_array = calculate_32x32_bits_array(arr_32x32, t_value)
    #show_32x32(arr_32x32,bits_array)
    sizeStatistic_result= q_sizeStatistic(bits_array)
    return sizeStatistic_result



def allFeatureNorm(arr):
    sizeStatistic_result=allFeature(arr);
    sizeStatistic_result_norm = np.zeros(24, float)
    for i in range(4):
        _sum=1
        for j in range(6):
            _sum+=sizeStatistic_result[i*6+j]
        for j in range(6):
            sizeStatistic_result_norm[i*6+j]=sizeStatistic_result[i*6+j]/_sum
            
    return sizeStatistic_result_norm

def allFeatureNorm_fine(arr):
    sizeStatistic_result=allFeature_core(arr);
    sizeStatistic_result_norm = np.zeros(24, float)
    for i in range(4):
        _sum=1
        for j in range(6):
            _sum+=sizeStatistic_result[i*6+j]
        for j in range(6):
            sizeStatistic_result_norm[i*6+j]=sizeStatistic_result[i*6+j]/_sum
            
    return sizeStatistic_result_norm



        
def SSDFeature(y,mean_arr):
    ssdArr=np.zeros(5)
    for i in range(4):
        ssdArr[i+1]=np.sum((y[i*6:i*6+6] - mean_arr[i*6:i*6+6]) ** 2)
        ssdArr[0] +=ssdArr[i+1]

    return ssdArr


