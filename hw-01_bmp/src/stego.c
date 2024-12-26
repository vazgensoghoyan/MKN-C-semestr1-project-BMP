#include "stego.h"
#include "bmp.h"

#define ppppp(pixel, i, c) { pixel.c -= (pixel.c & 1); pixel.c += i % 2; }

int symbol_to_int(char c) {
    if (c == ' ')
        return 26;
    if (c == '.')
        return 27;
    if (c == ',')
        return 28;
    return c - 'a';
}

int insert(char *in_filepath, char *out_filepath, char *key_txt, char *msg_txt) {
    bmp_t *bitmap = malloc(sizeof(bmp_t));
    
    if (load_bmp(in_filepath, bitmap))
        return 1;

    FILE *key_file = fopen(key_txt, "rb");
    FILE *msg_file = fopen(msg_txt, "rb");

    if (key_file == NULL || msg_file == NULL) return 1;

    char current_char, c;
    int secret_info, x, y;

    while (!feof(msg_file)) {
        if (fread(&current_char, sizeof(char), 1, msg_file))
            break;
        if (ferror(msg_file))
            break;

        secret_info = symbol_to_int(current_char);
        for (int i = 0; i < 5; i++) {
            fscanf(key_file, "%d %d %c\n", &x, &y, &c);
            y = bitmap->v5header->image_height - 1 - y;
            
            if (c == 'B')
                ppppp(bitmap->pixel_array[y][x], secret_info, G);
            if (c == 'G')
                ppppp(bitmap->pixel_array[y][x], secret_info, G);
            if (c == 'R')
                ppppp(bitmap->pixel_array[y][x], secret_info, R);
                
            secret_info >>= 1;
        }
    }

    save_bmp(out_filepath, bitmap);
    free(bitmap);

    fclose(key_file);
    fclose(msg_file);

    return 0;
}
