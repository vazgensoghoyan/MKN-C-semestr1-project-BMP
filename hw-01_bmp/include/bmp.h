#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h> 

#ifndef BMP_H_
#define BMP_H_

typedef struct {

    char R;
    char G;
    char B;

} __attribute__((packed)) pixel_t;

typedef struct {
    
    char signature[2];
    unsigned int file_size;
    unsigned int reserved;
    unsigned int file_offset_to_pixels;

} __attribute__((packed)) bmp_fileheader_t;

typedef struct {

    unsigned int header_size;
    unsigned int image_width;
    unsigned int image_height;
    int smth1;
    int smth2;
    unsigned int image_size;

} __attribute__((packed)) bmp_v5header_t;

typedef struct {

    bmp_fileheader_t *fileheader;
    bmp_v5header_t *v5header;

    pixel_t **pixel_array;

} bmp_t;

int load_bmp(char *filepath, bmp_t *bitmap);
int crop(bmp_t bmp, bmp_t *cropped, int x, int y, int width, int height);
int rotate(bmp_t *bmp);
void save_bmp(char *filepath, bmp_t bmp);
void free_bmp(bmp_t bmp);

#endif