#include "bmp.h"

int main(int argc, char **argv) {
    if (argc != 8 || strcmp(argv[1], "crop-rotate") != 0) {
        return 1;
    }

    char *IN_FILEPATH = argv[2];
    char *OUT_FILEPATH = argv[3];

    int X = atoi(argv[4]);
    int Y = atoi(argv[5]);
    int W = atoi(argv[6]);
    int H = atoi(argv[7]);

    bmp_t bitmap;
    bmp_t cropped;

    if (load_bmp(IN_FILEPATH, &bitmap))
        return 1;
    
    if (crop(bitmap, &cropped, X, Y, W, H))
        return 1;

    if (rotate(&cropped))
        return 1;

    save_bmp(OUT_FILEPATH, cropped);

    free_bmp(bitmap);
    free_bmp(cropped);

    return 0;
}