#include "bmp.h"

int main(int argc, char **argv) {
    if (argc != 8 || strcmp(argv[1], "crop-rotate") != 0) {
        return 0;
    }

    char *IN_FILEPATH = argv[2];
    char *OUT_FILEPATH = argv[3];

    int X = atoi(argv[4]);
    int Y = atoi(argv[5]);
    int W = atoi(argv[6]);
    int H = atoi(argv[7]);

    bmp_t bitmap = load_bmp(IN_FILEPATH);

    return 0;
}