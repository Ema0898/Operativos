#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "imagefilters.h"

void readCounter(int *clientCounter)
{
  FILE *file = fopen("./server.client", "r");

  if (file != NULL)
  {
    char line[3];
    fgets(line, sizeof line, file);
    *clientCounter = atoi(line);

    fclose(file);
  }
  else
  {
    printf("File does not exists\n");
  }
}

void writeCounter(int clientCounter)
{
  FILE *file = fopen("./server.client", "w+");
  char line[3];
  sprintf(line, "%d", clientCounter);
  fputs(line, file);
  fclose(file);
}

// https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
int split(char *str, char c, char ***arr)
{
  int count = 1;
  int token_len = 1;
  int i = 0;
  char *p;
  char *t;

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
      count++;
    p++;
  }

  *arr = (char **)malloc(sizeof(char *) * count);
  if (*arr == NULL)
    exit(1);

  p = str;
  while (*p != '\0')
  {
    if (*p == c)
    {
      (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
      if ((*arr)[i] == NULL)
        exit(1);

      token_len = 0;
      i++;
    }
    p++;
    token_len++;
  }
  (*arr)[i] = (char *)malloc(sizeof(char) * token_len);
  if ((*arr)[i] == NULL)
    exit(1);

  i = 0;
  p = str;
  t = ((*arr)[i]);
  while (*p != '\0')
  {
    if (*p != c && *p != '\0')
    {
      *t = *p;
      t++;
    }
    else
    {
      *t = '\0';
      i++;
      t = ((*arr)[i]);
    }
    p++;
  }

  return count;
}

void getData(int *port, char **colors, char **histo, char **log)
{
  FILE *file = fopen("/home/ema0898/Programas/Operativos/Tarea1/config.conf", "r");

  int count = 0;
  char line[256];
  char **arr = NULL;
  char **arr2 = NULL;
  /* or other suitable maximum line size */
  while (fgets(line, sizeof line, file) != NULL) /* read a line */
  {
    split(line, ':', &arr);
    split(arr[1], '\n', &arr2);

    if (count == 0 && port != NULL)
    {
      *port = atoi(arr2[0]);
    }
    else if (count == 1 && colors != NULL)
    {
      *colors = arr2[0];
    }
    else if (count == 2 && histo != NULL)
    {
      *histo = arr2[0];
    }
    else if (count == 3 && log != NULL)
    {
      *log = arr2[0];
    }

    count++;

    free(arr);
    free(arr2);
  }
  fclose(file);
}

int main()
{
  // int port = 1777;
  // char *colors = "/home/ema0898/Programas/Operativos/Tarea1/media/classify/";
  // char *histo = "/home/ema0898/Programas/Operativos/Tarea1/media/";
  // char *logs = "/home/ema0898/Programas/Operativos/Tarea1/";
  // getData(&port, &colors, &histo, &logs);

  // FILE *file = fopen("/home/ema0898/Programas/Operativos/Tarea1/config.conf", "r");

  // char line[256];
  // char **arr = NULL;
  // char **arr2 = NULL;

  // fgets(line, sizeof line, file);
  // fgets(line, sizeof line, file);

  // split(line, ':', &arr);
  // split(arr[1], '\n', &arr2);

  // printf("%s", arr[1]);

  //printf("%d,\n %s,\n %s,\n %s,\n", port, colors, histo, logs);

  //classify("./media/", "lena.jpg", colors);

  //getData(NULL, NULL, NULL, NULL);
  int hola = 12;
  //writeCounter(hola);
  readCounter(&hola);
  printf("%d\n", hola);

  return 0;
}