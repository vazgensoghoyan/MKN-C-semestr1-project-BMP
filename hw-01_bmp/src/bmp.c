#include "bmp.h"

const int ALIGNMENT = 4;

int calculate_padding(int size_of_el, int count) {
    int bytes_in_row = size_of_el * count;
    return (ALIGNMENT - bytes_in_row % ALIGNMENT) % ALIGNMENT;
}

int load_bmp(char *filepath, bmp_t *bitmap) {
    FILE *file = fopen(filepath, "rb");

    if (!file) return 1;

    bitmap->fileheader = malloc(sizeof(bmp_fileheader_t));
    if (bitmap->fileheader == NULL)
        return 1;

    bitmap->v5header = malloc(sizeof(bmp_v5header_t));
    if (bitmap->v5header == NULL)
        return 1;

    fread(bitmap->fileheader, sizeof(bmp_fileheader_t), 1, file);
    fread(bitmap->v5header, sizeof(bmp_v5header_t), 1, file);

    fseek(file, bitmap->fileheader->file_offset_to_pixels, SEEK_SET);

    int height = bitmap->v5header->image_height;
    int width = bitmap->v5header->image_width;

    int padding = calculate_padding(sizeof(pixel_t), width);
    
    pixel_t *pixels = malloc(width * height * sizeof(pixel_t));
    if (pixels == NULL)
        return 1;

    pixel_t **pxl_array = malloc(height * sizeof(pixel_t *));
    if (pxl_array == NULL)
        return 1;

    for (int x = 0; x < height; x++) {
        pxl_array[x] = pixels + x * width;

        for (int y = 0; y < width; y++) {
            fread(&pxl_array[x][y], sizeof(pixel_t), 1, file);
        }

        fseek(file, padding, SEEK_CUR);
    }

    bitmap->pixel_array = pxl_array;

    fclose(file);

    return 0;
}

int crop(bmp_t bmp, bmp_t *cropped, int x, int y, int width, int height) {
    if (width <= 0 || x < 0 || bmp.v5header->image_width < x + width)
		return 1;
    if (height <= 0 || y < 0 || bmp.v5header->image_height < y + height)
		return 1;

    y = bmp.v5header->image_height - y - height;

    cropped->fileheader = malloc(sizeof(bmp_fileheader_t));
    if (cropped->fileheader == NULL)
        return 1;

    cropped->v5header = malloc(sizeof(bmp_v5header_t));
    if (cropped->v5header == NULL)
        return 1;

    *(cropped->fileheader) = *bmp.fileheader;
    *(cropped->v5header) = *bmp.v5header;

    int padding = calculate_padding(sizeof(pixel_t), width);

    cropped->v5header->image_height = height;
    cropped->v5header->image_width = width;
    cropped->v5header->image_size = (sizeof(pixel_t) * width + padding) * height;

    cropped->fileheader->file_size = bmp.fileheader->file_size - \
                                    bmp.v5header->image_size + \
                                    cropped->v5header->image_size;

    pixel_t *pixels = malloc(width *height * sizeof(pixel_t));
    if (pixels == NULL)
        return 1;

    pixel_t **pxl_array = malloc(height * sizeof(pixel_t *));
    if (pxl_array == NULL)
        return 1;

    for (int i = 0; i < height; i++) {
        pxl_array[i] = pixels + i * width;

        for (int j = 0; j < width; j++)
            pxl_array[i][j] = bmp.pixel_array[x+i][y+j];
    }

    cropped->pixel_array = pxl_array;

    return 0;
}

int rotate(bmp_t *bmp) {
    int height = bmp->v5header->image_height;
    int width = bmp->v5header->image_width;

    pixel_t *pixels = malloc(height * width * sizeof(pixel_t));
    if (pixels == NULL)
        return 1;

    pixel_t **new_pixel_array = malloc(width * sizeof(pixel_t *));
    if (new_pixel_array == NULL)
        return 1;

    for (int i = 0; i < width; i++) {
        new_pixel_array[i] = pixels + i * height;

        for (int j = 0; j < height; j++)
            new_pixel_array[i][j] = bmp->pixel_array[j][width-i-1];
    }

    int padding = calculate_padding(sizeof(pixel_t), height);
    int new_image_size = (sizeof(pixel_t) * height + padding) * width;

    bmp->fileheader->file_size += new_image_size - bmp->v5header->image_size;
    bmp->v5header->image_size = new_image_size;

    bmp->v5header->image_height = width;
    bmp->v5header->image_width = height;

    bmp->pixel_array = new_pixel_array;

    return 0;
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