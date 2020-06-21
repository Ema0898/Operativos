#ifndef _UTILITIES_H
#define _UTILITIES_H

/* Library to used various general functions */

int split(char *str, char c, char ***arr);
int check_dir(char *path);
void create_dirs(char *buffer_name);
char *concat(const char *s1, const char *s2);
int check_bin_dir(void);
int is_number(char *text);
int is_float(char *s);

#endif
