#include "bmp.h"

const int ALIGNMENT = 4;

int calculate_padding(int size_of_el, int count) {
    int bytes_in_row = size_of_el * count;
    return (ALIGNMENT - bytes_in_row % ALIGNMENT) % ALIGNMENT;
}

bmp_t load_bmp(char *filepath) {
    FILE *file = fopen(filepath, "rb");

    bmp_t bitmap;

    bitmap.fileheader = malloc(sizeof(bmp_fileheader_t));
    bitmap.v5header = malloc(sizeof(bmp_v5header_t));

    fread(bitmap.fileheader, sizeof(bmp_fileheader_t), 1, file);
    fread(bitmap.v5header, sizeof(bmp_v5header_t), 1, file);

    fseek(file, bitmap.fileheader->file_offset_to_pixels, SEEK_SET);

    int height = bitmap.v5header->image_height;
    int width = bitmap.v5header->image_width;

    int padding = calculate_padding(sizeof(pixel_t), width);
    
    pixel_t *pixels = malloc(width*height*sizeof(pixel_t));
    pixel_t **pxl_array = malloc(height*sizeof(pixel_t *));

    for (int x = 0; x < height; x++) {
        pxl_array[x] = pixels + x * width;

        for (int y = 0; y < width; y++) {
            fread(&pxl_array[x][y], sizeof(pixel_t), 1, file);
        }

        fseek(file, padding, SEEK_CUR);
    }

    bitmap.pixel_array = pxl_array;

    fclose(file);

    return bitmap;
}

void save_bmp(char *filepath, bmp_t bmp) {
    FILE *file = fopen(filepath, "wb");
    
    fwrite(bmp.fileheader, sizeof(bmp_fileheader_t), 1, file);
    fwrite(bmp.v5header, sizeof(bmp_v5header_t), 1, file);

    fseek(file, bmp.fileheader->file_offset_to_pixels, SEEK_SET);

    int height = bmp.v5header->image_height;
    int width = bmp.v5header->image_width;

    int padding = calculate_padding(sizeof(pixel_t), width);

    for (int i = 0; i < height; i++) {
        fwrite(bmp.pixel_array[i], sizeof(pixel_t), width, file);
        
        for (int j = 0; j < padding; j++)
            fputc(0, file);
    }

    fclose(file);
}

void free_bmp(bmp_t bmp) {
    free(bmp.fileheader);
    free(bmp.v5header);

    free(bmp.pixel_array[0]);
    free(bmp.pixel_array);
}