#include "stego.h"
#include "bmp.h"

#define ppppp(pixel, i, c) { pixel->c -= (pixel->c & 1); pixel->c += i % 2; }

int symbol_to_int(char c) {
    if (c == ' ')
        return 26;
    if (c == '.')
        return 27;
    if (c == ',')
        return 28;
    return c - 'a';
}

char int_to_sybmol(int x) {
    if (x == 26)
        return ' ';
    if (x == 27)
        return '.';
    if (x == 28)
        return ',';
    return x + 'a';
}

int insert(char *in_filepath, char *out_filepath, char *key_txt, char *msg_txt) {
    bmp_t *bitmap = malloc(sizeof(bmp_t));
    
    if (load_bmp(in_filepath, bitmap)) return 1;

    FILE *key_file = fopen(key_txt, "r");
    FILE *msg_file = fopen(msg_txt, "r");

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
            if (fscanf(key_file, "%d %d %c\n", &x, &y, &c) != 3)
                break;
            if (ferror(key_file))
                break;
            
            y = bitmap->v5header->image_height - 1 - y;

            pixel_t *p = &bitmap->pixel_array[y][x];
            
            if (c == 'B') ppppp(p, secret_info, G);
            if (c == 'G') ppppp(p, secret_info, G);
            if (c == 'R') ppppp(p, secret_info, R);
                
            secret_info >>= 1;

            if (feof(key_file))
                break;
        }
    }

    save_bmp(out_filepath, bitmap);
    free(bitmap);

    fclose(key_file);
    fclose(msg_file);

    return 0;
}

int extract(char *in_filepath, char *key_txt, char *msg_txt) {
    bmp_t *bitmap = malloc(sizeof(bmp_t));
    
    if (load_bmp(in_filepath, bitmap)) return 1;

    FILE *key_file = fopen(key_txt, "r");
    FILE *msg_file = fopen(msg_txt, "r");

    if (key_file == NULL || msg_file == NULL) return 1;

    char c;
    int x, y, bytes_read = 0, current_info = 0;

    while (!feof(key_file)) {
        fscanf(key_file, "%d %d %c\n", &x, &y, &c);
        y = bitmap->v5header->image_height - 1 - y;

        if (c == 'B') current_info += (bitmap->pixel_array[y][x].B & 1) << bytes_read;
        if (c == 'G') current_info += (bitmap->pixel_array[y][x].G & 1) << bytes_read;
        if (c == 'R') current_info += (bitmap->pixel_array[y][x].R & 1) << bytes_read;
        bytes_read++;

        if (bytes_read == 5) {
            bytes_read = 0;
            current_info = 0;
            fputc(int_to_sybmol(current_info), msg_file);
        }
    }

    free(bitmap);

    fputc('\n', msg_file);

    fclose(key_file);
    fclose(msg_file);

    return 0;
}