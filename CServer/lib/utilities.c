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
  char *fileData = readFile("/home/ema0898/Programas/Operativos/Tarea1/config.conf");
  char **arr = NULL;
  int rows = 0, lines = 0;

  int fileLines = split(fileData, '\n', &arr);

  char keys[fileLines][10];
  char values[fileLines][30];

  memset(keys, 0, sizeof(keys));
  memset(values, 0, sizeof(values));

  while (rows < fileLines)
  {
    int length = strlen(arr[rows]);

    for (int i = 0; i < length; ++i)
    {
      if (arr[rows][i] == ':')
      {
        int offset = lines + 1;

        for (int j = offset; j < length; ++j)
        {
          values[rows][j - offset] = arr[rows][j];
        }

        break;
      }

      keys[rows][i] = arr[rows][i];
      lines++;
    }

    lines = 0;
    rows++;
  }

  for (int i = 0; i < 4; ++i)
  {
    if (strcmp(keys[i], "Port") == 0)
    {
      *port = (atoi(values[i]));
    }

    if (strcmp(keys[i], "DirColors") == 0)
    {
      *colors = values[i];
    }

    if (strcmp(keys[i], "DirHisto") == 0)
    {
      *histo = values[i];
    }

    if (strcmp(keys[i], "DirLog") == 0)
    {
      *log = values[i];
    }
  }
}