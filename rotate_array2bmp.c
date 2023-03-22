//comile gcc xxx.c -lm
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1) 


#include <stdio.h>
#include <stdint.h>
#include <math.h>

//rotate and crop 42 x 42
//scale for 320x240 should be 4, for 160x120 should be 2
void create_matrices(uint8_t* ref_image, int ref_image_w, int ref_image_h, double theta, int scale, uint8_t* output_matrix, int output_matrix_w, int output_matrix_h) {

    int matrix3;
    int matrix4;
    int i, j;
    //如果陣列是來自bmp 因為上下顛倒的關係，theta 要加負號
    //如果陣列是來自sensor ，theta 正號
    double angle = (-theta * 3.1415926 / 180);
    double cos_angle = cos(angle);
    double sin_angle = sin(angle);

    // Fill output_matrix
    for (i = -output_matrix_h / 2; i < output_matrix_h / 2; i++) {
        for (j = -output_matrix_w / 2; j < output_matrix_w / 2; j++) {
            matrix3 = round((j*cos_angle + i*sin_angle + 40 - 0.5) * scale);
            matrix4 = round((-j*sin_angle + i*cos_angle + 30 - 0.5) * scale);
            output_matrix[(i + output_matrix_h / 2) * output_matrix_w + j + output_matrix_w / 2] = ref_image[matrix4 * ref_image_w + matrix3];

        }
    }
}






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

#pragma pack(pop)

int main() {
    int x,y, i;
    BMPHeader header;
    BMPInfoHeader info_header;

    FILE* file = fopen("gray_2023_1_16_15_56_33_Image.bmp", "rb");
    fread(&header, sizeof(BMPHeader), 1, file);
    fread(&info_header, sizeof(BMPInfoHeader), 1, file);

    int width = info_header.width;
    int height = info_header.height;

    unsigned char* image = (unsigned char*)malloc(width * height);

    fseek(file, header.offset, SEEK_SET);
    fread(image, sizeof(unsigned char), width * height, file);
    fclose(file);

    int crop_width = 42;
    int crop_height = 42;
    int crop_x = width / 2 - crop_width / 2;
    int crop_y = height / 2 - crop_height / 2;

    int crop_row_size = crop_width;
    if (crop_width % 4 != 0) {
        crop_row_size += 4 - crop_width % 4;
    }
    
    uint8_t output_matrix[42 * 42] = {0}; // 初始化輸出矩陣為全黑
    double theta = -79.2234532345284;
    int scale = 4;
    create_matrices(image, width, height, theta, scale, output_matrix, 42, 42);

    

    unsigned char* cropped_image = (unsigned char*)malloc(crop_row_size * crop_height);
    memset(cropped_image, 0, crop_row_size * crop_height);

    for (  y = 0; y < crop_height; y++) {
        for (  x = 0; x < crop_width; x++) {
            int original_x = crop_x + x;
            int original_y = crop_y + y;
            cropped_image[y * crop_row_size + x] = image[original_y * width + original_x];
        }
    }

    
    for (  y = 0; y < crop_height; y++) {
        for (  x = 0; x < crop_width; x++) {
            cropped_image[y * crop_row_size + x] = output_matrix[y * 42 + x];
        }
    }

    
    BMPHeader output_header = header;
    BMPInfoHeader output_info_header = info_header;
    output_info_header.width = crop_width;
    output_info_header.height = crop_height;
    output_info_header.bits_per_pixel = 8;
    output_info_header.image_size = crop_row_size * crop_height;
    output_info_header.num_colors = 256;
    output_info_header.important_colors = 256;
    output_header.size = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + 256 * 4 + crop_row_size * crop_height;
    output_header.offset = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + 256 * 4;

    unsigned char* palette = (unsigned char*)malloc(256 * 4);
    for (  i = 0; i < 256; i++) {
        palette[i * 4] = i;
        palette[i * 4 + 1] = i;
        palette[i * 4 + 2] = i;
        palette[i * 4 + 3] = 0;
    }

    file = fopen("output.bmp", "wb");
    fwrite(&output_header, sizeof(BMPHeader), 1, file);
    fwrite(&output_info_header, sizeof(BMPInfoHeader), 1, file);
    fwrite(palette, sizeof(unsigned char), 256 * 4, file);

    for (  y = 0; y < crop_height; y++) {
        fwrite(cropped_image + y * crop_row_size, sizeof(unsigned char), crop_width, file);
        if (crop_row_size > crop_width) {
            fwrite("\0\0\0", sizeof(unsigned char), crop_row_size - crop_width, file);
        }
    }

    fclose(file);

    free(image);
    free(cropped_image);
    free(palette);

    return 0;
}