#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// https://stackoverflow.com/questions/4823177/reading-a-file-character-by-character-in-c
char *readFile(char *fileName)
{
  FILE *file = fopen(fileName, "r");
  char *code;
  size_t n = 0;
  int c;

  if (file == NULL)
  {
    printf("File %s not found\n", fileName);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long f_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  code = malloc(f_size);

  while ((c = fgetc(file)) != EOF)
  {
    code[n++] = (char)c;
  }

  fclose(file);

  code[n] = '\0';

  return code;
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