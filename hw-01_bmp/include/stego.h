#ifndef STEGO_H
#define STEGO_H

int insert(char *in_filepath, char *out_filepath, char *key_txt, char *msg_txt);
int extract(char *in_filepath, char *key_txt, char *msg_txt);

#endif