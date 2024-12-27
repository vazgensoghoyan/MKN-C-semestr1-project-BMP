#include "stego.h"
#include "bmp.h"

int symbol_to_int(char c) {
    if (c == ' ')
        return 26;
    if (c == '.')
        return 27;
    if (c == ',')
        return 28;
    return c - 'A';
}

char int_to_symbol(int x) {
    if (x == 26)
        return ' ';
    if (x == 27)
        return '.';
    if (x == 28)
        return ',';
    return x + 'A';
}

int insert(char *in_filepath, char *out_filepath, char *key_txt, char *msg_txt) {
    bmp_t *bitmap = malloc(sizeof(bmp_t));
    
    if (load_bmp(in_filepath, bitmap)) return 1;

    FILE *key_file = fopen(key_txt, "r");
    FILE *msg_file = fopen(msg_txt, "r");

    if (key_file == NULL || msg_file == NULL) return 1;

    char c;
    int secret_info, x, y;

    while (!feof(msg_file)) {
        secret_info = symbol_to_int( fgetc(msg_file) );

        for (int i = 0; i < 5; i++) {
            if (fscanf(key_file, "%d %d %c\n", &x, &y, &c) != 3)
                break;
            if (ferror(key_file))
                break;
            
            y = bitmap->v5header->image_height - 1 - y;

            pixel_t *p = bitmap->pixel_array[y] + x;
            
            if (c == 'B') p->B += (secret_info & 1) - (p->B & 1); 
            if (c == 'G') p->G += (secret_info & 1) - (p->G & 1);
            if (c == 'R') p->R += (secret_info & 1) - (p->R & 1);
                
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
    FILE *msg_file = fopen(msg_txt, "w");

    if (key_file == NULL || msg_file == NULL) return 1;

    char needed_char, c;
    int x, y, bytes_read = 0, current_info = 0;

    while (!feof(key_file)) {
        fscanf(key_file, "%d %d %c\n", &x, &y, &c);
        y = bitmap->v5header->image_height - 1 - y;

        if (c == 'B') needed_char = bitmap->pixel_array[y][x].B;
        if (c == 'G') needed_char = bitmap->pixel_array[y][x].G;
        if (c == 'R') needed_char = bitmap->pixel_array[y][x].R;

        current_info += (needed_char & 1) << (bytes_read++);

        if (bytes_read == 5) {
            bytes_read = 0;
            current_info = 0;
            fputc(int_to_symbol(current_info), msg_file);
        }
    }

    free(bitmap);

    fputc('\n', msg_file);

    fclose(key_file);
    fclose(msg_file);

    return 0;
}