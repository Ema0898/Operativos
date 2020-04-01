#ifndef _UTILITIES_H
#define _UTILITIES_H

char *readFile(char *fileName);
int split(char *str, char c, char ***arr);
void getData(int *port, char **colors, char **histo, char **log);

#endif