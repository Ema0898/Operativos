#ifndef _UTILITIES_H
#define _UTILITIES_H

// Contiene funciones para leer y escribir los archivos que usa el servidor

int split(char *str, char c, char ***arr);
int checkDir(char *path);
void create_dirs(void);
char *concat(const char *s1, const char *s2);
void create_dir(char *dir);

#endif
