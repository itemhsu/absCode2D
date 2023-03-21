#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

void save_gray_bmp(const char *filename, uint8_t *data, int width, int height) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    int row_padded = (width + 3) & (~3);
    int image_size = row_padded * height;

    file_header.bfType = 0x4D42;
    file_header.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + image_size;
    file_header.bfReserved1 = 0;
    file_header.bfReserved2 = 0;
    file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    info_header.biSize = sizeof(BITMAPINFOHEADER);
    info_header.biWidth = width;
    info_header.biHeight = -height;
    info_header.biPlanes = 1;
    info_header.biBitCount = 8;
    info_header.biCompression = 0;
    info_header.biSizeImage = image_size;
    info_header.biXPelsPerMeter = 2835;
    info_header.biYPelsPerMeter = 2835;
    info_header.biClrUsed = 256;
    info_header.biClrImportant = 256;

    fwrite(&file_header, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&info_header, sizeof(BITMAPINFOHEADER), 1, file);

    uint8_t color_table[256 * 4];
    int i = 0;
    for (i = 0; i < 256; i++) {
        color_table[i * 4] = i;
        color_table[i * 4 + 1] = i;
        color_table[i * 4 + 2] = i;
        color_table[i * 4 + 3] = 0;
    }
    fwrite(color_table, sizeof(color_table), 1, file);

    uint8_t *row_buffer = (uint8_t *)malloc(row_padded);
    for (i = 0; i < height; i++) {
        memcpy(row_buffer, data + i * width, width);
        memset(row_buffer + width, 0, row_padded - width);
        fwrite(row_buffer, row_padded, 1, file);
    }

    free(row_buffer);
    fclose(file);

    printf("Successfully saved gray BMP to %s\n", filename);
}

int main() {
    int width = 320;
    int height = 240;
    int data_size = 76800;
    uint8_t data[data_size];
    int i = 0;
	// Generate sample data
	for (i = 0; i < data_size; i++) {
    	data[i] = i % 256;
	}

	const char *filename = "output_gray.bmp";
	save_gray_bmp(filename, data, width, height);

	return 0;
}
