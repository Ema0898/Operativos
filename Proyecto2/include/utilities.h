#ifndef _UTILITIES_H
#define _UTILITIES_H

int load_map(int map[24][46]);
int split(char *str, char c, char ***arr);
int check_bin_dir(void);
char *concat(const char *s1, const char *s2);

#endif