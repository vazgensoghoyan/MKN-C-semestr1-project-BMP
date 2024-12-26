#include "bmp.h"

int main(int argc, char **argv) {
    char *target = argv[1];

    const int ARGS_COUNT_FOR_CR = 8;
    const int ARGS_COUNT_FOR_INS = 6;
    const int ARGS_COUNT_FOR_EXTR = 5;

    if (strcmp(target, "crop-rotate") == 0 && argc == ARGS_COUNT_FOR_CR) {
        char *IN_FILEPATH = argv[2];
        char *OUT_FILEPATH = argv[3];

        int X = atoi(argv[4]);
        int Y = atoi(argv[5]);
        int W = atoi(argv[6]);
        int H = atoi(argv[7]);

        bmp_t *bitmap = malloc(sizeof(bmp_t));

        if (load_bmp(IN_FILEPATH, bitmap))
            return 1;

        if (crop(bitmap, X, Y, W, H))
            return 1;

        if (rotate(bitmap))
            return 1;

        save_bmp(OUT_FILEPATH, bitmap);

        free_bmp(bitmap);

        return 0;
    }

    if (strcmp(target, "insert") && argc == ARGS_COUNT_FOR_INS) {
        char *IN_FILEPATH = argv[2];
        char *OUT_FILEPATH = argv[3];

        char *KEY_TXT = argv[4];
        char *MSG_TXT = argv[5];

        return insert(IN_FILEPATH, OUT_FILEPATH, KEY_TXT, MSG_TXT);
    }

    if (strcmp(target, "extract") && argc == ARGS_COUNT_FOR_EXTR) {
        char *IN_FILEPATH = argv[2];

        char *KEY_TXT = argv[3];
        char *MSG_TXT = argv[4];

        return 0;
    }

    return 1;
}