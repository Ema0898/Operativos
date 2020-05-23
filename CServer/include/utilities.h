#ifndef _UTILITIES_H
#define _UTILITIES_H

void readCounter(int *clientCounter);
void writeCounter(int clientCounter);
int split(char *str, char c, char ***arr);
void checkDir(char *path);
void checkColorsRoute(char *path);
void getData(int *port, char **colors, char **histo, char **log);

#endif