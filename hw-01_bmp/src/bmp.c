#include "bmp.h"

bmp_t load_bmp(char *filepath) {
    FILE *file = fopen(filepath, "rb");

    bmp_t bitmap;

    bitmap.fileheader = malloc(sizeof(bmp_fileheader_t));
    bitmap.v5header = malloc(sizeof(bmp_v5header_t));

    fread(bitmap.fileheader, sizeof(bmp_fileheader_t), 1, file);
    fread(bitmap.v5header, sizeof(bmp_v5header_t), 1, file);

    int height = bitmap.v5header->image_height;
    int width = bitmap.v5header->image_width;
    
    pixel_t *pixels = malloc(width*height*sizeof(pixel_t));
    pixel_t **pxl_array = malloc(height*sizeof(pixel_t *));

    fseek(file, bitmap.fileheader->file_offset_to_pixels, SEEK_SET);

    for (int x = 0; x < height; x++) {
        pxl_array[x] = pixels + x * width;

        for (int y = 0; y < width; y++) {
            fread(&pxl_array[x][y], sizeof(pixel_t), 1, file);
        }
    }

    bitmap.pixel_array = pxl_array;

    fclose(file);

    return bitmap;
}